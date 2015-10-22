/*
CuteTorrent BitTorrenttClient with dht support, userfriendly interface
and some additional features which make it more convenient.
Copyright (C) <year>  <name of author>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stddef.h>
#include <QAbstractItemView>
#include <QClipboard>
#include <QComboBox>
#include <QDateTime>
#include <QDebug>
#include <QDesktopServices>
#include <QProcess>
#include <QShortcut>
#include <QSortFilterProxyModel>
#include <QTreeWidgetItem>
#include <QUrl>
#include <QtNetwork/QHostAddress>
#include "CreateTorrentDialog.h"
#include "CuteTorrentMainWindow.h"
#include "OpenTorrentDialog.h"
#include "QApplicationSettings.h"
#include "QSearchDisplayModel.h"
#include "QSearchItemDelegate.h"
#include "QTorrentDisplayModel.h"
#include "QTorrentFilterProxyModel.h"
#include "QTorrentItemDelegat.h"
#include "QRssDisplayModel.h"
#include "QRssItemDelegate.h"
#include "RssManager.h"
#include "Scheduller.h"
#include "SearchEngine.h"
#include "SettingsDialog.h"
#include "Torrent.h"
#include "TorrentManager.h"
#include "UpdateNotyfier.h"
#include "defs.h"
#include "peicedisplaywidget.h"
#include "FileSizeItemDelegate.h"
#include "ProgressItemDelegate.h"
#include "PriorityItemDelegate.h"
#include "torrentracker.h"
#include "Version.h"
#include "RconWebService.h"
#include "messagebox.h"
#include "backupwizard/backupwizard.h"
#include "RssFeedSettingsDialog.h"
#include "HtmlView.h"
#include "NotificationSystem.h"
#include <libtorrent/peer_info.hpp>
#include "RssItem.h"
#include "RconWebService.h"
#include "StaticHelpers.h"
#include "SearchResult.h"
#ifdef Q_WS_WIN
#include "qwinjumplist.h"
#include "qwinjumplistcategory.h"
#endif
#include "PieceAvailabilityWidget.h"
#include <FileSystemTorrentWatcher.h>
#include <PeerTableModel.h>
#include "PeerSortModel.h"
#include "SpeedItemDelegate.h"
#include <viewModel/itemDelegate/IpItemDelegate.h>
#include "InitializationDialog.h"
#include <FaviconDownloader.h>
#include <AddRssDwonloadRuleDialog.h>
class Application;
class ISerachProvider;
class SearchResult;

Q_DECLARE_METATYPE(QList<int>)

CuteTorrentMainWindow::CuteTorrentMainWindow(QWidget* parent)
	: BaseWindow(FullTitle, AllowResize, parent)
	, m_httpLinkRegexp("(http|ftp|https):\\/\\/[\\w\\-_]+(\\.[\\w\\-_]+)+([\\w\\-\\.,@?^=%&amp;:/~\\+#]*[\\w\\-\\@?^=%&amp;/~\\+#])?")
	, m_pPieceView(NULL)
	, m_pSearchEngine(SearchEngine::getInstance())
	, m_pieceAvalibilityWidget(NULL)
	, m_pPeerTableModel(NULL)
	, m_initFinished(false)
	, m_pNotificationSystem(NotificationSystem::getInstance())
	, m_pItemSorterView(new EditableHeaderView(Qt::Horizontal, this))
#ifdef Q_WS_WIN
	, m_pJumpList(new QWinJumpList(this))
#endif
{
	m_pSettings = QApplicationSettings::getInstance();
	Application::setLanguage(m_pSettings->valueString("System", "Lang", "en_US"));
	Application::setLanguageQt(m_pSettings->valueString("System", "Lang", "en_US"));
	m_pStyleEngine = StyleEngene::getInstance();
	m_pStyleEngine->setStyle(m_pSettings->valueString("System", "Style", "CuteTorrent"));
	m_pTorrentGroupsManager = TorrentGroupsManager::getInstance();
	setWindowModality(Qt::NonModal);
	setupCustomeWindow();
	m_pUpdateTimer = new QTimer(this);
	m_pUpdateTimer->setInterval(400);
	m_pTorrentManager = TorrentManager::getInstance();
	m_pTorrentFilterProxyModel = new QTorrentFilterProxyModel(this);
	m_pTorrentDisplayModel = new QTorrentDisplayModel(m_pTorrentListView, m_pTorrentFilterProxyModel, this);
	m_pTorrentFilterProxyModel->setSourceModel(m_pTorrentDisplayModel);
	m_pSearchFilterModel = new QSearchFilterModel();
	m_pSearchDisplayModel = new QSearchDisplayModel(m_pTorrentListView, m_pSearchFilterModel, this);
	m_pSearchFilterModel->setSourceModel(m_pSearchDisplayModel);
	m_pSearchItemDelegate = new QSearchItemDelegate(this);
	m_pRssDisplayModel = new QRssDisplayModel(m_pTorrentListView, this);
	m_pRssItemDelegate = new QRssItemDelegate(this);
	m_pUpdateNotifier = new UpdateNotifier(this);
	m_pFiltersViewModel = new FiltersViewModel(FiltersViewModel::All, this);
	boost::scoped_ptr<InitializationDialog> pDlg(new InitializationDialog(this));
	pDlg->exec();
	setAcceptDrops(true);
	setupStatusBar();
	setupTray();
	setupFileTabel();
	setupToolBar();
	setupListView();
	setupRssInfoTab();
	setupTabelWidgets();
	setupGroupTreeWidget();
	setupConnections();
#ifdef Q_WS_WIN
	setupJumpList();
#endif
	setupKeyMappings();
	initWindowIcons();
	m_pTracker = TorrentTracker::getInstance();
	m_pRcon = RconWebService::getInstance();

	if (m_pSettings->valueBool("WebControl", "webui_enabled", false))
	{
		m_pRcon->Start();

		if (m_pSettings->valueBool("WebControl", "enable_ipfilter", false))
		{
			m_pRcon->parseIpFilter(m_pSettings->valueString("WebControl", "ipfilter"));
		}

		if (m_pSettings->valueBool("WebControl", "enable_upnp", false))
		{
			int port = m_pSettings->valueInt("WebControl", "port", 8080);
			m_pTorrentManager->AddPortMapping(
#if LIBTORRENT_VERSION_NUM >= 10000
			    session::tcp,
#else
			    upnp::tcp,
#endif
			    port, port);
		}
	}

	bool isScriptDebuggingEnabled = m_pSettings->valueBool("Search", "script_debuging_enabled", false);
	SearchEnginePtr searchEngine = SearchEngine::getInstance();

	if (isScriptDebuggingEnabled)
	{
		if (!searchEngine->isEnabledScriptDebugging())
		{
			searchEngine->enableScriptDebugging();
		}
	}
	else
	{
		if (searchEngine->isEnabledScriptDebugging())
		{
			searchEngine->disableScriptDebugging();
		}
	}

	m_pTorrentWatcher = FileSystemTorrentWatcher::getInstance();

	if (m_pSettings->valueBool("WatchDir", "enabled"))
	{
		QString watchDir = m_pSettings->valueString("WatchDir", "dir_to_watch");

		if (!watchDir.isEmpty())
		{
			m_pTorrentWatcher->addPath(watchDir);
		}
	}

	if (m_pSettings->valueBool("TorrentTracker", "enabled", false))
	{
		m_pTracker->start();
	}

	m_centralWidget->installEventFilter(this);
	m_pTorrentListView->installEventFilter(this);
	m_pGroupTreeView->installEventFilter(this);
	m_pTabWidget->installEventFilter(this);
	m_pTorrents = TorrentStorrage::getInstance();
	QTimer::singleShot(10000, this, SLOT(CheckForUpdates()));
	Scheduller::getInstance();
	m_pUpdateTimer->start();
	m_initFinished = true;
}

void CuteTorrentMainWindow::CheckForUpdates()
{
	m_pUpdateNotifier->fetch();
}

void CuteTorrentMainWindow::ShowAbout()
{
	CustomMessageBox::about(this, tr("ABAUT_TITLE"), tr("ABAUT_TEXT").arg(Version::getVersionStr()));
}

void CuteTorrentMainWindow::initStatusBarIcons()
{
	upLabel->setPixmap(m_pStyleEngine->getIcon("status_bar_upload").pixmap(16, 16));
	downLabel->setPixmap(m_pStyleEngine->getIcon("status_bar_download").pixmap(16, 16));
}

void CuteTorrentMainWindow::setupStatusBar()
{
	upLabel = new QLabel(this);
	upLabelText = new QLabel(this);
	dhtNodesLabel = new QLabel(this);
	upLabelText->setMinimumWidth(140);
	downLabel = new QLabel(this);
	downLabelText = new QLabel(this);
	downLabel->setToolTip(tr("STATUS_DWONLOAD"));
	downLabelText->setToolTip(tr("STATUS_DWONLOAD"));
	upLabel->setToolTip(tr("STATUS_UPLOAD"));
	upLabelText->setToolTip(tr("STATUS_UPLOAD"));
	downLabelText->setMinimumWidth(140);
	mystatusBar->addPermanentWidget(dhtNodesLabel);
	mystatusBar->addPermanentWidget(upLabel);
	mystatusBar->addPermanentWidget(upLabelText);
	mystatusBar->addPermanentWidget(downLabel);
	mystatusBar->addPermanentWidget(downLabelText);
	m_pPieceView = new PeiceDisplayWidget(this);
	gridLayout_4->addWidget(m_pPieceView, 0, 0, 1, 1);
	initStatusBarIcons();
	m_pieceAvalibilityWidget = new PieceAvailabilityWidget(this);
	gridLayout_4->addWidget(m_pieceAvalibilityWidget, 1, 0, 1, 1);
}

void CuteTorrentMainWindow::setupTorrentHeaderModel()
{
	m_torrentHeaderModel = new QStandardItemModel(0, 7, this);
	m_torrentHeaderModel->setHeaderData(0, Qt::Horizontal, "#", Qt::DisplayRole);
	m_torrentHeaderModel->setHeaderData(1, Qt::Horizontal, tr("SORTER_NAME"), Qt::DisplayRole);
	m_torrentHeaderModel->setHeaderData(2, Qt::Horizontal, tr("SORTER_SIZE"), Qt::DisplayRole);
	m_torrentHeaderModel->setHeaderData(3, Qt::Horizontal, tr("SORTER_DOWNLOADED"), Qt::DisplayRole);
	m_torrentHeaderModel->setHeaderData(4, Qt::Horizontal, tr("SORTER_UPLOADED"), Qt::DisplayRole);
	m_torrentHeaderModel->setHeaderData(5, Qt::Horizontal, tr("SORTER_UPTIME"), Qt::DisplayRole);
	m_torrentHeaderModel->setHeaderData(6, Qt::Horizontal, tr("SORTER_TIME_LEFT"), Qt::DisplayRole);
	m_torrentHeaderModel->setHeaderData(0, Qt::Horizontal, QTorrentDisplayModel::TorrentQueuePosition, Qt::UserRole);
	m_torrentHeaderModel->setHeaderData(1, Qt::Horizontal, QTorrentDisplayModel::TorrentName, Qt::UserRole);
	m_torrentHeaderModel->setHeaderData(2, Qt::Horizontal, QTorrentDisplayModel::TorrentSize, Qt::UserRole);
	m_torrentHeaderModel->setHeaderData(3, Qt::Horizontal, QTorrentDisplayModel::TorrentTotalDownloaded, Qt::UserRole);
	m_torrentHeaderModel->setHeaderData(4, Qt::Horizontal, QTorrentDisplayModel::TorrentTotalUploaded, Qt::UserRole);
	m_torrentHeaderModel->setHeaderData(5, Qt::Horizontal, QTorrentDisplayModel::TorrentUptime, Qt::UserRole);
	m_torrentHeaderModel->setHeaderData(6, Qt::Horizontal, QTorrentDisplayModel::TorrentRemainingTime, Qt::UserRole);
}

void CuteTorrentMainWindow::setupSearchHeaderModel()
{
	m_searchHeaderModel = new QStandardItemModel(0, 4, this);
	m_searchHeaderModel->setHeaderData(0, Qt::Horizontal, tr("SORTER_NAME"), Qt::DisplayRole);
	m_searchHeaderModel->setHeaderData(1, Qt::Horizontal, tr("SORTER_SIZE"), Qt::DisplayRole);
	m_searchHeaderModel->setHeaderData(2, Qt::Horizontal, tr("SORTER_LEECHERS"), Qt::DisplayRole);
	m_searchHeaderModel->setHeaderData(3, Qt::Horizontal, tr("SORTER_SEEDERS"), Qt::DisplayRole);
	m_searchHeaderModel->setHeaderData(0, Qt::Horizontal, QSearchDisplayModel::SearchItemName, Qt::UserRole);
	m_searchHeaderModel->setHeaderData(1, Qt::Horizontal, QSearchDisplayModel::SearchItemSize, Qt::UserRole);
	m_searchHeaderModel->setHeaderData(2, Qt::Horizontal, QSearchDisplayModel::SearchItemPeers, Qt::UserRole);
	m_searchHeaderModel->setHeaderData(3, Qt::Horizontal, QSearchDisplayModel::SearchItemSeeders, Qt::UserRole);
}

void CuteTorrentMainWindow::setupListView()
{
	m_pTorrentItemDelegate = new QTorrentItemDelegat(this);
	m_pTorrentListView->setItemDelegate(m_pTorrentItemDelegate);
	m_pTorrentListView->setModel(m_pTorrentFilterProxyModel);
	setupTorrentHeaderModel();
	setupSearchHeaderModel();
	m_pItemSorterView->setObjectName("torrentSorterView");
	m_pItemSorterView->setModel(m_torrentHeaderModel);
	m_pItemSorterView->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	m_pItemSorterView->setSortIndicatorShown(true);
	m_pItemSorterView->setDefaultSectionSize(60);
	m_pItemSorterView->setClickable(true);
	m_pItemSorterView->setMinimumSectionSize(20);
	QList<int> defaultSizes = QList<int>() << 40 << 70 << 60 << 70 << 80 << 80 << 80;
	loadHeaderState("torrent_sorter", m_pItemSorterView, defaultSizes);
	QTorrentDisplayModel::Role sortRole = QTorrentDisplayModel::Role(m_pSettings->valueInt("Window", "torrent_sorter_sort_role", QTorrentDisplayModel::TorrentName));
	Qt::SortOrder order = Qt::SortOrder(m_pSettings->valueInt("Window", "torrent_sorter_sort_order", Qt::AscendingOrder));
	int columnIndex = sortRole - QTorrentDisplayModel::TorrentQueuePosition;
	m_pItemSorterView->setResizeMode(QHeaderView::Stretch);
	m_pItemSorterView->setSortIndicator(columnIndex, order);
	m_pTorrentFilterProxyModel->setSortRole(sortRole);
	m_pTorrentFilterProxyModel->sort(0, order);
	torrentViewLayout->insertWidget(0, m_pItemSorterView);
	m_lastSorter = TORRENT;
}

void CuteTorrentMainWindow::setupTabelWidgets()
{
	QList<int> defaultTrackerColumnSizes = QList<int>() << 320 << 60 << 50 << 50;
	m_pTrackersHeader = new EditableHeaderView(Qt::Horizontal, trackerTableWidget);
	trackerTableWidget->setHorizontalHeader(m_pTrackersHeader);
	loadHeaderState("trackers", m_pTrackersHeader, defaultTrackerColumnSizes);
	trackerTableWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
	addTracker = new QAction(m_pStyleEngine->getIcon("add_torrent"), tr("ADD_TRACKER"), trackerTableWidget);
	addTracker->setObjectName("ACTION_TRACKER_ADD");
	removeTracker = new QAction(m_pStyleEngine->getIcon("delete"), tr("REMOVE_TRACKER"), trackerTableWidget);
	removeTracker->setObjectName("ACTION_TRACKER_REMOVE");
	editTracker = new QAction(m_pStyleEngine->getIcon("delete"), tr("EDIT_TRACKER"), trackerTableWidget);
	editTracker->setObjectName("ACTION_TRACKER_EDIT");
	updateTracker = new QAction(m_pStyleEngine->getIcon("update_trackers"), tr("UPDATE_TRACKER"), trackerTableWidget);
	updateTracker->setObjectName("ACTION_TRACKER_UPDATE");
	connect(updateTracker, SIGNAL(triggered()), this, SLOT(UpdateTracker()));
	connect(addTracker, SIGNAL(triggered()), this, SLOT(AddTracker()));
	connect(removeTracker, SIGNAL(triggered()), this, SLOT(RemoveTracker()));
	connect(editTracker, SIGNAL(triggered()), this, SLOT(EditTracker()));
	QAction* separator = new QAction(trackerTableWidget);
	separator->setSeparator(true);
	trackerTableWidget->addAction(updateTracker);
	trackerTableWidget->addAction(separator);
	trackerTableWidget->addAction(addTracker);
	trackerTableWidget->addAction(removeTracker);
	trackerTableWidget->addAction(editTracker);
	QList<int> defaultPeerColumnSizes = QList<int>() << 100 << 100 << 50 << 100 << 100 << 100 << 100 << 100;
	m_pPeerTableModel = new PeerTableModel(peerTableView);
	PeerSortModel* peerSorter = new PeerSortModel(peerTableView);
	peerSorter->setSourceModel(m_pPeerTableModel);
	peerTableView->setModel(peerSorter);
	m_pPeersHeader = new EditableHeaderView(Qt::Horizontal, peerTableView);
	peerTableView->setHorizontalHeader(m_pPeersHeader);
	loadHeaderState("peers", m_pPeersHeader, defaultPeerColumnSizes);
	m_pPeersHeader->setClickable(true);
	peerTableView->setContextMenuPolicy(Qt::ActionsContextMenu);
	addPeer = new QAction(m_pStyleEngine->getIcon("add_torrent"), tr("ADD_PEER"), peerTableView);
	addPeer->setObjectName("ACTION_PEER_ADD");
	addWebSeed = new QAction(m_pStyleEngine->getIcon("add_torrent"), tr("ADD_WEB_SEED"), peerTableView);
	addWebSeed->setObjectName("ACTION_PEER_ADD_WEB_SEED");
	connect(addPeer, SIGNAL(triggered()), this, SLOT(AddPeer()));
	connect(addWebSeed, SIGNAL(triggered()), this, SLOT(AddWebSeed()));
	peerTableView->addAction(addPeer);
	peerTableView->addAction(addWebSeed);
	peerTableView->setItemDelegateForColumn(PeerTableModel::IP, new IpItemDelegate(this));
	peerTableView->setItemDelegateForColumn(PeerTableModel::DOWNLOADED, new FileSizeItemDelegate(true, this));
	peerTableView->setItemDelegateForColumn(PeerTableModel::UPLOADED, new FileSizeItemDelegate(true, this));
	peerTableView->setItemDelegateForColumn(PeerTableModel::DOWNLOAD_RATE, new SpeedItemDelegate(this));
	peerTableView->setItemDelegateForColumn(PeerTableModel::UPLOAD_RATE, new SpeedItemDelegate(this));
	peerTableView->setItemDelegateForColumn(PeerTableModel::PEER_SPEED, new SpeedItemDelegate(this));
	peerTableView->setItemDelegateForColumn(PeerTableModel::PEER_PROGRESS, new ProgressItemDelegate(this));
}

void CuteTorrentMainWindow::initToolbarIcons()
{
	ACTION_TOOLBAR_START->setIcon(m_pStyleEngine->getIcon("toolbar_start"));
	ACTION_TOOLBAR_PAUSE->setIcon(m_pStyleEngine->getIcon("toolbar_pause"));
	ACTION_TOOLBAR_REMOVE->setIcon(m_pStyleEngine->getIcon("toolbar_remove"));
	ACTION_TOOLBAR_SETTINGS->setIcon(m_pStyleEngine->getIcon("toolbar_settings"));
	ACTION_TOOLBAR_DOWNLOAD->setIcon(m_pStyleEngine->getIcon("toolbar_download"));
	ACTION_TOOLBAR_OPEN_URL->setIcon(m_pStyleEngine->getIcon("toolbar_open_url"));
	ACTION_TOOLBAR_RSS_ADD->setIcon(m_pStyleEngine->getIcon("toolbar_add_rss"));
	ACTION_TOOLBAR_RSS_REMOVE->setIcon(m_pStyleEngine->getIcon("toolbar_remove"));
	ACTION_TOOLBAR_RSS_EDIT->setIcon(m_pStyleEngine->getIcon("toolbar_edit_rss"));
	ACTION_TOOLBAR_QUEUE_UP->setIcon(m_pStyleEngine->getIcon("toolbar_queue_up"));
	ACTION_TOOLBAR_QUEUE_DOWN->setIcon(m_pStyleEngine->getIcon("toolbar_queue_down"));
}

void CuteTorrentMainWindow::setupToolBar()
{
	initToolbarIcons();
	m_pSearchCategory = new QComboBox(this);
	QMap<ISerachProvider::SearchCategories, QString> categoriesToStr;
	categoriesToStr[ISerachProvider::Anime] = tr("ANIME_CATEGORY");
	categoriesToStr[ISerachProvider::Music] = tr("MUSIC_CATEGORY");
	categoriesToStr[ISerachProvider::TV] = tr("TV_CATEGORY");
	categoriesToStr[ISerachProvider::Porn] = tr("PORN_CATEGORY");
	categoriesToStr[ISerachProvider::Software] = tr("APP_CATEGORY");
	categoriesToStr[ISerachProvider::Games] = tr("GAMES_CATEGORY");
	categoriesToStr[ISerachProvider::Books] = tr("BOOKS_CATEGORY");
	categoriesToStr[ISerachProvider::Movie] = tr("FILMS_CATEGORY");;
	categoriesToStr[ISerachProvider::All] = tr("ALL_CATEGORY");;;

	for (int i = categoriesToStr.size() - 1; i >= 0; i--)
	{
		m_pSearchCategory->addItem(categoriesToStr.values().at(i), categoriesToStr.keys().at(i));
	}

	m_pSearchCategory->setCurrentIndex(0);
	m_pSearchEdit = new QLineEdit(this);
	m_pSearchEdit->setPlaceholderText(tr("Search"));
	m_pTorrentSearchEdit = new QLineEdit(this);
	m_pTorrentSearchEdit->setPlaceholderText(tr("Search"));
	connect(m_pTorrentSearchEdit, SIGNAL(textChanged(const QString&)), this, SLOT(PeformTorrentSearch(const QString&)));
	connect(m_pSearchEdit, SIGNAL(returnPressed()), this, SLOT(PeformSearch()));
	ul = new QSpinBox(this);
	ul->setSpecialValueText(tr("None"));
	ul->setSuffix(" Kb\\s");
	ul->setMaximum(12000);
	ul->setSingleStep(10);
	ul->setButtonSymbols(QAbstractSpinBox::PlusMinus);
	connect(ul, SIGNAL(valueChanged(int)), this, SLOT(UpdateUL(int)));
	dl = new QSpinBox(this);
	dl->setSpecialValueText(tr("None"));
	dl->setMaximum(12000);
	dl->setSuffix(" Kb\\s");
	dl->setSingleStep(10);
	dl->setButtonSymbols(QAbstractSpinBox::PlusMinus);
	connect(dl, SIGNAL(valueChanged(int)), this, SLOT(UpdateDL(int)));
	uploadLimit = new QLabel(tr("LIMIT_UL"), this);
	uploadLimit->setBuddy(ul);
	downloadLimit = new QLabel(tr("LIMIT_DL"), this);
	downloadLimit->setBuddy(dl);
	m_pSearchCategory->setMinimumWidth(120);
	m_pTorrentToolBar->addWidget(uploadLimit);
	m_pTorrentToolBar->addWidget(ul);
	m_pTorrentToolBar->addWidget(downloadLimit);
	m_pTorrentToolBar->addWidget(dl);
	m_pTorrentToolBar->addWidget(m_pTorrentSearchEdit);
	m_pTorrentSearchEdit->setContentsMargins(0, 0, 5, 0);
	QWidget* searchSpacer = new QWidget();
	searchSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	m_pSearchToolBar->addWidget(searchSpacer);
	m_pSearchEdit->setContentsMargins(0, 0, 5, 0);
	m_pSearchToolBar->addWidget(m_pSearchCategory);
	m_pSearchToolBar->addWidget(m_pSearchEdit);
	UpdateTabWidget();
}

void CuteTorrentMainWindow::setupConnections()
{
	connect(m_pTorrentListView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
	        m_pTorrentDisplayModel, SLOT(UpdateSelectedIndex(const QItemSelection&)));
	connect(m_pTorrentListView, SIGNAL(customContextMenuRequested(const QPoint&)), m_pTorrentDisplayModel, SLOT(contextualMenu(const QPoint&)));
	connect(m_pTorrentListView, SIGNAL(customContextMenuRequested(const QPoint&)), m_pRssDisplayModel, SLOT(contextualMenu(const QPoint&)));
	connect(m_pTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
	        this, SLOT(IconActivated(QSystemTrayIcon::ActivationReason)));
	connect(ACTION_MENU_EXIT, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(m_pTabWidget, SIGNAL(currentChanged(int)), this, SLOT(UpdateTabWidget()));
	connect(m_pTorrentListView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
	        this, SLOT(UpdateTabWidget()));
	connect(m_pTorrentListView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
	        this, SLOT(UpdateLimits()));
	connect(m_pUpdateTimer, SIGNAL(timeout()), SLOT(UpdateStatusbar()));
	connect(m_pUpdateTimer, SIGNAL(timeout()), SLOT(UpdateTabWidget()));
	connect(fileTableView, SIGNAL(customContextMenuRequested(const QPoint&)), m_pFileViewModel, SLOT(FileTabContextMenu(const QPoint&)));
	connect(m_pGroupTreeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(ChnageTorrentFilter()));
	connect(m_pStyleEngine, SIGNAL(styleChanged()), this, SLOT(initWindowIcons()));
	connect(m_pStyleEngine, SIGNAL(styleChanged()), m_pTorrentDisplayModel, SLOT(setupContextMenu()));
	connect(m_pTorrentListView, SIGNAL(doubleClicked(const QModelIndex&)), m_pTorrentDisplayModel, SLOT(OpenDirSelected()));
	connect(m_pTorrentListView, SIGNAL(doubleClicked(const QModelIndex&)), m_pSearchDisplayModel, SLOT(downloadTorrent()));
	connect(ACTION_TOOLBAR_DOWNLOAD, SIGNAL(triggered()), m_pSearchDisplayModel, SLOT(downloadTorrent()));
	connect(ACTION_MENU_ABOUT_QT, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	connect(qApp, SIGNAL(aboutToQuit()), SLOT(OnQuit()));
	connect(m_pItemSorterView, SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)), SLOT(updateSorting(int, Qt::SortOrder)));
}

void CuteTorrentMainWindow::UpdateStatusbar()
{
	m_pTrayIcon->setToolTip(tr("CuteTorrent %1\nUpload: %2\nDownload: %3").arg(Version::getVersionStr(), m_pTorrentManager->GetSessionUploadSpeed(), m_pTorrentManager->GetSessionDownloadSpeed()));

	if (this->isMinimized())
	{
		return;
	}

	upLabelText->setText(QString("%1(%2)").arg(m_pTorrentManager->GetSessionUploaded(), m_pTorrentManager->GetSessionUploadSpeed()));
	downLabelText->setText(QString("%1(%2)").arg(m_pTorrentManager->GetSessionDownloaded(), m_pTorrentManager->GetSessionDownloadSpeed()));
	dhtNodesLabel->setText(tr("DHT: %1 nodes").arg(m_pTorrentManager->GetSessionDHTstate()));
}

void CuteTorrentMainWindow::UpdateLimits()
{
	if (this->isMinimized())
	{
		return;
	}

	if (m_pTorrentListView->model() != m_pTorrentFilterProxyModel)
	{
		return;
	}

	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if (tor != NULL)
	{
		if (ul->value() != tor->GetUploadLimit() / KbInt)
		{
			ul->setValue(tor->GetUploadLimit() / KbInt);
		}

		if (dl->value() != tor->GetDownloadLimit() / KbInt)
		{
			dl->setValue(tor->GetDownloadLimit() / KbInt);
		}
	}
	else
	{
		if (ul->value() != m_pTorrentManager->GetUploadLimit() / KbInt)
		{
			ul->setValue(m_pTorrentManager->GetUploadLimit() / KbInt);
		}

		if (dl->value() != m_pTorrentManager->GetDownloadLimit() / KbInt)
		{
			dl->setValue(m_pTorrentManager->GetDownloadLimit() / KbInt);
		}
	}
}

void CuteTorrentMainWindow::UpdateTabWidget()
{
	if (this->isMinimized())
	{
		return;
	}

	int tabIndex = m_pTabWidget->currentIndex();

	switch (tabIndex)
	{
		case 0:
		{
			UpdateInfoTab();
			break;
		}

		case 1:
		{
			UpdatePeerTab();
			break;
		}

		case 2:
		{
			UpadteTrackerTab();
			break;
		}

		case 3:
		{
			UpdateFileTab();
			break;
		}

		default:
			break;
	}
}

void CuteTorrentMainWindow::UpdateFileTab()
{
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if (tor != NULL)
	{
		if (m_pFileViewModel->setDataSource(tor->GetInternalHandle()))
		{
			fileTableView->expandToDepth(0);
		}
	}
	else
	{
		m_pFileViewModel->setDataSource(torrent_handle());
	}
}

void CuteTorrentMainWindow::setupTray()
{
	createActions();
	createTrayIcon();
	m_pTrayIcon->setToolTip(QString("CuteTorrent %1").arg(Version::getVersionStr()));
	m_pTrayIcon->show();
}

void CuteTorrentMainWindow::changeEvent(QEvent* event)
{
	if (event->type() == QEvent::ActivationChange)
	{
		if (!isVisible() && isMinimized())
		{
			setUpdatesEnabled(false);
		}
		else
		{
			setUpdatesEnabled(true);
		}
	}

	if (event->type() == QEvent::LanguageChange)
	{
		retranslateUi(this);

		if (m_initFinished)
		{
			minimizeAction->setText(tr("ACTION_HIDE"));
			maximizeAction->setText(tr("ACTION_MAXIMIZE_FULLSCREEN"));
			restoreAction->setText(tr("ACTION_MAXIMIZE"));
			quitAction->setText(tr("ACTION_EXIT"));
			copyContext->setText(tr("ACTION_COPY"));
			downLabel->setToolTip(tr("STATUS_DWONLOAD"));
			downLabelText->setToolTip(tr("STATUS_DWONLOAD"));
			upLabel->setToolTip(tr("STATUS_UPLOAD"));
			upLabelText->setToolTip(tr("STATUS_UPLOAD"));
			uploadLimit->setText(tr("LIMIT_UL"));
			downloadLimit->setText(tr("LIMIT_DL"));
			addTracker->setText(tr("ADD_TRACKER"));
			removeTracker->setText(tr("REMOVE_TRACKER"));
			editTracker->setText(tr("EDIT_TRACKER"));
			addPeer->setText(tr("ADD_PEER"));
			ul->setSpecialValueText(tr("None"));
			dl->setSpecialValueText(tr("None"));
			m_pFileViewModel->retranslateUI();
			m_pTorrentDisplayModel->retranslate();
			m_pRssDisplayModel->retranslate();
			m_pFiltersViewModel->Retranslate();
			QMap<ISerachProvider::SearchCategories, QString> categoriesToStr;
			categoriesToStr[ISerachProvider::Anime] = tr("ANIME_CATEGORY");
			categoriesToStr[ISerachProvider::Music] = tr("MUSIC_CATEGORY");
			categoriesToStr[ISerachProvider::TV] = tr("TV_CATEGORY");
			categoriesToStr[ISerachProvider::Porn] = tr("PORN_CATEGORY");
			categoriesToStr[ISerachProvider::Software] = tr("APP_CATEGORY");
			categoriesToStr[ISerachProvider::Games] = tr("GAMES_CATEGORY");
			categoriesToStr[ISerachProvider::Books] = tr("BOOKS_CATEGORY");
			categoriesToStr[ISerachProvider::Movie] = tr("FILMS_CATEGORY");;
			categoriesToStr[ISerachProvider::All] = tr("ALL_CATEGORY");
			int prevSearchCat = m_pSearchCategory->currentIndex();
			m_pSearchEdit->setPlaceholderText(tr("Search"));
			m_pTorrentSearchEdit->setPlaceholderText(tr("Search"));
#ifdef Q_WS_WIN
			setupJumpList();
#endif
			m_pSearchCategory->clear();

			for (int i = categoriesToStr.size() - 1; i >= 0; i--)
			{
				m_pSearchCategory->addItem(categoriesToStr.values().at(i), categoriesToStr.keys().at(i));
			}

			m_pSearchCategory->setCurrentIndex(prevSearchCat);
		}
	}

	QWidget::changeEvent(event);
}

void CuteTorrentMainWindow::IconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
		case QSystemTrayIcon::DoubleClick:
			if (isVisible())
			{
				hide();
			}
			else
			{
				showNormal();
				QApplication::alert(this);
				RaiseWindow();
			}

			break;

		default:
			break;
	}
}

void CuteTorrentMainWindow::createTrayIcon()
{
	trayIconMenu = new QMenu(this);
	trayIconMenu->addAction(minimizeAction);
	trayIconMenu->addAction(maximizeAction);
	trayIconMenu->addAction(restoreAction);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(quitAction);
	m_pTrayIcon = new QSystemTrayIcon(this);
	m_pTrayIcon->setIcon(QIcon(QString::fromUtf8(":/icons/app.ico")));
	m_pTrayIcon->setContextMenu(trayIconMenu);
}

void CuteTorrentMainWindow::createActions()
{
	minimizeAction = new QAction(m_pStyleEngine->getIcon("app_min"), tr("ACTION_HIDE"), this);
	minimizeAction->setObjectName("ACTION_TRAY_MINIMIZE");
	connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
	maximizeAction = new QAction(m_pStyleEngine->getIcon("app_max"), tr("ACTION_MAXIMIZE_FULLSCREEN"), this);
	maximizeAction->setObjectName("ACTION_TRAY_MAXIMIZE");
	connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));
	restoreAction = new QAction(m_pStyleEngine->getIcon("app_max"), tr("ACTION_MAXIMIZE"), this);
	restoreAction->setObjectName("ACTION_TRAY_RESTORE");
	connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
	quitAction = new QAction(m_pStyleEngine->getIcon("app_close"), tr("ACTION_EXIT"), this);
	quitAction->setObjectName("ACTION_TRAY_EXIT");
	connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
	copyContext = new QAction(m_pStyleEngine->getIcon("copy"), tr("ACTION_COPY"), describtionLabel);
	copyContext->setObjectName("ACTION_OTHER_COPY_DISCRIBTION");
	connect(copyContext, SIGNAL(triggered()), this, SLOT(CopyDiscribtion()));
	describtionLabel->addAction(copyContext);
	copyInfoHash = new QAction(m_pStyleEngine->getIcon("copy"), tr("ACTION_COPY"), infoHashLabel);
	copyInfoHash->setObjectName("ACTION_OTHER_COPY_INFOHASH");
	connect(copyInfoHash, SIGNAL(triggered()), this, SLOT(CopyInfoHash()));
	infoHashLabel->addAction(copyInfoHash);
}

void CuteTorrentMainWindow::ConnectMessageReceved(Application* a)
{
	connect(a, SIGNAL(messageReceived(const QString&)), this, SLOT(OnMessageRecived(const QString&)));
}

void CuteTorrentMainWindow::RaiseWindow()
{
	// This hack does not give the focus to the app but brings it to front so
	// the user sees it.
#ifdef Q_WS_WIN
	SetWindowPos(effectiveWinId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	SetWindowPos(effectiveWinId(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
#endif
	// HACK END
	showNormal();
	setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
	raise();
	activateWindow();
}

void CuteTorrentMainWindow::HandleNewTorrent(const QString& path)
{
	RaiseWindow();
	OpenTorrentDialog dlg(this);
	dlg.SetData(path);
	QApplication::alert(&dlg);

	if (dlg.exec() == QDialog::Accepted)
	{
#ifdef Q_WS_WIN
		QWinJumpListCategory* recent = m_pJumpList->recent();
		recent->addDestination(path);
		recent->setVisible(true);
#endif
	}
}

void CuteTorrentMainWindow::ShowCreateTorrentDialog(QString path)
{
	RaiseWindow();
	CreateTorrentDialog dlg;
	dlg.setPath(path);
	QApplication::alert(&dlg);
	dlg.exec();
}

void CuteTorrentMainWindow::ShowOpenTorrentDialog()
{
	QString lastDir = m_pSettings->valueString("System", "LastOpenTorrentDir", "");
	QString filename = QFileDialog::getOpenFileName(this, tr("OPEN_TOORENT_DIALOG"),
	                   lastDir, tr("TORRENT_FILES (*.torrent);;Any File (*.*)"));

	if (!filename.isEmpty())
	{
		m_pSettings->setValue("System", "LastOpenTorrentDir", filename);
		HandleNewTorrent(filename);
	}
}

void CuteTorrentMainWindow::UpdateInfoTab()
{
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if (tor != NULL)
	{
		fillPieceDisplay(m_pPieceView->size());
		downloadedBytesLabel->setText(tor->GetTotalDownloadedStr());
		uploadedBytesLabel->setText(tor->GetTotalUploadedStr());
		downloadSpeedLabel->setText(tor->GetDwonloadSpeed());
		activetimeLabel->setText(tor->GetActiveTimeStr());
		uploadSpeedLabel->setText(tor->GetUploadSpeed());
		pathLabel->setText(tor->GetSavePath());
		totalSizeLabel->setText(tor->GetTotalSizeStr());
		seedCoutLabel->setText(tor->GetSeedString());
		peerCoutLabel->setText(tor->GetPeerString());
		infoHashLabel->setText(tor->GetInfoHash().toUpper());
		QFontMetrics metrics(describtionLabel->font());
		QString discribtion = tor->GetDiscribtion();
		discribtion = metrics.elidedText(discribtion, Qt::ElideRight, describtionLabel->width());
		int matchedIndex = m_httpLinkRegexp.indexIn(discribtion);

		if (matchedIndex > -1)
		{
			while (matchedIndex > -1)
			{
				int matchedLength = m_httpLinkRegexp.matchedLength();
				QString match = discribtion.mid(matchedIndex, matchedLength);
				QString replacement = QString("<a href='%1' >%1</a>").arg(match);
				discribtion = discribtion.replace(matchedIndex, matchedLength, replacement);
				int offset = matchedIndex + replacement.length();
				matchedIndex = m_httpLinkRegexp.indexIn(discribtion, offset);
			}
		}

		std::vector<int> pieceAvalibility;
		tor->GetPieceAvalibility(pieceAvalibility);

		if (m_pieceAvalibilityWidget != NULL)
		{
			m_pieceAvalibilityWidget->setPiceAvailability(pieceAvalibility);
		}

		progressLabel->setText(QString("%1 %2").arg(QString::number(tor->GetProgress(), 'f', 2), "%"));
		availibilityLabel->setText(QString::number(std::abs(tor->GetDistributedCopies()), 'f', 2));
		describtionLabel->setText(discribtion);
		timeleftLabel->setText(tor->GetRemainingTimeStr());
	}
	else
	{
		ClearPieceDisplay();
		downloadedBytesLabel->setText("");
		downloadSpeedLabel->setText("");
		uploadedBytesLabel->setText("");
		activetimeLabel->setText("");
		timeleftLabel->setText("");
		uploadSpeedLabel->setText("");
		pathLabel->setText("");
		totalSizeLabel->setText("");
		seedCoutLabel->setText("");
		peerCoutLabel->setText("");
		describtionLabel->setText("");
		progressLabel->setText("");
		availibilityLabel->setText("");
		infoHashLabel->setText("");
	}
}

void CuteTorrentMainWindow::UpdatePeerTab()
{
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if (m_pPeerTableModel != NULL)
	{
		if (tor != NULL)
		{
			std::vector<peer_info> peerInfos = tor->GetPeerInfo();
			m_pPeerTableModel->UpdateData(peerInfos);
			//peerTableView->update();
		}
		else
		{
			m_pPeerTableModel->Clear();
		}
	}
}

void CuteTorrentMainWindow::UpadteTrackerTab()
{
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if (tor != NULL)
	{
		std::vector<announce_entry> trackers = tor->GetTrackerInfo();
		QTableWidgetItem* dhtItem = new QTableWidgetItem("DHT");
		dhtItem->setData(Qt::ForegroundRole, QColor("grey"));
		QTableWidgetItem* pexItem = new QTableWidgetItem("PEx");
		pexItem->setData(Qt::ForegroundRole, QColor("grey"));
		QTableWidgetItem* lsdItem = new QTableWidgetItem("LSD");
		lsdItem->setData(Qt::ForegroundRole, QColor("grey"));
		trackerTableWidget->setRowCount(trackers.size() + 3);
		trackerTableWidget->setItem(0, 0, dhtItem);
		trackerTableWidget->setItem(1, 0, pexItem);
		trackerTableWidget->setItem(2, 0, lsdItem);
		std::vector<peer_info> peerInfos = tor->GetPeerInfo();
		int nb_dht = 0, nb_pex = 0, nb_lsd = 0;

		for (int i = 0; i < peerInfos.size(); i++)
		{
			if (peerInfos[i].source & peer_info::dht)
			{
				nb_dht++;
			}

			if (peerInfos[i].source & peer_info::pex)
			{
				nb_pex++;
			}

			if (peerInfos[i].source & peer_info::lsd)
			{
				nb_lsd++;
			}
		}

		trackerTableWidget->setItem(0, 3, new QTableWidgetItem((nb_dht > 0 ? QString::number(nb_dht) : "")));
		trackerTableWidget->setItem(1, 3, new QTableWidgetItem((nb_pex > 0 ? QString::number(nb_pex) : "")));
		trackerTableWidget->setItem(2, 3, new QTableWidgetItem((nb_lsd > 0 ? QString::number(nb_lsd) : "")));
		FaviconDownloaderPtr pFaviconDownloader = FaviconDownloader::getInstance();

		for (size_type i = 0; i < trackers.size(); i++)
		{
			QString url = QString::fromUtf8(trackers[i].url.c_str());
			QIcon favIcon = pFaviconDownloader->getFavicon(url);
			QTableWidgetItem* rootItem = new QTableWidgetItem(favIcon, url);
			trackerTableWidget->setItem(i + 3, 0, rootItem);
			QString stateStr;

			if (trackers[i].last_error && trackers[i].last_error.value() != errors::tracker_failure)
			{
				stateStr = StaticHelpers::translateLibTorrentError(trackers[i].last_error);
			}
			else if (!trackers[i].message.empty())
			{
				stateStr = QString::fromUtf8(trackers[i].message.c_str());
			}
			else if (trackers[i].updating)
			{
				stateStr = tr("TRACKER_UPDATING");
			}
			else
			{
				stateStr = "Ok";
			}

			trackerTableWidget->setItem(i + 3, 1, new QTableWidgetItem(stateStr));
			trackerTableWidget->setItem(i + 3, 2, new QTableWidgetItem(StaticHelpers::toTimeString(trackers[i].next_announce_in())));
			int peer_cnt = tor->GetPeerCountForTracker(QString::fromStdString(trackers[i].url));
			trackerTableWidget->setItem(i + 3, 3, new QTableWidgetItem((peer_cnt > 0 ? QString::number(peer_cnt) : "")));
		}
	}
	else
	{
		trackerTableWidget->setRowCount(0);
	}
}

void CuteTorrentMainWindow::PauseSelected()
{
	m_pTorrentDisplayModel->ActionOnSelectedItem(QTorrentDisplayModel::pause);
}

void CuteTorrentMainWindow::ResumeSelected()
{
	m_pTorrentDisplayModel->ActionOnSelectedItem(QTorrentDisplayModel::resume);
}

void CuteTorrentMainWindow::DeleteSelected()
{
	m_pTorrentDisplayModel->ActionOnSelectedItem(QTorrentDisplayModel::remove);
}

void CuteTorrentMainWindow::Retranslate()
{
	retranslateUi(this);
}

void CuteTorrentMainWindow::OpenSettingsDialog()
{
	boost::shared_ptr<SettingsDialog> dlg(new SettingsDialog(this));
	connect(dlg.get(), SIGNAL(needRetranslate()), this, SLOT(Retranslate()));
	dlg->exec();
	m_pGroupTreeView->expandToDepth(0);
	setupKeyMappings();
	UpdateTabWidget();
}

void CuteTorrentMainWindow::closeEvent(QCloseEvent* ce)
{
	ce->ignore();
	hide();
	m_pNotificationSystem->OnNewNotification(NotificationSystem::TORRENT_INFO, tr("CT_HIDE_MSG"), QVariant());
	setUpdatesEnabled(false);
}

void CuteTorrentMainWindow::dragEnterEvent(QDragEnterEvent* event)
{
	if (event->mimeData()->hasFormat(QString("text/plain")) || event->mimeData()->hasFormat(QString("text/uri-list")))
	{
		event->acceptProposedAction();
	}
}

void CuteTorrentMainWindow::dropEvent(QDropEvent* event)
{
	QStringList files;

	if (event->mimeData()->hasUrls())
	{
		const QList<QUrl> urls = event->mimeData()->urls();

		foreach(const QUrl& url, urls)
		{
			if (!url.isEmpty())
			{
				if (url.scheme().compare("file", Qt::CaseInsensitive) == 0)
				{
					files << url.toLocalFile();
				}
				else
				{
					files << url.toString();
				}
			}
		}
	}
	else
	{
		files = event->mimeData()->text().split(QString::fromUtf8("\n"));
	}

	foreach(QString file, files)
	{
		if (file.startsWith("magnet:", Qt::CaseInsensitive) || file.endsWith(".torrent", Qt::CaseInsensitive))
		{
			HandleNewTorrent(file);
			event->acceptProposedAction();
		}
		else
		{
			CustomMessageBox::warning(this, tr("UNABLE_TO_PROCESS_FILE"), tr("DROPPED_FILE %1 IS_NOT_TORRENT_OR_MAGNET_LINK").arg(file));
		}
	}
}

void CuteTorrentMainWindow::saveWindowState()
{
	m_pUpdateTimer->stop();
	m_pSettings->setValue("Window", "geometry", geometry());
	m_pSettings->setValue("Window", "preMaximizeGeometry", preMaximizeGeomentry);
	m_pSettings->setValue("Window", "maximized", isMaximized());
	m_pSettings->setValue("Window", "selected_tab", m_pTabWidget->currentIndex());
	m_pSettings->setValue("Window", "horizontal_sizes", QVariant::fromValue(spliiter1->sizes()));
	m_pSettings->setValue("Window", "vertical_sizes", QVariant::fromValue(spliiter->sizes()));
	saveHeaderState("peers", m_pPeersHeader);
	saveHeaderState("files", m_pFilesHeader);
	saveHeaderState("trackers", m_pTrackersHeader);

	if (m_lastSorter == TORRENT)
	{
		saveHeaderState("torrent_sorter", m_pItemSorterView);
	}
	else if (m_lastSorter == SEARCH)
	{
		saveHeaderState("search_sorter", m_pItemSorterView);
	}

	m_pSettings->setValue("Window", "torrent_sorter_sort_role", QVariant::fromValue(m_pTorrentFilterProxyModel->sortRole()));
	m_pSettings->setValue("Window", "search_sorter_sort_role", QVariant::fromValue(m_pSearchFilterModel->sortRole()));
}

void CuteTorrentMainWindow::loadHeaderState(QString prefix, EditableHeaderView* header, QList<int>& defaultColumnSizes)
{
	QList<int> columnSizes = m_pSettings->value("Window", prefix % "_sizes", QVariant::fromValue(defaultColumnSizes)).value<QList<int> >();
	header->setStretchLastSection(true);
	int columnCount = columnSizes.count();
	Q_ASSERT_X(defaultColumnSizes.size() == columnCount, "CuteTorrentMainWindow::loadHeaderState", "Inconsistent default sizes given");

	if (columnCount > 0)
	{
		for (int i = 0; i < columnCount; i++)
		{
			if (columnSizes[i] == 0)
			{
				header->resizeSection(i, defaultColumnSizes[i]);
				header->setSectionHidden(i, true);
			}
			else
			{
				header->resizeSection(i, columnSizes[i]);
			}
		}

		//header->setResizeMode(columnCount - 1, QHeaderView::Stretch);
	}

	int sortColumn = m_pSettings->valueInt("Window", prefix % "_sort_column", 0);
	Qt::SortOrder sortOrder = Qt::SortOrder(m_pSettings->valueInt("Window", prefix % "_sort_order", Qt::AscendingOrder));
	header->setSortIndicator(sortColumn, sortOrder);
}

void CuteTorrentMainWindow::saveHeaderState(QString prefix, EditableHeaderView* header)
{
	QList<int> columnSizes;
	int columnCount = header->model()->columnCount();
	columnSizes.reserve(columnCount);

	for (int i = 0; i < columnCount; i++)
	{
		columnSizes.append(header->sectionSize(i));
	}

	m_pSettings->setValue("Window", prefix % "_sizes", QVariant::fromValue(columnSizes));
	m_pSettings->setValue("Window", prefix % "_sort_column", header->sortIndicatorSection());
	m_pSettings->setValue("Window", prefix % "_sort_order", header->sortIndicatorOrder());
}

CuteTorrentMainWindow::~CuteTorrentMainWindow()
{
	m_pTrayIcon->hide();
	Scheduller::freeInstance();
}

void CuteTorrentMainWindow::setupFileTabel()
{
	m_pFileViewProxymodel = new FileViewSortProxyModel(this);
	fileTableView->setModel(m_pFileViewProxymodel);
	m_pFilesHeader = new EditableHeaderView(Qt::Horizontal, fileTableView);
	m_pFileViewModel = new FileViewModel(fileTableView, this);
	m_pFileViewProxymodel->setSourceModel(m_pFileViewModel);
	fileTableView->setItemDelegateForColumn(1, new FileSizeItemDelegate(false, this));
	fileTableView->setItemDelegateForColumn(2, new ProgressItemDelegate(this));
	fileTableView->setItemDelegateForColumn(3, new PriorityItemDelegate(this));
	QList<int> defaultFileSectionSizes = QList<int>() << 450 << 65 << 65 << 70;
	fileTableView->setHeader(m_pFilesHeader);
	loadHeaderState("files", m_pFilesHeader, defaultFileSectionSizes);
	fileTableView->header()->setDefaultAlignment(Qt::AlignCenter);
	fileTableView->header()->setClickable(true);
}


void CuteTorrentMainWindow::ProcessMagnet()
{
	bool ok;
	const QClipboard *clipboard = QApplication::clipboard();
	const QMimeData *mimeData = clipboard->mimeData();
	QString clipboardMagentLink;
	if (mimeData->hasText())
	{
		clipboardMagentLink = mimeData->text();
	}
	QString magnetLink = QInputDialog::getText(this, tr("MAGNET_LINK_DLG"), tr("MAGNET_LINK:"), QLineEdit::Normal, clipboardMagentLink, &ok);

	if (ok && !magnetLink.isEmpty())
	{
		OpenTorrentDialog dialog(this);
		dialog.SetData(magnetLink);
		dialog.exec();
	}
}

void CuteTorrentMainWindow::PeformSearch()
{
	QString searchText = m_pSearchEdit->text().trimmed();

	if (!searchText.isEmpty())
	{
		ISerachProvider::SearchCategories category = static_cast<ISerachProvider::SearchCategories>(m_pSearchCategory->itemData(m_pSearchCategory->currentIndex()).toInt());
		m_pSearchEngine->DoSerach(searchText, category, 1);
		QModelIndex searchItemIndex = m_pFiltersViewModel->index(3, 0, QModelIndex());
		m_pGroupTreeView->selectionModel()->setCurrentIndex(searchItemIndex, QItemSelectionModel::Current);
	}
}

void CuteTorrentMainWindow::PeformTorrentSearch(const QString& text)
{
	m_pTorrentFilterProxyModel->setTorrentSearchFilter(text);
}

void CuteTorrentMainWindow::resizeEvent(QResizeEvent* /*event*/)
{
	if (m_pPieceView != NULL)
	{
		fillPieceDisplay(m_pPieceView->size());
	}
}

void CuteTorrentMainWindow::keyPressEvent(QKeyEvent* event)
{
	if (!isVisible())
	{
		QWidget::keyPressEvent(event);
		return;
	}

	QKeySequence pressedKey = QKeySequence((event->key() == Qt::Key_Return ? Qt::Key_Enter : event->key()) | event->modifiers());
	QVariantMap keyMap = m_pSettings->getGroupValues("KeyMap");
	QStringList keys = keyMap.keys();

	/* qDebug() << pressedKey;
	 qDebug() << keyMap;*/
	foreach(QString key, keys)
	{
		if (QKeySequence(keyMap[key].toString()) == pressedKey)
		{
			QAction* action = this->findChild<QAction*>(key);

			if (action != NULL)
			{
				//qDebug() << "Matched action:" << pressedKey << action->objectName();
				action->activate(QAction::Trigger);
			}
		}
	}

	QWidget::keyPressEvent(event);
}

void CuteTorrentMainWindow::fillPieceDisplay(QSize size)
{
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if (tor != NULL)
	{
		int piece_count = tor->GetPieceCount();
		QBitArray avaliablePieces = tor->GetDownloadedPieces();
		QBitArray dwonloadingPieces = tor->GetDownloadingPieces();
		m_pPieceView->resize(size);
		m_pPieceView->setProgress(avaliablePieces, dwonloadingPieces);
	}
}

void CuteTorrentMainWindow::CopyDiscribtion()
{
	QClipboard* clipboard = QApplication::clipboard();
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if (tor != NULL)
	{
		clipboard->setText(tor->GetDiscribtion());
	}
}

void CuteTorrentMainWindow::CopyInfoHash()
{
	QClipboard* clipboard = QApplication::clipboard();
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if (tor != NULL)
	{
		clipboard->setText(tor->GetInfoHash().toUpper());
	}
}

void CuteTorrentMainWindow::ClearPieceDisplay()
{
	m_pPieceView->clear();
	m_pieceAvalibilityWidget->clear();
}

void CuteTorrentMainWindow::UpdateUL(int kbps)
{
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if (tor != NULL)
	{
		tor->SetUlLimit(kbps * KbInt);
	}
	else
	{
		m_pSettings->setValue("Torrent", "upload_rate_limit", kbps * KbInt);
		m_pTorrentManager->SetUlLimit(kbps * KbInt);
	}
}

void CuteTorrentMainWindow::UpdateDL(int kbps)
{
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if (tor != NULL)
	{
		tor->SetDlLimit(kbps * KbInt);
	}
	else
	{
		m_pSettings->setValue("Torrent", "download_rate_limit", kbps * KbInt);
		m_pTorrentManager->SetDlLimit(kbps * KbInt);
	}
}

void CuteTorrentMainWindow::AddPeer()
{
	Torrent* torrent = m_pTorrentDisplayModel->GetSelectedTorrent();

	if (torrent != NULL)
	{
		bool ok;
		QString peerStr = QInputDialog::getText(this, tr("ADD_PEER_DLG"), tr("PEER:"), QLineEdit::Normal, "", &ok);

		if (ok && !peerStr.isEmpty())
		{
			QStringList parts = peerStr.split(':');

			if (parts.count() == 2)
			{
				QHostAddress adr(parts[0]);

				if (!adr.isNull())
				{
					torrent->AddPeer(adr, parts[1].toUInt());
					return;
				}
			}

			CustomMessageBox::critical(this, tr("PEER_ERR"), tr("INVALID_IP_STRING"));
		}
	}
}

void CuteTorrentMainWindow::AddTracker()
{
	Torrent* torrent = m_pTorrentDisplayModel->GetSelectedTorrent();

	if (torrent != NULL)
	{
		bool ok;
		QString trackerUrl = QInputDialog::getText(this, tr("ADD_TRACKER_DLG"), tr("TRACKER:"), QLineEdit::Normal, "", &ok);

		if (ok && !trackerUrl.isEmpty())
		{
			torrent->AddTracker(trackerUrl);
		}
	}
}

void CuteTorrentMainWindow::StopSelected()
{
	m_pTorrentDisplayModel->ActionOnSelectedItem(QTorrentDisplayModel::stop);
}

void CuteTorrentMainWindow::setupGroupTreeWidget()
{
	m_pGroupTreeView->setModel(m_pFiltersViewModel);
	m_pGroupTreeView->resizeColumnToContents(0);
	m_pGroupTreeView->expandToDepth(0);
}
#ifdef Q_WS_WIN
void CuteTorrentMainWindow::setupTasksCategory()
{
	QWinJumpListCategory* tasks = m_pJumpList->tasks();
	tasks->clear();
	QStringList createTorrentArgs;
	createTorrentArgs << "--create_torrent";
	tasks->addLink(m_pStyleEngine->getIcon("create_torrent"), QApplication::translate("CustomWindow", "MENU_CREATE_TORRENT"), QApplication::applicationFilePath(), createTorrentArgs);
	QStringList settingsArgs;
	settingsArgs << "--settings";
	tasks->addLink(m_pStyleEngine->getIcon("menu_settings"), QApplication::translate("CustomWindow", "MENU_CONFIGURATION"), QApplication::applicationFilePath(), settingsArgs);
	tasks->setVisible(true);
}

void CuteTorrentMainWindow::setupJumpList()
{
	setupTasksCategory();
	QWinJumpListCategory* recent = m_pJumpList->recent();
	recent->setVisible(true);
}
#endif
void CuteTorrentMainWindow::ChnageTorrentFilter()
{
	QModelIndex current = m_pGroupTreeView->currentIndex();
	FilterType filterType = static_cast<FilterType>(current.data(FiltersViewModel::FilterTypeRole).toInt());
	m_pTorrentListView->selectionModel()->reset();

	switch (filterType)
	{
		case GROUP_FILTER_TYPE:
		{
			QUuid gropupFilter = current.data(FiltersViewModel::FilterRole).value<QUuid>();
			m_pTorrentFilterProxyModel->setGroupFilter(gropupFilter);
			switchToTorrentsModel();
			break;
		}

		case FilterType::SEARCH:
		{
			QString engineName = current.data(FiltersViewModel::FilterRole).toString();
			m_pSearchFilterModel->setSearchFilter(engineName);
			switchToSearchModel();
			break;
		}

		case RSS_FEED:
		{
			switchToRssModel();
			break;
		}

		case FilterType::TORRENT:
		{
			TorrentFilterType torrentFilter = static_cast<TorrentFilterType>(current.data(FiltersViewModel::FilterRole).toInt());
			m_pTorrentFilterProxyModel->setTorrentFilter(torrentFilter);
			switchToTorrentsModel();
			break;
		}

		case NONE:
			break;
	}

	//	m_pTorrentDisplayModel->updateVisibleTorrents();
}

void CuteTorrentMainWindow::startBackUpWizard()
{
	BackupWizard* wizard = new BackupWizard(this);
	wizard->show();
}

void CuteTorrentMainWindow::initMainMenuIcons()
{
	ACTION_MENU_ABAUT_CT->setIcon(m_pStyleEngine->getIcon("about"));
	ACTION_MENU_CHECK_UPDATE->setIcon(m_pStyleEngine->getIcon("check_update"));
	ACTION_MENU_CREATE->setIcon(m_pStyleEngine->getIcon("create_torrent"));
	ACTION_MENU_EXIT->setIcon(m_pStyleEngine->getIcon("menu_exit"));
	ACTION_MENU_OPEN_MAGNET->setIcon(m_pStyleEngine->getIcon("magnet"));
	ACTION_MENU_OPEN_TORRENT->setIcon(m_pStyleEngine->getIcon("add_torrent"));
	ACTION_MENU_SETTINGS->setIcon(m_pStyleEngine->getIcon("menu_settings"));
	ACTION_MENU_BACKUP->setIcon(m_pStyleEngine->getIcon("menu_backup"));
}

void CuteTorrentMainWindow::initWindowIcons()
{
	BaseWindow::setupWindowIcons();
	m_pFiltersViewModel->UpdateIcons();
	initToolbarIcons();
	initStatusBarIcons();
	initMainMenuIcons();
}

void CuteTorrentMainWindow::setupCustomeWindow()
{
	setupUi(this);
	BaseWindow::setupCustomWindow();
	QRect geo = m_pSettings->value("Window", "geometry", QRect(0, 0, 683, 643)).toRect();

	if (m_pSettings->value("Window", "maximized", false).toBool())
	{
		m_bIsMaximized = true;
		pbMax->setIcon(m_pStyleEngine->getIcon("app_reset"));
	}

	preMaximizeGeomentry = m_pSettings->value("Window", "preMaximizeGeometry", QRect(0, 0, 683, 643)).toRect();

	if (geo.height() > 0 && geo.x() < QApplication::desktop()->width() && geo.width() > 0 && geo.y() < QApplication::desktop()->height())
	{
		setGeometry(geo);
	}

	int selectedTab = m_pSettings->valueInt("Window", "selected_tab", 0);

	if (selectedTab >= 0 && selectedTab < m_pTabWidget->count())
	{
		m_pTabWidget->setCurrentIndex(selectedTab);
	}

	QList<int> horizontal_sizes = m_pSettings->value("Window", "horizontal_sizes", qVariantFromValue(QList<int>() << 130 << 538)).value<QList<int> >();

	if (horizontal_sizes.size() > 0)
	{
		spliiter1->setSizes(horizontal_sizes);
	}

	QList<int> vertical_sizes = m_pSettings->value("Window", "vertical_sizes", qVariantFromValue(QList<int>() << 530 << 195)).value<QList<int> >();

	if (vertical_sizes.size() > 0)
	{
		spliiter->setSizes(vertical_sizes);
	}

	QList<QWidget*> children = findChildren<QWidget*>();

	for (int i = 0; i < children.size(); i++)
	{
		QWidget* child = children[i];
		child->installEventFilter(this);
	}
}

void CuteTorrentMainWindow::setupKeyMappings()
{
	QList<QAction*> actions = this->findChildren<QAction*>();
	QMap<QString, QVariant> keyMap = m_pSettings->getGroupValues("KeyMap");

	foreach(QAction* action, actions)
	{
		QString objName = action->objectName();

		if (!objName.isEmpty() && objName.startsWith("ACTION_") && !keyMap.contains(objName))
		{
			keyMap.insert(action->objectName(), action->shortcut().toString());
		}
		else
		{
			if (objName.startsWith("ACTION_") && keyMap.contains(objName))
			{
				action->setShortcut(QKeySequence(keyMap[objName].toString()));
			}
		}
	}

	m_pSettings->setGroupValues("KeyMap", keyMap);
	//qDebug() << keyMap;
}


void CuteTorrentMainWindow::resizeWindow(QMouseEvent* e)
{
	BaseWindow::resizeWindow(e);
	m_pPieceView->update();
}

QPushButton* CuteTorrentMainWindow::getMinBtn()
{
	return pbMin;
}

QPushButton* CuteTorrentMainWindow::getMaxBtn()
{
	return pbMax;
}

QPushButton* CuteTorrentMainWindow::getCloseBtn()
{
	return pbClose;
}

QWidget* CuteTorrentMainWindow::getTitleBar()
{
	return titleBar;
}

QWidget* CuteTorrentMainWindow::centralWidget()
{
	return m_centralWidget;
}

QLabel* CuteTorrentMainWindow::getTitleLabel()
{
	return LTitle;
}

QLabel* CuteTorrentMainWindow::getTitleIcon()
{
	return tbMenu;
}

void CuteTorrentMainWindow::queueTorrentsUp()
{
	m_pTorrentDisplayModel->ActionOnSelectedItem(QTorrentDisplayModel::queue_up);
}

void CuteTorrentMainWindow::queueTorrentsDown()
{
	m_pTorrentDisplayModel->ActionOnSelectedItem(QTorrentDisplayModel::queue_down);
}

void CuteTorrentMainWindow::updateSorting(int logincalIndex, Qt::SortOrder order)
{
	if (m_pTorrentListView->model() == m_pTorrentFilterProxyModel)
	{
		QAbstractItemModel* headerModel = m_pItemSorterView->model();
		QTorrentDisplayModel::Role sorter = QTorrentDisplayModel::Role(headerModel->headerData(logincalIndex, Qt::Horizontal, Qt::UserRole).toInt());
		m_pTorrentFilterProxyModel->setSortRole(sorter);

		if (m_pTorrentFilterProxyModel->sortOrder() != order)
		{
			m_pTorrentFilterProxyModel->sort(0, order);
		}
	}
	else if (m_pTorrentListView->model() == m_pSearchFilterModel)
	{
		QAbstractItemModel* headerModel = m_pItemSorterView->model();
		QSearchDisplayModel::Role sorter = QSearchDisplayModel::Role(headerModel->headerData(logincalIndex, Qt::Horizontal, Qt::UserRole).toInt());
		m_pSearchFilterModel->setSortRole(sorter);

		if (m_pSearchFilterModel->sortOrder() != order)
		{
			m_pSearchFilterModel->sort(0, order);
		}
	}
}

void CuteTorrentMainWindow::maximizeBtnClicked()
{
	BaseWindow::maximizeBtnClicked();
}

void CuteTorrentMainWindow::minimizeBtnClicked()
{
	BaseWindow::minimizeBtnClicked();
}

bool CuteTorrentMainWindow::eventFilter(QObject* obj, QEvent* event)
{
	switch (event->type())
	{
		case QEvent::MouseButtonDblClick:
		{
			mouseMoveEvent((static_cast<QMouseEvent*>(event)));
			break;
		}

		case QEvent::MouseButtonRelease:
		{
			mouseMoveEvent((static_cast<QMouseEvent*>(event)));
			break;
		}

		case QEvent::MouseButtonPress:
		{
			mouseMoveEvent((static_cast<QMouseEvent*>(event)));
			break;
		}

		case QEvent::MouseMove:
		{
			mouseMoveEvent((static_cast<QMouseEvent*>(event)));
			break;
		}

		case QEvent::Resize:
		{
			resizeEvent(static_cast<QResizeEvent*>(event));
			break;
		}
	}

	return QObject::eventFilter(obj, event);
}

void CuteTorrentMainWindow::switchToTorrentsModel()
{
	if (m_pTorrentListView->model() != m_pTorrentFilterProxyModel)
	{
		m_pTorrentListView->setModel(m_pTorrentFilterProxyModel);
		m_pTorrentListView->setItemDelegate(m_pTorrentItemDelegate);
		m_pToolBarsContainer->setCurrentIndex(0);
		m_pInfoPlaneContainer->setCurrentIndex(0);
		connect(m_pTorrentListView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
		        m_pTorrentDisplayModel, SLOT(UpdateSelectedIndex(const QItemSelection&)));
		connect(m_pTorrentListView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
		        this, SLOT(UpdateTabWidget()));
		connect(m_pTorrentListView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
		        this, SLOT(UpdateLimits()));
		m_pTorrentListView->setRootIsDecorated(false);
		saveHeaderState("search_sorter", m_pItemSorterView);
		m_pSettings->setValue("Window", "search_sorter_sort_role", QVariant::fromValue(m_pSearchFilterModel->sortRole()));
		m_pItemSorterView->setVisible(true);
		m_pItemSorterView->setModel(m_torrentHeaderModel);
		QList<int> defaultSizes = QList<int>() << 40 << 70 << 60 << 70 << 80 << 80 << 80;
		loadHeaderState("torrent_sorter", m_pItemSorterView, defaultSizes);
		m_pTorrentFilterProxyModel->setSortRole(m_pSettings->valueInt("Window", "torrent_sorter_sort_role"));
		m_lastSorter = TORRENT;
	}

	m_pTorrentListView->selectionModel()->clear();
	m_pTorrentDisplayModel->UpdateSelectedIndex(m_pTorrentListView->selectionModel()->selection());
	UpdateTabWidget();
	UpdateLimits();
}

void CuteTorrentMainWindow::switchToSearchModel()
{
	if (m_pTorrentListView->model() != m_pSearchFilterModel)
	{
		m_pTorrentListView->setRootIsDecorated(false);
		disconnect(m_pTorrentListView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
		           m_pTorrentDisplayModel, SLOT(UpdateSelectedIndex(const QItemSelection&)));
		disconnect(m_pTorrentListView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
		           this, SLOT(UpdateTabWidget()));
		disconnect(m_pTorrentListView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
		           this, SLOT(UpdateLimits()));
		m_pTorrentListView->selectionModel()->clear();
		m_pTorrentDisplayModel->UpdateSelectedIndex(m_pTorrentListView->selectionModel()->selection());
		m_pTorrentListView->setModel(m_pSearchFilterModel);
		m_pTorrentListView->setItemDelegate(m_pSearchItemDelegate);
		m_pToolBarsContainer->setCurrentIndex(1);
		m_pInfoPlaneContainer->setCurrentIndex(0);
		UpdateTabWidget();
		saveHeaderState("torrent_sorter", m_pItemSorterView);
		m_pSettings->setValue("Window", "torrent_sorter_sort_role", QVariant::fromValue(m_pTorrentFilterProxyModel->sortRole()));
		m_pItemSorterView->setVisible(true);
		m_pItemSorterView->setModel(m_searchHeaderModel);
		QList<int> defaultSearchSize = QList<int>() << 80 << 80 << 80 << 80;
		loadHeaderState("search_sorter", m_pItemSorterView, defaultSearchSize);
		m_pSearchFilterModel->setSortRole(m_pSettings->valueInt("Window", "search_sorter_sort_role"));
		m_lastSorter = SEARCH;
	}
}

void CuteTorrentMainWindow::switchToRssModel()
{
	if (m_pTorrentListView->model() != m_pRssDisplayModel)
	{
		m_pTorrentListView->setRootIsDecorated(true);
		disconnect(m_pTorrentListView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
		           m_pTorrentDisplayModel, SLOT(UpdateSelectedIndex(const QItemSelection&)));
		disconnect(m_pTorrentListView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
		           this, SLOT(UpdateTabWidget()));
		disconnect(m_pTorrentListView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
		           this, SLOT(UpdateLimits()));
		m_pTorrentListView->setModel(m_pRssDisplayModel);
		m_pTorrentListView->setItemDelegate(m_pRssItemDelegate);
		connect(m_pTorrentListView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
		        this, SLOT(UpdateRssInfo(const QItemSelection&)));
		m_pToolBarsContainer->setCurrentIndex(2);
		m_pInfoPlaneContainer->setCurrentIndex(1);
		UpdateRssInfo(m_pTorrentListView->selectionModel()->selection());
		m_pItemSorterView->setVisible(false);
	}
}

void CuteTorrentMainWindow::openSearchItemDescribtion()
{
	if (m_pTorrentListView->model() != m_pSearchFilterModel)
	{
		return;
	}

	QModelIndex index = m_pTorrentListView->currentIndex();
	SearchResult* searchResult = index.data(QSearchDisplayModel::SearchItemRole).value<SearchResult*>();
	QDesktopServices::openUrl(searchResult->TorrentDescUrl());
}

void CuteTorrentMainWindow::startDownloadTorrent()
{
	if (m_pTorrentListView->model() != m_pSearchFilterModel)
	{
		return;
	}

	QModelIndex index = m_pTorrentListView->currentIndex();
	SearchResult* searchResult = index.data(QSearchDisplayModel::SearchItemRole).value<SearchResult*>();
	QDesktopServices::openUrl(searchResult->TorrentFileUrl());
}

void CuteTorrentMainWindow::RemoveTracker()
{
	Torrent* torrent = m_pTorrentDisplayModel->GetSelectedTorrent();

	if (torrent != NULL)
	{
		QList<QTableWidgetSelectionRange> selectedRows = trackerTableWidget->selectedRanges();

		if (selectedRows.isEmpty())
		{
			return;
		}

		QStringList trackers2remove;

		for (int j = 0 ; j < selectedRows.size(); j++)
		{
			QTableWidgetSelectionRange selection = selectedRows[j];

			for (int i = selection.topRow(); i <= selection.bottomRow(); i++)
			{
				trackers2remove.append(trackerTableWidget->item(i, 0)->text());
			}
		}

		if (trackers2remove.size() > 1)
		{
			torrent->RemoveTrackers(trackers2remove);
		}
		else
		{
			QString trackerUrl = trackers2remove.first();

			if (trackerUrl.startsWith("http", Qt::CaseInsensitive) || trackerUrl.startsWith("udp", Qt::CaseInsensitive))
			{
				torrent->RemoveTracker(trackerUrl);
			}
		}
	}
}

void CuteTorrentMainWindow::EditTracker()
{
	Torrent* torrent = m_pTorrentDisplayModel->GetSelectedTorrent();

	if (torrent != NULL)
	{
		int selectedRow = trackerTableWidget->currentRow();
		QString tracker2edit = trackerTableWidget->item(selectedRow, 0)->text();
		bool ok;
		QString newTrackerUrl = QInputDialog::getText(this, tr("EDIT_TRACKER_DLG"), tr("TRACKER:"), QLineEdit::Normal, tracker2edit, &ok);

		if (ok && !newTrackerUrl.isEmpty())
		{
			torrent->ReplaceTracker(tracker2edit, newTrackerUrl);
		}
	}
}

void CuteTorrentMainWindow::UpdateTracker()
{
	Torrent* torrent = m_pTorrentDisplayModel->GetSelectedTorrent();

	if (torrent != NULL)
	{
		int currentRow = trackerTableWidget->currentRow();

		if (currentRow > 2)
		{
			torrent->UpdateTracker(currentRow - 3);
		}
	}
}

void CuteTorrentMainWindow::AddWebSeed()
{
	Torrent* torrent = m_pTorrentDisplayModel->GetSelectedTorrent();

	if (torrent != NULL)
	{
		bool ok;
		QString webSeedUrl = QInputDialog::getText(this, tr("EDIT_TRACKER_DLG"), tr("HTTP_SEED_URL:"), QLineEdit::Normal, "", &ok);

		if (ok && !webSeedUrl.isEmpty() && webSeedUrl.startsWith("http"))
		{
			torrent->addWebSeed(webSeedUrl);
		}
	}
}

void CuteTorrentMainWindow::addRssFeed()
{
	if (m_pTorrentListView->model() == m_pRssDisplayModel)
	{
		bool ok;
		QString newFeedUrl = QInputDialog::getText(this, tr("RSS_FEED_ADD"), tr("RSS_FEED_URL:"), QLineEdit::Normal, "", &ok);

		if (ok && !newFeedUrl.isEmpty() && newFeedUrl.startsWith("http"))
		{
			RssManagerPtr pRssManager = RssManager::getInstance();
			bool isNew;
			pRssManager->addFeed(newFeedUrl, isNew);
		}
	}
}

void CuteTorrentMainWindow::removeRssFeed()
{
	if (m_pTorrentListView->model() == m_pRssDisplayModel)
	{
		m_pRssDisplayModel->onFeedRemove();
	}
}

void CuteTorrentMainWindow::editRssFeed()
{
	if (m_pTorrentListView->model() == m_pRssDisplayModel)
	{
		RssFeed* selecteFeed = m_pRssDisplayModel->SelectedFeed();

		if (selecteFeed != NULL)
		{
			boost::scoped_ptr<RssFeedSettingsDialog> pDialog(new RssFeedSettingsDialog(this));
			pDialog->SetFeed(selecteFeed);
			pDialog->exec();
		}
	}
}

void CuteTorrentMainWindow::OnQuit()
{
	if (m_pTorrentManager != NULL)
	{
		m_pTorrentManager->SaveSession();
	}
	m_pTorrentGroupsManager->SaveGroups();
	RssManagerPtr pRssManager = RssManager::getInstance();
	pRssManager->SaveFeeds();
	pRssManager->SaveDownloadRules();
	saveWindowState();
	m_pSettings->WriteSettings();
}

void CuteTorrentMainWindow::UpdateRssInfo(const QItemSelection& /*selection*/)
{
	if (m_pTorrentListView->model() != m_pRssDisplayModel)
	{
		return;
	}

	RssItem* currentItem = m_pRssDisplayModel->SelectedRssItem();

	if (currentItem != NULL)
	{
		m_pFeedItemDescribtionEdit->setHtml(currentItem->description());
	}
	else
	{
		m_pFeedItemDescribtionEdit->setText("");
	}
}

void CuteTorrentMainWindow::OnMessageRecived(QString message)
{
	int commandEndIndex = message.indexOf(':');

	if (commandEndIndex > -1)
	{
		QString command = message.mid(0, commandEndIndex);
		QString arg = message.mid(commandEndIndex + 1);
		qDebug() << "Command" << command << arg;
		QStringList supportedCommands;
		supportedCommands << "open" << "create_torrent" << "settings";

		switch (supportedCommands.indexOf(command))
		{
			case 0:
			{
				HandleNewTorrent(arg);
				break;
			}

			case 1:
			{
				ShowCreateTorrentDialog(arg);
				break;
			}

			case 2:
			{
				OpenSettingsDialog();
				break;
			}
		}
	}
	else
	{
		CustomMessageBox::information(this, "CuteTorrent", tr("INVALID_COMMAND_RECIVED"));
	}
}

void CuteTorrentMainWindow::setupRssInfoTab()
{
	m_pFeedItemDescribtionEdit = new HtmlView(this);
	m_pRssInfoLayout->addWidget(m_pFeedItemDescribtionEdit, 0, 0, 1, 1);
}
