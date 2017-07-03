/*
CuteTorrent BitTorrent Client with dht support, userfriendly interface
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
#include <WinSock2.h>
#include <Windows.h>
#include "CuteTorrentMainWindow.h"
#include <stddef.h>
#include <QAbstractItemView>
#include <QClipboard>
#include <QComboBox>
#include <QDebug>
#include <QDesktopServices>
#ifdef Q_OS_WIN 
#include <QtWinExtras/QWinTaskbarButton>
#endif
#include <QSortFilterProxyModel>
#include <QTreeWidgetItem>
#include <QUrl>
#include <QtNetwork/QHostAddress>
#include "CreateTorrentDialog.h"
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
#include <libtorrent/announce_entry.hpp>
#include "RssItem.h"
#include "StaticHelpers.h"
#include "SearchResult.h"
#include "SearchLineEdit.h"
#include "ReportProblemDialog.h"
#ifdef Q_OS_WIN 
#include "QtWinExtras/qwinjumplist.h"
#include "QtWinExtras/qwinjumplistcategory.h"
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
#include <PowerManagement.h>
#include <gui/ImportWizard/ImportWizard.h>
#include <RaitingDialog.h>
#include "Utils/ValueGetters.h"
#include "Utils/ValueSetters.h"
#include <CommentsModel.h>
#include <CommentItemDelegate.h>
#include <gui/Controls/RaitingWidget.h>
#include <gui/Dialogs/AddCommentDialog.h>
#include <CommentsWebClient.h>
#include <gui/Dialogs/LoginDialog.h>
#include <gui/Dialogs/RegisterDialog.h>
#include <SortButton.h>
#include <QtWinExtras/QtWinExtras>
#include <QList>
class Application;
class ISerachProvider;
class SearchResult;

Q_DECLARE_METATYPE(QList<int>)

CuteTorrentMainWindow::CuteTorrentMainWindow(QWidget* parent)
	: BaseWindow(FullTitle, AllowResize, parent)
	  , m_initFinished(false)
	  , m_pTaskBarBtn(new QWinTaskbarButton(this))
	  //  , m_pItemSorterView(new EditableHeaderView(Qt::Horizontal, this))
	  , m_httpLinkRegexp("(http|ftp|https):\\/\\/[\\w\\-_]+(\\.[\\w\\-_]+)+([\\w\\-\\.,@?^=%&amp;:/~\\+#]*[\\w\\-\\@?^=%&amp;/~\\+#])?")
	  , m_pPieceView(NULL)
	  , m_pNotificationSystem(NotificationSystem::getInstance())
	  , m_pieceAvalibilityWidget(NULL)
	  , m_pPeerTableModel(NULL)
	  , m_pSearchEngine(SearchEngine::getInstance())
	  , m_ignoreSortByIndexChange(false)
#ifdef Q_OS_WIN 
	  , m_pJumpList(new QWinJumpList(this))
#endif
{
	qRegisterMetaTypeStreamOperators<QList<int> >("QList<int>");
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
	m_pTorrentDisplayModel = new QTorrentDisplayModel(QTorrentDisplayModel::Compact, m_pTorrentListView, m_pTorrentFilterProxyModel, this);
	m_pTorrentFilterProxyModel->setSourceModel(m_pTorrentDisplayModel);
	m_pSearchFilterModel = new QSearchFilterModel();
	m_pSearchDisplayModel = new QSearchDisplayModel(m_pTorrentListView, m_pSearchFilterModel, this);
	m_pSearchFilterModel->setSourceModel(m_pSearchDisplayModel);
	m_pSearchItemDelegate = new QSearchItemDelegate(this);
	m_pRssDisplayModel = new QRssDisplayModel(m_pTorrentListView, this);
	m_pRssItemDelegate = new QRssItemDelegate(this);
	m_pUpdateNotifier = new UpdateNotifier(this);
	m_pCommentsModel = new CommentsModel(this);
	m_pCommentItemDelegate = new CommentItemDelegate(this);
	m_pFiltersViewModel = new FiltersViewModel(FiltersViewModel::All, this);
	{
		boost::scoped_ptr<InitializationDialog> pDlg(new InitializationDialog());
		pDlg->exec();
	}
	setAcceptDrops(true);
	setupItemDelegates();
	setupStatusBar();
	setupTray();
	setupFileTabel();
	setupRatingTab();
	setupRssInfoTab();
	setupToolBar();
	setupListView();
	setupTabelWidgets();
	setupGroupTreeWidget();
	setupLanguageChoseMenu();
	setupSkinChoseMenu();
	setupAutoShutdownMenu();
	setupConnections();
#ifdef Q_OS_WIN 
	setupJumpList();
#endif
	setupKeyMappings();
	setupViewMenuState();
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

	if (m_pSettings->valueBool("WatchDir", "enabled", false))
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
	QTimer::singleShot(2000, this, SLOT(CheckForDefaultTorrentApp()));
	if (m_pTorrentManager->shouldRate())
		QTimer::singleShot(5000, this, SLOT(showRaitingDialog()));

	Scheduller::getInstance();
	m_pUpdateTimer->start();
	m_initFinished = true;
}

void CuteTorrentMainWindow::setupItemDelegates()
{
	m_pProgressItemDelegate = new ProgressItemDelegate(this);
	m_pFileSizeItemDelegateNotShowZero = new FileSizeItemDelegate(true, this);
	m_pFileSizeItemDelegateShowZero = new FileSizeItemDelegate(false, this);
	m_pSpeedItemDelegate = new SpeedItemDelegate(this);
	m_pIpItemDelegate = new IpItemDelegate(this);
	m_pPriorityItemDelegate = new PriorityItemDelegate(this);
}

void CuteTorrentMainWindow::CheckForUpdates() const
{
	m_pUpdateNotifier->fetch();
}

void CuteTorrentMainWindow::ShowAbout()
{
	CustomMessageBox::about(tr("ABAUT_TITLE"), tr("ABAUT_TEXT").arg(Version::getVersionStr()));
}

void CuteTorrentMainWindow::initStatusBarIcons() const
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

void CuteTorrentMainWindow::updateTorrentSorterHeader() const
{
	m_torrentSortersModel->setData(m_torrentSortersModel->index(0, 0), "#", Qt::DisplayRole);
	m_torrentSortersModel->setData(m_torrentSortersModel->index(1, 0), tr("SORTER_NAME"), Qt::DisplayRole);
	m_torrentSortersModel->setData(m_torrentSortersModel->index(2, 0), tr("SORTER_SIZE"), Qt::DisplayRole);
	m_torrentSortersModel->setData(m_torrentSortersModel->index(3, 0), tr("SORTER_DOWNLOADED"), Qt::DisplayRole);
	m_torrentSortersModel->setData(m_torrentSortersModel->index(4, 0), tr("SORTER_UPLOADED"), Qt::DisplayRole);
	m_torrentSortersModel->setData(m_torrentSortersModel->index(5, 0), tr("SORTER_UPTIME"), Qt::DisplayRole);
	m_torrentSortersModel->setData(m_torrentSortersModel->index(6, 0), tr("SORTER_TIME_LEFT"), Qt::DisplayRole);
	m_torrentSortersModel->setData(m_torrentSortersModel->index(7, 0), tr("SORTER_SEEDS"), Qt::DisplayRole);
	m_torrentSortersModel->setData(m_torrentSortersModel->index(8, 0), tr("SORTER_PEERS"), Qt::DisplayRole);
}

void CuteTorrentMainWindow::setupTorrentHeaderModel()
{
	m_torrentSortersModel = new QStandardItemModel(9, 1, this);
	updateTorrentSorterHeader();
	m_torrentSortersModel->setData(m_torrentSortersModel->index(0, 0), QTorrentDisplayModel::TorrentQueuePosition, Qt::UserRole);
	m_torrentSortersModel->setData(m_torrentSortersModel->index(1, 0), QTorrentDisplayModel::TorrentName, Qt::UserRole);
	m_torrentSortersModel->setData(m_torrentSortersModel->index(2, 0), QTorrentDisplayModel::TorrentSize, Qt::UserRole);
	m_torrentSortersModel->setData(m_torrentSortersModel->index(3, 0), QTorrentDisplayModel::TorrentTotalDownloaded, Qt::UserRole);
	m_torrentSortersModel->setData(m_torrentSortersModel->index(4, 0), QTorrentDisplayModel::TorrentTotalUploaded, Qt::UserRole);
	m_torrentSortersModel->setData(m_torrentSortersModel->index(5, 0), QTorrentDisplayModel::TorrentUptime, Qt::UserRole);
	m_torrentSortersModel->setData(m_torrentSortersModel->index(6, 0), QTorrentDisplayModel::TorrentRemainingTime, Qt::UserRole);
	m_torrentSortersModel->setData(m_torrentSortersModel->index(7, 0), QTorrentDisplayModel::TorrentSeeds, Qt::UserRole);
	m_torrentSortersModel->setData(m_torrentSortersModel->index(8, 0), QTorrentDisplayModel::TorrentPeers, Qt::UserRole);
}

void CuteTorrentMainWindow::updateSearchSorterHeader() const
{
	m_searchSorterModel->setData(m_searchSorterModel->index(0, 0), tr("SORTER_NAME"), Qt::DisplayRole);
	m_searchSorterModel->setData(m_searchSorterModel->index(1, 0), tr("SORTER_SIZE"), Qt::DisplayRole);
	m_searchSorterModel->setData(m_searchSorterModel->index(2, 0), tr("SORTER_LEECHERS"), Qt::DisplayRole);
	m_searchSorterModel->setData(m_searchSorterModel->index(3, 0), tr("SORTER_SEEDERS"), Qt::DisplayRole);
}

void CuteTorrentMainWindow::setupSearchHeaderModel()
{
	m_searchSorterModel = new QStandardItemModel(4, 1, this);
	updateSearchSorterHeader();
	m_searchSorterModel->setData(m_searchSorterModel->index(0, 0), QSearchDisplayModel::SearchItemName, Qt::UserRole);
	m_searchSorterModel->setData(m_searchSorterModel->index(1, 0), QSearchDisplayModel::SearchItemSize, Qt::UserRole);
	m_searchSorterModel->setData(m_searchSorterModel->index(2, 0), QSearchDisplayModel::SearchItemPeers, Qt::UserRole);
	m_searchSorterModel->setData(m_searchSorterModel->index(3, 0), QSearchDisplayModel::SearchItemSeeders, Qt::UserRole);
}

void CuteTorrentMainWindow::setupListView()
{
	m_pTimeItemDelegate = new TimeItemDelegate(this);
	m_pTorrentItemDelegate = new QTorrentItemDelegat(this);
	m_pSimpleTorrentItemDelegate = new QStyledItemDelegate(this);
	m_pTorrentListView->header()->setSectionsMovable(true);
	m_pTorrentListView->setItemDelegate(m_pTorrentItemDelegate);
	m_pTorrentListView->setItemDelegateForColumn(QTorrentDisplayModel::Column::DownloadSpeed, m_pSpeedItemDelegate);
	m_pTorrentListView->setItemDelegateForColumn(QTorrentDisplayModel::Column::Peers, m_pSimpleTorrentItemDelegate);
	m_pTorrentListView->setItemDelegateForColumn(QTorrentDisplayModel::Column::Progress, m_pProgressItemDelegate);
	m_pTorrentListView->setItemDelegateForColumn(QTorrentDisplayModel::Column::RemainingTime, m_pTimeItemDelegate);
	m_pTorrentListView->setItemDelegateForColumn(QTorrentDisplayModel::Column::Seeds, m_pSimpleTorrentItemDelegate);
	m_pTorrentListView->setItemDelegateForColumn(QTorrentDisplayModel::Column::Size, m_pFileSizeItemDelegateShowZero);
	m_pTorrentListView->setItemDelegateForColumn(QTorrentDisplayModel::Column::DownloadSpeed, m_pSpeedItemDelegate);
	m_pTorrentListView->setItemDelegateForColumn(QTorrentDisplayModel::Column::UploadSpeed, m_pSpeedItemDelegate);
	m_pTorrentListView->setItemDelegateForColumn(QTorrentDisplayModel::Column::TotalDownloaded, m_pFileSizeItemDelegateNotShowZero);
	m_pTorrentListView->setItemDelegateForColumn(QTorrentDisplayModel::Column::TotalUploaded, m_pFileSizeItemDelegateNotShowZero);
	m_pTorrentListView->setItemDelegateForColumn(QTorrentDisplayModel::Column::Uptime, m_pTimeItemDelegate);
	m_pTorrentListView->setModel(m_pTorrentFilterProxyModel);
	m_pTorrentHeader = new EditableHeaderView(Qt::Horizontal, m_pTorrentListView);
	m_pTorrentListView->setHeader(m_pTorrentHeader);
	m_pTorrentHeader->setStretchLastSection(true);
	m_pTorrentListView->setHeaderHidden(true);
	setupTorrentHeaderModel();
	setupSearchHeaderModel();
	//sortByComboBox->setModel(m_torrentSortersModel);
	m_commentsView->setItemDelegate(m_pCommentItemDelegate);
	m_commentsView->setModel(m_pCommentsModel);
	/*m_pItemSorterView->setObjectName("torrentSorterView");
	m_pItemSorterView->setModel(m_torrentSortersModel);
	m_pItemSorterView->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	m_pItemSorterView->setSortIndicatorShown(true);
	m_pItemSorterView->setDefaultSectionSize(60);
	m_pItemSorterView->setClickable(true);
	m_pItemSorterView->setMinimumSectionSize(20);
	QList<int> defaultSizes = QList<int>() << 40 << 70 << 60 << 70 << 80 << 80 << 80;
	loadHeaderState("torrent_sorter", m_pItemSorterView, defaultSizes);*/

	/*m_pItemSorterView->setResizeMode(QHeaderView::Stretch);
	m_pItemSorterView->setSortIndicator(columnIndex, order);
	m_pTorrentFilterProxyModel->setSortRole(sortRole);
	m_pTorrentFilterProxyModel->sort(0, order);
	torrentViewLayout->insertWidget(0, m_pItemSorterView);*/
	m_pSortButton = new SortButton(this);
	m_pSortButton->setModel(m_torrentSortersModel);
	horizontalLayout_3->addWidget(m_pSortButton);
	QTorrentDisplayModel::Role sortRole = QTorrentDisplayModel::Role(m_pSettings->valueInt("Window", "torrent_sorter_sort_role", QTorrentDisplayModel::TorrentName));
	m_pSortButton->setSortRole(sortRole);
	//sortByComboBox->setCurrentIndex(columnIndex);
	Qt::SortOrder order = Qt::SortOrder(m_pSettings->valueInt("Window", "torrent_sorter_sort_order", Qt::AscendingOrder));
	m_pTorrentFilterProxyModel->setSortRole(sortRole);
	m_pTorrentFilterProxyModel->sort(0, order);
	m_pSortButton->setSortOrder(order);
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
	peerTableView->setContextMenuPolicy(Qt::ActionsContextMenu);
	addPeer = new QAction(m_pStyleEngine->getIcon("add_torrent"), tr("ADD_PEER"), peerTableView);
	addPeer->setObjectName("ACTION_PEER_ADD");
	addWebSeed = new QAction(m_pStyleEngine->getIcon("add_torrent"), tr("ADD_WEB_SEED"), peerTableView);
	addWebSeed->setObjectName("ACTION_PEER_ADD_WEB_SEED");
	connect(addPeer, SIGNAL(triggered()), this, SLOT(AddPeer()));
	connect(addWebSeed, SIGNAL(triggered()), this, SLOT(AddWebSeed()));
	peerTableView->addAction(addPeer);
	peerTableView->addAction(addWebSeed);
	peerTableView->setItemDelegateForColumn(PeerTableModel::IP, m_pIpItemDelegate);
	peerTableView->setItemDelegateForColumn(PeerTableModel::DOWNLOADED, m_pFileSizeItemDelegateNotShowZero);
	peerTableView->setItemDelegateForColumn(PeerTableModel::UPLOADED, m_pFileSizeItemDelegateNotShowZero);
	peerTableView->setItemDelegateForColumn(PeerTableModel::DOWNLOAD_RATE, m_pSpeedItemDelegate);
	peerTableView->setItemDelegateForColumn(PeerTableModel::UPLOAD_RATE, m_pSpeedItemDelegate);
	peerTableView->setItemDelegateForColumn(PeerTableModel::PEER_SPEED, m_pSpeedItemDelegate);
	peerTableView->setItemDelegateForColumn(PeerTableModel::PEER_PROGRESS, m_pProgressItemDelegate);
}

void CuteTorrentMainWindow::initToolbarIcons() const
{
	ACTION_TORRENTTOOLBAR_ADD_TORRENT->setIcon(m_pStyleEngine->getIcon("toolbar_add_rss"));
	ACTION_TORRENTTOOLBAR_OPEN_MAGNET->setIcon(m_pStyleEngine->getIcon("magnet_toolbar"));
	ACTION_TOOLBAR_SETTINGS->setIcon(m_pStyleEngine->getIcon("toolbar_settings"));
	ACTION_SEARCHTOOLBAR_DOWNLOAD->setIcon(m_pStyleEngine->getIcon("toolbar_download"));
	ACTION_SEARCHTOOLBAR_OPEN_URL->setIcon(m_pStyleEngine->getIcon("toolbar_open_url"));
	ACTION_RSSTOOLBAR_RSS_ADD->setIcon(m_pStyleEngine->getIcon("toolbar_add_rss"));
	ACTION_RSSTOOLBAR_RSS_REMOVE->setIcon(m_pStyleEngine->getIcon("toolbar_remove"));
	ACTION_RSSTOOLBAR_RSS_EDIT->setIcon(m_pStyleEngine->getIcon("toolbar_edit_rss"));
	ACTION_TORRENTTOOLBAR_QUEUE_UP->setIcon(m_pStyleEngine->getIcon("toolbar_queue_up"));
	ACTION_TORRENTTOOLBAR_QUEUE_DOWN->setIcon(m_pStyleEngine->getIcon("toolbar_queue_down"));
}

void CuteTorrentMainWindow::updateGroupVisibilityMenu() const
{
	int groupsIndex = horizontalSplitter->indexOf(m_pGroupTreeView);
	if (groupsIndex > -1)
	{
		QList<int> horizontalSplitterSizes = horizontalSplitter->sizes();
		qDebug() << "updateGroupVisibilityMenu sizes=" << horizontalSplitterSizes;
		ACTION_MENUVIEW_TOGGLE_GROUPS_VISIBILITY->setChecked(horizontalSplitterSizes[groupsIndex] != 0);
	}
}

void CuteTorrentMainWindow::updateInfoPlaneVsibilityMenu() const
{
	int deatailInfoIndex = verticalSplitter->indexOf(m_pInfoPlaneContainer);
	if (deatailInfoIndex > -1)
	{
		QList<int> verticalSplitterSizes = verticalSplitter->sizes();
		qDebug() << "updateInfoPlaneVsibilityMenu sizes=" << verticalSplitterSizes;

		bool isInfoPlaneVisible = verticalSplitterSizes[deatailInfoIndex] != 0;
		ACTION_MENUVIEW_TOGGLE_DETAIL_INFO_VISIBILITY->setChecked(isInfoPlaneVisible);
	}
}

void CuteTorrentMainWindow::setLanguage(QString language) const
{
	Application::setLanguage(language);
	Application::setLanguageQt(language);
	m_pSettings->setValue("System", "Lang", language);
}

void CuteTorrentMainWindow::setSkin(QString styleName) const
{
	m_pStyleEngine->setStyle(styleName);
	m_pSettings->setValue("System", "Style", styleName);
}

void CuteTorrentMainWindow::showReportDialog()
{
	boost::scoped_ptr<ReportProblemDialog> pDlg(new ReportProblemDialog());
	pDlg->exec();
}

void CuteTorrentMainWindow::showImportWiazrd()
{
	boost::scoped_ptr<ImportWizard> wizard(new ImportWizard(this));
	wizard->exec();
}

void CuteTorrentMainWindow::onAutoShutdownAction(int packedData) const
{
	//int packed = (idleType >> 16) & actionType;
	PowerManagement::ActionType action = PowerManagement::ActionType(short(packedData & 0xffff));
	PowerManagement::IdleType idleType = PowerManagement::IdleType(packedData >> 16);

	PowerManagementPtr powerManagement = PowerManagement::getInstance();
	powerManagement->setIdleType(idleType);
	powerManagement->setIdleAction(action);
}

void CuteTorrentMainWindow::onResetPowerMenu() const
{
	autoShutdownMenu->actions().first()->setChecked(true);
}

void CuteTorrentMainWindow::pauseAllTorrents() const
{
	m_pTorrentManager->PauseAllTorrents();
}

void CuteTorrentMainWindow::restoreAllTorrents() const
{
	m_pTorrentManager->StartAllTorrents();
}

void CuteTorrentMainWindow::showRaitingDialog()
{
	shared_ptr<RaitingDialog> pDlg(new RaitingDialog());
	pDlg->exec();
}

void CuteTorrentMainWindow::CheckForDefaultTorrentApp()
{
	bool openTorrents = ValueGetters::TorrentAssociationValueGetter("System", "open_torrent_files").toBool();
	bool openMagnetLinks = ValueGetters::MagnetAssociationValueGetter("System", "open_magent_links").toBool();
	QString message;
	if (!openTorrents && !openMagnetLinks)
	{
		message = tr("SET_DEFAULT_APP_FOR_TOR_AND_MAGNET_LINKS");
	}
	else
	{
		if (!openTorrents)
		{
			message = tr("SET_DEFAULT_APP_FOR_TOR");
		}
		else if (!openMagnetLinks)
		{
			message = tr("SET_DEFAULT_APP_FOR_MAGNET_LINKS");
		}
	}
	if ((!openTorrents || !openMagnetLinks) && !message.isEmpty())
	{
		if (CustomMessageBox::question(NULL, tr("DEFAULT_APP"), message) == QMessageBox::Yes)
		{
			ValueSetters::TorrentAssociationValueSetter("System", "open_torrent_files", true);
			ValueSetters::MagnetAssociationValueSetter("System", "open_magent_links", true);
		}
	}
}

void CuteTorrentMainWindow::ShowAddCommentDialog()
{
	shared_ptr<CommentsWebClient> commentsWebClient = CommentsWebClient::getInstance();
	if (!commentsWebClient->isLoggedIn())
	{
		CustomMessageBox::CustomButtonInfoList buttonsInfo;
		CustomMessageBox::CustomButonInfo regButtonInfo = {tr("REGISTER_BTN"), QDialogButtonBox::AcceptRole};
		CustomMessageBox::CustomButonInfo loginButtonInfo = {tr("LOG_IN_BTN"), QDialogButtonBox::AcceptRole};
		buttonsInfo << loginButtonInfo << regButtonInfo;
		CustomMessageBox::Buttons buttons = CustomMessageBox::Cancel | CustomMessageBox::CustomButton1 | CustomMessageBox::CustomButton2;
		CustomMessageBox::Button button = CustomMessageBox::warning(NULL, tr("NOT_LOGGED_IN"), tr("COMMENTS_LOGIN_MESSAGE"), buttons, buttonsInfo);
		qDebug() << "Not logged in dlg button pressed" << button;
		switch (button)
		{
			case CustomMessageBox::Cancel:
				{
					return;
				}
			case CustomMessageBox::CustomButton1:
				{
					boost::scoped_ptr<LoginDialog> pDlg(new LoginDialog());
					if (pDlg->exec() == QDialog::Rejected)
					{
						return;
					}
					break;
				}
			case CustomMessageBox::CustomButton2:
				{
					boost::scoped_ptr<RegisterDialog> pDlg(new RegisterDialog());
					if (pDlg->exec() == QDialog::Rejected)
					{
						return;
					}
					break;
				}

			default: break;
		}
	}
	boost::scoped_ptr<AddCommentDialog> pAddCommentDlg(new AddCommentDialog(AddCommentDialog::Add, m_pTorrentDisplayModel->GetSelectedTorrent()->GetInfoHash()));
	pAddCommentDlg->exec();
}

void CuteTorrentMainWindow::UpdateStyleMenu()
{
	StyleInfo currentStyle = m_pStyleEngine->getCuurentStyle();
	QList<QAction*> actions = skinSubMenu->actions();
	for (int i = 0; i < actions.size(); i++)
	{
		QAction* action = actions[i];
		if (action->text() == currentStyle.DisplayName)
		{
			action->setChecked(true);
			break;
		}
	}
}

void CuteTorrentMainWindow::OnCommentRemove(const QModelIndex& index)
{
	if (index.isValid())
	{
		if (CustomMessageBox::question(tr("Delete comment"), tr("Are you shure you want to delete selected comment?")) == CustomMessageBox::Yes)
		{
			Comment comment = index.data(CommentsModel::CommentRole).value<Comment>();

			shared_ptr<CommentsWebClient> commentsWebClient = CommentsWebClient::getInstance();
			commentsWebClient->DeleteComment(comment);
		}
	}
}

void CuteTorrentMainWindow::OnTorrentAction(QModelIndex index, QTorrentItemDelegat::Buttons button)
{
	Torrent* torrent = index.data(QTorrentDisplayModel::TorrentRole).value<Torrent*>();
	if (torrent != NULL)
	{
		switch (button)
		{
			case QTorrentItemDelegat::Pause:
				{
					if (torrent->isPaused())
					{
						m_pTorrentDisplayModel->ActionOnSelectedItem(QTorrentDisplayModel::resume);
					}
					else
					{
						m_pTorrentDisplayModel->ActionOnSelectedItem(QTorrentDisplayModel::pause);
					}
					break;
				}
			case QTorrentItemDelegat::Remove:
				{
					m_pTorrentDisplayModel->ActionOnSelectedItem(QTorrentDisplayModel::remove_all);
					break;
				}
			default:
				{
					break;
				}
		}
	}
}

void CuteTorrentMainWindow::setupViewMenuState() const
{
	bool toolbarVisible = m_pSettings->valueBool("Window", "toolbar_visible");
	ACTION_MENUVIEW_TOGGLE_TOOLBAR_VISIBILITY->setChecked(toolbarVisible);
	m_pToolBarsContainer->setVisible(toolbarVisible);
	bool statusbarVisible = m_pSettings->valueBool("Window", "statusbar_visible");
	ACTION_MENUVIEW_TOGGLE_STATUSBAR_VISIBILITY->setChecked(statusbarVisible);
	mystatusBar->setVisible(statusbarVisible);
	updateInfoPlaneVsibilityMenu();
	updateGroupVisibilityMenu();
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
	categoriesToStr[ISerachProvider::Movie] = tr("FILMS_CATEGORY");
	categoriesToStr[ISerachProvider::All] = tr("ALL_CATEGORY");

	for (int i = categoriesToStr.size() - 1; i >= 0; i--)
	{
		m_pSearchCategory->addItem(categoriesToStr.values().at(i), categoriesToStr.keys().at(i));
	}

	m_pSearchCategory->setCurrentIndex(0);
	m_pSearchEdit = new SearchLineEdit(this);
	m_pSearchEdit->setPlaceholderText(tr("Search"));
	m_pTorrentSearchEdit = new SearchLineEdit(this);
	m_pTorrentSearchEdit->setMaximumWidth(150);
	m_pTorrentSearchEdit->setPlaceholderText(tr("TORENT_FILTER"));
	connect(m_pTorrentSearchEdit, SIGNAL(textChanged(const QString&)), this, SLOT(PeformTorrentSearch(const QString&)));
	connect(m_pSearchEdit, SIGNAL(returnPressed()), this, SLOT(PeformSearch()));
	//ul = new QSpinBox(this);
	ul->setSpecialValueText(tr("None"));
	ul->setSuffix(" Kb\\s");
	ul->setMaximum(12000);
	ul->setSingleStep(10);
	ul->setButtonSymbols(QAbstractSpinBox::PlusMinus);
	connect(ul, SIGNAL(valueChanged(int)), this, SLOT(UpdateUL(int)));
	//dl = new QSpinBox(this);
	dl->setSpecialValueText(tr("None"));
	dl->setMaximum(12000);
	dl->setSuffix(" Kb\\s");
	dl->setSingleStep(10);
	dl->setButtonSymbols(QAbstractSpinBox::PlusMinus);
	connect(dl, SIGNAL(valueChanged(int)), this, SLOT(UpdateDL(int)));
	/*	uploadLimit = new QLabel(tr("LIMIT_UL"), this);
		uploadLimit->setBuddy(ul);
		downloadLimit = new QLabel(tr("LIMIT_DL"), this);
		downloadLimit->setBuddy(dl);*/
	QWidget* spacer = new QWidget();
	spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	m_pSearchCategory->setMinimumWidth(120);
	/*m_pTorrentToolBar->addWidget(uploadLimit);
	m_pTorrentToolBar->addWidget(ul);
	m_pTorrentToolBar->addWidget(downloadLimit);
	m_pTorrentToolBar->addWidget(dl);*/
	m_pTorrentToolBar->addWidget(spacer);
	m_pTorrentToolBar->addWidget(m_pTorrentSearchEdit);
	m_pTorrentSearchEdit->setContentsMargins(0, 0, 5, 0);
	QWidget* searchSpacer = new QWidget();
	searchSpacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	m_pSearchToolBar->addWidget(searchSpacer);
	m_pSearchEdit->setContentsMargins(0, 0, 5, 0);
	m_pSearchEdit->setMaximumWidth(150);
	m_pSearchToolBar->addWidget(m_pSearchCategory);
	m_pSearchToolBar->addWidget(m_pSearchEdit);
	UpdateTabWidget();
}

void CuteTorrentMainWindow::setupAutoShutdownMenu()
{
	m_pAutoshutdownActionGroup = new QActionGroup(this);
	m_pAutoshutdownSignalMapper = new QSignalMapper(this);
	QList<QAction*> autoshutdownActions = autoShutdownMenu->actions();
	autoshutdownActions.first()->setChecked(true);
	for (int i = 0; i < autoshutdownActions.size(); i++)
	{
		if (autoshutdownActions[i]->isCheckable())
		{
			m_pAutoshutdownActionGroup->addAction(autoshutdownActions[i]);
			QString actionName = autoshutdownActions[i]->objectName();
			if (actionName.isEmpty())
				continue;

			short actionType = -1, idleType = -1;
			if (actionName.contains("_SHUTDOWN", Qt::CaseInsensitive))
			{
				autoshutdownActions[i]->setIcon(m_pStyleEngine->getIcon("shutdown"));
				actionType = PowerManagement::SHUTDOWN;
			}
			else if (actionName.contains("_REBOOT", Qt::CaseInsensitive))
			{
				autoshutdownActions[i]->setIcon(m_pStyleEngine->getIcon("reboot"));
				actionType = PowerManagement::REBOOT;
			}
			else if (actionName.contains("_SLEEP", Qt::CaseInsensitive))
			{
				autoshutdownActions[i]->setIcon(m_pStyleEngine->getIcon("standby"));
				actionType = PowerManagement::SLEEP;
			}
			else if (actionName.contains("_HIBERNATE", Qt::CaseInsensitive))
			{
				autoshutdownActions[i]->setIcon(m_pStyleEngine->getIcon("sleep"));
				actionType = PowerManagement::HIBERNATE;
			}
			else
			{
				actionType = PowerManagement::NONE;
			}

			if (actionName.contains("_on_dow", Qt::CaseInsensitive))
			{
				idleType = PowerManagement::IDLE_DOWNLOAD;
			}
			else if (actionName.contains("_on_all", Qt::CaseInsensitive))
			{
				idleType = PowerManagement::IDLE_ALL;
			}
			else
			{
				idleType = PowerManagement::NOT_IDLE;
			}

			int packed = (idleType << 16) | actionType;
			m_pAutoshutdownSignalMapper->setMapping(autoshutdownActions[i], packed);
			connect(autoshutdownActions[i], SIGNAL(triggered()), m_pAutoshutdownSignalMapper,SLOT(map()));
		}
	}
	connect(m_pAutoshutdownSignalMapper, SIGNAL(mapped(int)), SLOT(onAutoShutdownAction(int)));
}

void CuteTorrentMainWindow::setupConnections() const
{
	connect(m_pTorrentListView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
	        m_pTorrentDisplayModel, SLOT(UpdateSelectedIndex(const QItemSelection&)));
	connect(m_pTorrentListView, SIGNAL(customContextMenuRequested(const QPoint&)), m_pTorrentDisplayModel, SLOT(contextualMenu(const QPoint&)));
	connect(m_pTorrentListView, SIGNAL(customContextMenuRequested(const QPoint&)), m_pRssDisplayModel, SLOT(contextualMenu(const QPoint&)));
	connect(m_pTorrentListView, SIGNAL(customContextMenuRequested(const QPoint&)), m_pSearchDisplayModel, SLOT(contextualMenu(const QPoint&)));
	connect(m_pTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
	        this, SLOT(IconActivated(QSystemTrayIcon::ActivationReason)));
	connect(ACTION_MENUFILE_EXIT, SIGNAL(triggered()), qApp, SLOT(quit()));
	connect(m_pTabWidget, SIGNAL(currentChanged(int)), this, SLOT(UpdateTabWidget()));
	connect(m_pTorrentListView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
	        this, SLOT(UpdateTabWidget()));
	connect(m_pTorrentListView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
	        this, SLOT(UpdateLimits()));
	connect(m_pUpdateTimer, SIGNAL(timeout()), SLOT(UpdateStatusbar()));
	connect(m_pUpdateTimer, SIGNAL(timeout()), SLOT(UpdateTabWidget()));
#ifdef Q_OS_WIN 
	connect(m_pUpdateTimer, SIGNAL(timeout()), SLOT(UpdateTrayIconOverlay()));
#endif
	connect(fileTableView, SIGNAL(customContextMenuRequested(const QPoint&)), m_pFileViewModel, SLOT(FileTabContextMenu(const QPoint&)));
	connect(m_pGroupTreeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(ChnageTorrentFilter()));
	connect(m_pStyleEngine, SIGNAL(styleChanged()), this, SLOT(initWindowIcons()));
	connect(m_pStyleEngine, SIGNAL(styleChanged()), m_pTorrentDisplayModel, SLOT(setupContextMenu()));
	connect(m_pStyleEngine, SIGNAL(styleChanged()), this, SLOT(UpdateStyleMenu()));
	connect(m_pTorrentListView, SIGNAL(doubleClicked(const QModelIndex&)), m_pTorrentDisplayModel, SLOT(OpenDirSelected()));
	connect(m_pTorrentListView, SIGNAL(doubleClicked(const QModelIndex&)), m_pSearchDisplayModel, SLOT(downloadTorrent()));
	connect(ACTION_SEARCHTOOLBAR_DOWNLOAD, SIGNAL(triggered()), m_pSearchDisplayModel, SLOT(downloadTorrent()));
	connect(ACTION_MENUHELP_ABOUT_QT, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	connect(qApp, SIGNAL(aboutToQuit()), SLOT(OnQuit()));
	connect(horizontalSplitter, SIGNAL(splitterMoved(int, int)), SLOT(updateGroupVisibilityMenu()));
	connect(verticalSplitter, SIGNAL(splitterMoved(int, int)), SLOT(updateInfoPlaneVsibilityMenu()));
	//connect(sortDirectionToggleButton, SIGNAL(toggled(bool)), SLOT(updateSortDirection()));
	//connect(sortByComboBox, SIGNAL(currentIndexChanged(int)), SLOT(updateSorting(int)));
	connect(m_pSortButton, SIGNAL(sortRoleChanged(int)), SLOT(updateSorting(int)));
	connect(m_pSortButton, SIGNAL(sortOrderChanged(Qt::SortOrder)), SLOT(updateSortDirection()));
	PowerManagementPtr powerManagement = PowerManagement::getInstance();
	connect(powerManagement.get(), SIGNAL(resetPowerState()), SLOT(onResetPowerMenu()));
	connect(m_pCommentItemDelegate, SIGNAL(buttonClicked(QModelIndex)), SLOT(OnCommentRemove(QModelIndex)));
	connect(m_pTorrentItemDelegate, SIGNAL(buttonClicked(QModelIndex, QTorrentItemDelegat::Buttons)), SLOT(OnTorrentAction(QModelIndex, QTorrentItemDelegat::Buttons)));
	connect(m_pTorrentManager.get(), SIGNAL(FileRenameCompleted()), m_pFileViewModel, SLOT(OnFileRenameCompleted()));
}

void CuteTorrentMainWindow::UpdateStatusbar() const
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

void CuteTorrentMainWindow::UpdateLimits() const
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

void CuteTorrentMainWindow::UpdateTabWidget() const
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
		case 4:
			{
				UpadateCommentsTab();
				break;
			}
		default:
			break;
	}
}

void CuteTorrentMainWindow::UpdateFileTab() const
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

void CuteTorrentMainWindow::UpadateCommentsTab() const
{
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if (tor != NULL)
	{
		m_pCommentsModel->setTorrentHash(tor->GetInfoHash());
		m_pRatingWidget->setRating(m_pCommentsModel->AverageRating());
		addCommentButton->setEnabled(true);
	}
	else
	{
		m_pCommentsModel->Clear();
		m_pRatingWidget->setRating(-1);
		addCommentButton->setEnabled(false);
	}
}

static int cnt = 0;

void CuteTorrentMainWindow::UpdateTrayIconOverlay() const
{
	int downloadingCount = m_pFiltersViewModel->getDownloadingTorrentsCount();
	if (!downloadingCount)
	{
		m_pTaskBarBtn->clearOverlayIcon();
	}
	else
	{
		QIcon icon(":/icons/appOverlay.ico");
		QPixmap iconPixmap = icon.pixmap(256, 256);
		QSize pixmapSize = iconPixmap.size();
		QPainter painter(&iconPixmap);
		QRectF rect(QPointF(0, 0), pixmapSize);
		painter.setPen(QColor(Qt::white));
		painter.setBrush(QBrush(Qt::white));
		QFont font = painter.font();
		font.setBold(true);
		font.setPixelSize(pixmapSize.height() * 0.7);
		painter.setFont(font);
		painter.drawText(rect, Qt::AlignCenter, QString::number(downloadingCount));
		painter.end();

		QIcon iconWithNumber(iconPixmap);
		m_pTaskBarBtn->setOverlayIcon(iconWithNumber);
		m_pTaskBarBtn->setOverlayAccessibleDescription(tr("DOWNLOADING_TORRENTS_COUNT"));
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
			/*			uploadLimit->setText(tr("LIMIT_UL"));
						downloadLimit->setText(tr("LIMIT_DL"));*/
			addTracker->setText(tr("ADD_TRACKER"));
			removeTracker->setText(tr("REMOVE_TRACKER"));
			editTracker->setText(tr("EDIT_TRACKER"));
			addPeer->setText(tr("ADD_PEER"));
			ul->setSpecialValueText(tr("None"));
			dl->setSpecialValueText(tr("None"));
			pauseAll->setText(tr("ACTION_PAUSE_ALL_TORRENTS"));
			restoreAll->setText(tr("ACTION_RESTORE_ALL_TORRENTS"));
			m_pPeerTableModel->Retranslate();
			m_pFileViewModel->retranslateUI();
			m_pTorrentDisplayModel->retranslate();
			m_pRssDisplayModel->retranslate();
			m_pFiltersViewModel->Retranslate();
			m_pSearchDisplayModel->retranslate();
			QMap<ISerachProvider::SearchCategories, QString> categoriesToStr;
			categoriesToStr[ISerachProvider::Anime] = tr("ANIME_CATEGORY");
			categoriesToStr[ISerachProvider::Music] = tr("MUSIC_CATEGORY");
			categoriesToStr[ISerachProvider::TV] = tr("TV_CATEGORY");
			categoriesToStr[ISerachProvider::Porn] = tr("PORN_CATEGORY");
			categoriesToStr[ISerachProvider::Software] = tr("APP_CATEGORY");
			categoriesToStr[ISerachProvider::Games] = tr("GAMES_CATEGORY");
			categoriesToStr[ISerachProvider::Books] = tr("BOOKS_CATEGORY");
			categoriesToStr[ISerachProvider::Movie] = tr("FILMS_CATEGORY");
			categoriesToStr[ISerachProvider::All] = tr("ALL_CATEGORY");
			int prevSearchCat = m_pSearchCategory->currentIndex();
			m_pSearchEdit->setPlaceholderText(tr("Search"));
			m_pTorrentSearchEdit->setPlaceholderText(tr("TORENT_FILTER"));
			updateTorrentSorterHeader();
			updateSearchSorterHeader();
#ifdef Q_OS_WIN 
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
	trayIconMenu->addAction(pauseAll);
	trayIconMenu->addAction(restoreAll);
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
	restoreAction = new QAction(m_pStyleEngine->getIcon("app_reset"), tr("ACTION_MAXIMIZE"), this);
	restoreAction->setObjectName("ACTION_TRAY_RESTORE");
	connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
	pauseAll = new QAction(m_pStyleEngine->getIcon("pause"), tr("ACTION_PAUSE_ALL_TORRENTS"), this);
	pauseAll->setObjectName("ACTION_TRAY_PAUSE_ALL_TORRENTS");
	connect(pauseAll, SIGNAL(triggered()), SLOT(pauseAllTorrents()));
	restoreAll = new QAction(m_pStyleEngine->getIcon("play"), tr("ACTION_RESTORE_ALL_TORRENTS"), this);
	restoreAll->setObjectName("ACTION_TRAY_RESTORE_ALL_TORRENTS");
	connect(restoreAll, SIGNAL(triggered()), SLOT(restoreAllTorrents()));
	quitAction = new QAction(m_pStyleEngine->getIcon("app_close"), tr("ACTION_EXIT"), this);
	quitAction->setObjectName("ACTION_TRAY_EXIT");
	connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
	copyContext = new QAction(m_pStyleEngine->getIcon("copy"), tr("ACTION_COPY"), describtionLabel);
	copyContext->setObjectName("ACTION_TORRENTDESCRIPTION_COPY");
	connect(copyContext, SIGNAL(triggered()), this, SLOT(CopyDiscribtion()));
	describtionLabel->addAction(copyContext);
	copyInfoHash = new QAction(m_pStyleEngine->getIcon("copy"), tr("ACTION_COPY"), infoHashLabel);
	copyInfoHash->setObjectName("ACTION_INFOHASH_COPY");
	connect(copyInfoHash, SIGNAL(triggered()), this, SLOT(CopyInfoHash()));
	infoHashLabel->addAction(copyInfoHash);
}

void CuteTorrentMainWindow::ConnectMessageReceved(Application* a) const
{
	connect(a, SIGNAL(messageReceived(const QString&)), this, SLOT(OnMessageRecived(const QString&)));
}

void CuteTorrentMainWindow::RaiseWindow()
{
	setUpdatesEnabled(true);
	// This hack does not give the focus to the app but brings it to front so
	// the user sees it.
#ifdef Q_OS_WIN 
	SetWindowPos(reinterpret_cast<HWND>(effectiveWinId()), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	SetWindowPos(reinterpret_cast<HWND>(effectiveWinId()), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
#endif
	// HACK END
	raise();
	activateWindow();
}

void CuteTorrentMainWindow::HandleNewTorrent(const QString& path)
{
	showNormal();
	RaiseWindow();
	boost::scoped_ptr<OpenTorrentDialog> dlg(new OpenTorrentDialog());
	dlg->SetData(path);
	QApplication::alert(dlg.get());

	if (dlg->exec() == QDialog::Accepted)
	{
#ifdef Q_OS_WIN 
		QWinJumpListCategory* recent = m_pJumpList->recent();
		/*QWinJumpListItem* item = new QWinJumpListItem(QWinJumpListItem::Link);
		item->setFilePath(path);
		recent->addLink();*/
		recent->setVisible(true);
#endif
	}
}

void CuteTorrentMainWindow::ShowCreateTorrentDialog(QString path)
{
	RaiseWindow();
	boost::scoped_ptr<CreateTorrentDialog> dlg(new CreateTorrentDialog);
	dlg->setPath(path);
	QApplication::alert(dlg.get());
	dlg->exec();
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

void CuteTorrentMainWindow::UpdateInfoTab() const
{
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if (tor != NULL)
	{
		fillPieceDisplay(m_pPieceView->size());
		downloadedBytesLabel->setText(tor->GetTotalDownloadedStr());
		uploadedBytesLabel->setText(tor->GetTotalUploadedStr());
		downloadSpeedLabel->setText(tor->GetDwonloadSpeedStr());
		activetimeLabel->setText(tor->GetActiveTimeStr());
		uploadSpeedLabel->setText(tor->GetUploadSpeedStr());
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
		availibilityLabel->setText(QString::number(abs(tor->GetDistributedCopies()), 'f', 2));
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

void CuteTorrentMainWindow::UpdatePeerTab() const
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

void CuteTorrentMainWindow::UpadteTrackerTab() const
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

		for (std::vector<announce_entry>::size_type i = 0; i < trackers.size(); i++)
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

void CuteTorrentMainWindow::PauseSelected() const
{
	m_pTorrentDisplayModel->ActionOnSelectedItem(QTorrentDisplayModel::pause);
}

void CuteTorrentMainWindow::ResumeSelected() const
{
	m_pTorrentDisplayModel->ActionOnSelectedItem(QTorrentDisplayModel::resume);
}

void CuteTorrentMainWindow::DeleteSelected() const
{
	m_pTorrentDisplayModel->ActionOnSelectedItem(QTorrentDisplayModel::remove);
}

void CuteTorrentMainWindow::Retranslate()
{
	retranslateUi(this);
}

void CuteTorrentMainWindow::OpenSettingsDialog()
{
	boost::scoped_ptr<SettingsDialog> dlg(new SettingsDialog);
	connect(dlg.get(), SIGNAL(needRetranslate()), this, SLOT(Retranslate()));
	dlg->exec();
	m_pGroupTreeView->expandToDepth(0);
	setupKeyMappings();
	UpdateTabWidget();
}

void CuteTorrentMainWindow::closeEvent(QCloseEvent* ce)
{
	bool valueBool = m_pSettings->valueBool("Window", "close_on_hide", true);
	qDebug() << "String cast result" << valueBool;
	if (valueBool)
	{
		ce->ignore();
		hide();
		m_pNotificationSystem->OnNewNotification(NotificationSystem::TORRENT_INFO, tr("CT_HIDE_MSG"), QVariant());
		setUpdatesEnabled(false);
		return;
	}
	QWidget::closeEvent(ce);
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
			CustomMessageBox::warning(tr("UNABLE_TO_PROCESS_FILE"), tr("DROPPED_FILE %1 IS_NOT_TORRENT_OR_MAGNET_LINK").arg(file));
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
	qDebug() << "horizontal_sizes=" << horizontalSplitter->sizes();
	m_pSettings->setValue("Window", "horizontal_sizes", QVariant::fromValue(horizontalSplitter->sizes()));
	qDebug() << "vertical_sizes=" << verticalSplitter->sizes();
	m_pSettings->setValue("Window", "vertical_sizes", QVariant::fromValue(verticalSplitter->sizes()));
	saveHeaderState("peers", m_pPeersHeader);
	saveHeaderState("files", m_pFilesHeader);
	saveHeaderState("trackers", m_pTrackersHeader);
}

void CuteTorrentMainWindow::loadHeaderState(QString prefix, EditableHeaderView* header, QList<int>& defaultColumnSizes) const
{
	QList<int> columnSizes = m_pSettings->value("Window", prefix % "_sizes", QVariant::fromValue(defaultColumnSizes)).value<QList<int>>();
	header->setStretchLastSection(true);
	int columnCount = columnSizes.count();
	//Q_ASSERT_X(defaultColumnSizes.size() == columnCount, "CuteTorrentMainWindow::loadHeaderState", "Inconsistent default sizes given");

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

void CuteTorrentMainWindow::saveHeaderState(QString prefix, EditableHeaderView* header) const
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

void CuteTorrentMainWindow::setupLanguageChoseMenu()
{
	m_pLanguageSignalMapper = new QSignalMapper(this);
	QStringList availableLanguages = Application::availableLanguages();
	QString currentLanguage = m_pSettings->valueString("System", "Lang", "en_US");
	m_pLanguageActionGroup = new QActionGroup(this);
	m_pLanguageActionGroup->setExclusive(true);
	for (int i = 0; i < availableLanguages.size(); i++)
	{
		QLocale language = QLocale(availableLanguages[i]);
		QString flagFileName = QString(":/flags/%1.png").arg(StaticHelpers::GetCountryCode(language.language(), language.country()));
		QIcon icon = QIcon(QPixmap(flagFileName));
		QString languageStr = language.nativeLanguageName();
		languageStr[0] = languageStr[0].toUpper();
		QAction* action = languageSubMenu->addAction(icon, languageStr, m_pLanguageSignalMapper, SLOT(map()));
		action->setCheckable(true);
		action->setChecked(availableLanguages[i] == currentLanguage);
		m_pLanguageSignalMapper->setMapping(action, availableLanguages[i]);
		m_pLanguageActionGroup->addAction(action);
	}
	connect(m_pLanguageSignalMapper,SIGNAL(mapped(QString)), SLOT(setLanguage(QString)));
}

void CuteTorrentMainWindow::setupSkinChoseMenu()
{
	m_pSkinSignalMapper = new QSignalMapper(this);
	QList<StyleInfo> styleInfos = m_pStyleEngine->getAvaliableStyles();
	m_pSkinActionGroup = new QActionGroup(this);
	StyleInfo currentStyle = m_pStyleEngine->getCuurentStyle();
	m_pSkinActionGroup->setExclusive(true);
	for (int i = 0; i < styleInfos.size(); i++)
	{
		StyleInfo styleInfo = styleInfos[i];
		QAction* action = skinSubMenu->addAction(styleInfo.DisplayName, m_pSkinSignalMapper, SLOT(map()));
		action->setCheckable(true);
		action->setChecked(styleInfo.InternalName == currentStyle.InternalName);
		m_pSkinActionGroup->addAction(action);
		m_pSkinSignalMapper->setMapping(action, styleInfo.InternalName);
	}
	connect(m_pSkinSignalMapper, SIGNAL(mapped(QString)), SLOT(setSkin(QString)));
}

void CuteTorrentMainWindow::setupFileTabel()
{
	m_pFileViewProxymodel = new FileViewSortProxyModel(this);
	fileTableView->setModel(m_pFileViewProxymodel);
	m_pFilesHeader = new EditableHeaderView(Qt::Horizontal, fileTableView);
	m_pFileViewModel = new FileViewModel(fileTableView, this);
	m_pFileViewProxymodel->setSourceModel(m_pFileViewModel);
	fileTableView->setItemDelegateForColumn(1, m_pFileSizeItemDelegateShowZero);
	fileTableView->setItemDelegateForColumn(2, m_pProgressItemDelegate);
	fileTableView->setItemDelegateForColumn(3, m_pPriorityItemDelegate);
	QList<int> defaultFileSectionSizes = QList<int>() << 450 << 65 << 65 << 70;
	fileTableView->setHeader(m_pFilesHeader);
	loadHeaderState("files", m_pFilesHeader, defaultFileSectionSizes);
	fileTableView->header()->setDefaultAlignment(Qt::AlignCenter);
}


void CuteTorrentMainWindow::ProcessMagnet()
{
	bool ok;
	const QClipboard* clipboard = QApplication::clipboard();
	const QMimeData* mimeData = clipboard->mimeData();
	QString clipboardMagentLink;
	if (mimeData->hasText())
	{
		clipboardMagentLink = mimeData->text();
		if (!clipboardMagentLink.startsWith("magnet:"))
		{
			clipboardMagentLink = "";
		}
	}
	QString magnetLink = QInputDialog::getText(this, tr("MAGNET_LINK_DLG"), tr("MAGNET_LINK:"), QLineEdit::Normal, clipboardMagentLink, &ok);

	if (ok && !magnetLink.isEmpty())
	{
		boost::scoped_ptr<OpenTorrentDialog> dialog(new OpenTorrentDialog);
		dialog->SetData(magnetLink);
		dialog->exec();
	}
}

void CuteTorrentMainWindow::PeformSearch() const
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

void CuteTorrentMainWindow::PeformTorrentSearch(const QString& text) const
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

void CuteTorrentMainWindow::setupRatingTab()
{
	m_pRatingWidget = new RatingWidget(this);
	m_pRatingWidget->setIsReadonly(true);
	m_pRatingWidget->setFixedHeight(20);
	ratingTabLayout->addWidget(m_pRatingWidget, 0, 1);
}

void CuteTorrentMainWindow::fillPieceDisplay(QSize size) const
{
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if (tor != NULL)
	{
		QBitArray avaliablePieces = tor->GetDownloadedPieces();
		QBitArray dwonloadingPieces = tor->GetDownloadingPieces();
		m_pPieceView->resize(size);
		m_pPieceView->setProgress(avaliablePieces, dwonloadingPieces);
	}
}

void CuteTorrentMainWindow::CopyDiscribtion() const
{
	QClipboard* clipboard = QApplication::clipboard();
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if (tor != NULL)
	{
		clipboard->setText(tor->GetDiscribtion());
	}
}

void CuteTorrentMainWindow::CopyInfoHash() const
{
	QClipboard* clipboard = QApplication::clipboard();
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if (tor != NULL)
	{
		clipboard->setText(tor->GetInfoHash().toUpper());
	}
}

void CuteTorrentMainWindow::ClearPieceDisplay() const
{
	m_pPieceView->clear();
	m_pieceAvalibilityWidget->clear();
}

void CuteTorrentMainWindow::UpdateUL(int kbps) const
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

void CuteTorrentMainWindow::UpdateDL(int kbps) const
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

			CustomMessageBox::critical(tr("PEER_ERR"), tr("INVALID_IP_STRING"));
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

void CuteTorrentMainWindow::StopSelected() const
{
	m_pTorrentDisplayModel->ActionOnSelectedItem(QTorrentDisplayModel::stop);
}

void CuteTorrentMainWindow::setupGroupTreeWidget() const
{
	m_pGroupTreeView->setModel(m_pFiltersViewModel);
	m_pGroupTreeView->resizeColumnToContents(0);
	m_pGroupTreeView->expandToDepth(0);
}
#ifdef Q_OS_WIN 
void CuteTorrentMainWindow::setupTasksCategory() const
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

void CuteTorrentMainWindow::setupJumpList() const
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

		case SEARCH:
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

		case TORRENT:
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
	boost::scoped_ptr<BackupWizard> wizard(new BackupWizard(this));
	wizard->exec();
}

void CuteTorrentMainWindow::initMainMenuIcons() const
{
	ACTION_TORRENTTOOLBAR_SWITCH_VIEW->setIcon(m_pStyleEngine->getIcon("grid-view"));
	ACTION_MENUHELP_ABAUT_CT->setIcon(m_pStyleEngine->getIcon("about"));
	ACTION_MENUHELP_CHECK_UPDATE->setIcon(m_pStyleEngine->getIcon("check_update"));
	ACTION_MENUFILE_CREATE->setIcon(m_pStyleEngine->getIcon("create_torrent"));
	ACTION_MENUFILE_EXIT->setIcon(m_pStyleEngine->getIcon("menu_exit"));
	ACTION_MENUFILE_OPEN_MAGNET->setIcon(m_pStyleEngine->getIcon("magnet"));
	ACTION_MENUFILE_OPEN_TORRENT->setIcon(m_pStyleEngine->getIcon("add_torrent"));
	ACTION_MENU_SETTINGS->setIcon(m_pStyleEngine->getIcon("menu_settings"));
	ACTION_TOOLS_BACKUP->setIcon(m_pStyleEngine->getIcon("menu_backup"));
	ACTION_TOOLS_IMPORT->setIcon(m_pStyleEngine->getIcon("import"));
	ACTION_MENUHELP_ABOUT_QT->setIcon(m_pStyleEngine->getIcon("qt"));
	ACTION_MENUHELP_REPORT_PROBLEM->setIcon(m_pStyleEngine->getIcon("report"));
	ACTION_MENUHELP_RATE->setIcon(m_pStyleEngine->getIcon("star"));
	autoShutdownMenu->setIcon(m_pStyleEngine->getIcon("power"));
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

	QList<int> horizontal_sizes = m_pSettings->value("Window", "horizontal_sizes", qVariantFromValue(QList<int>() << 130 << 538)).value<QList<int>>();
	m_splitterInitFinished = false;
	if (horizontal_sizes.size() > 0)
	{
		horizontalSplitter->setSizes(horizontal_sizes);
	}

	QList<int> vertical_sizes = m_pSettings->value("Window", "vertical_sizes", qVariantFromValue(QList<int>() << 530 << 195)).value<QList<int>>();

	if (vertical_sizes.size() > 0)
	{
		verticalSplitter->setSizes(vertical_sizes);
	}
	m_splitterInitFinished = true;
	QList<QWidget*> children = findChildren<QWidget*>();

	for (int i = 0; i < children.size(); i++)
	{
		QWidget* child = children[i];
		child->installEventFilter(this);
	}
}

void CuteTorrentMainWindow::setupKeyMappings() const
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



void CuteTorrentMainWindow::switchTorrentViewMode()
{
	int currentTorrentViewMode = m_pTorrentDisplayModel->viewMode();
	switch(currentTorrentViewMode)
	{
		case QTorrentDisplayModel::Compact: 
			m_pTorrentDisplayModel->setViewMode(QTorrentDisplayModel::Extended);
			m_pTorrentListView->setItemDelegateForColumn(QTorrentDisplayModel::Name, m_pSimpleTorrentItemDelegate);
			m_pTorrentListView->setHeaderHidden(false);
			ACTION_TORRENTTOOLBAR_SWITCH_VIEW->setIcon(m_pStyleEngine->getIcon("list-view"));
			break;
		case QTorrentDisplayModel::Extended: 
			m_pTorrentDisplayModel->setViewMode(QTorrentDisplayModel::Compact);
			m_pTorrentListView->setItemDelegateForColumn(QTorrentDisplayModel::Name, m_pTorrentItemDelegate);
			m_pTorrentListView->setHeaderHidden(true);
			ACTION_TORRENTTOOLBAR_SWITCH_VIEW->setIcon(m_pStyleEngine->getIcon("grid-view"));

			break;
		default: ;
	}

	
}

void CuteTorrentMainWindow::queueTorrentsUp() const
{
	m_pTorrentDisplayModel->ActionOnSelectedItem(QTorrentDisplayModel::queue_up);
}

void CuteTorrentMainWindow::queueTorrentsDown() const
{
	m_pTorrentDisplayModel->ActionOnSelectedItem(QTorrentDisplayModel::queue_down);
}

void CuteTorrentMainWindow::updateSorting(int sortRole) const
{
	if (m_ignoreSortByIndexChange)
		return;
	if (m_pTorrentListView->model() == m_pTorrentFilterProxyModel)
	{
		m_pTorrentFilterProxyModel->setSortRole(sortRole);
		m_pSettings->setValue("Window", "torrent_sorter_sort_role", sortRole);
	}
	else if (m_pTorrentListView->model() == m_pSearchFilterModel)
	{
		m_pSearchFilterModel->setSortRole(sortRole);
		m_pSettings->setValue("Window", "search_sorter_sort_role", sortRole);
	}
}

void CuteTorrentMainWindow::UpdateModelSortOrder(QString modelPrefix, QSortFilterProxyModel* pModel) const
{
	Qt::SortOrder sortOrder = m_pSortButton->sortOrder();

	pModel->sort(0, sortOrder);
	m_pSettings->setValue("Window", modelPrefix % "_sort_order", sortOrder);
}

void CuteTorrentMainWindow::updateSortDirection() const
{
	if (m_pTorrentListView->model() == m_pTorrentFilterProxyModel)
	{
		UpdateModelSortOrder("torrent_sorter", m_pTorrentFilterProxyModel);
	}
	else if (m_pTorrentListView->model() == m_pSearchFilterModel)
	{
		UpdateModelSortOrder("search_sorter", m_pSearchFilterModel);
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
		//saveHeaderState("search_sorter", m_pItemSorterView);

		SortersContainer->setVisible(true);
		TorentLimitsContainer->setVisible(true);
		m_ignoreSortByIndexChange = true;
		m_pSortButton->setModel(m_torrentSortersModel);
		m_ignoreSortByIndexChange = false;
		/*m_pItemSorterView->setVisible(true);
		
		QList<int> defaultSizes = QList<int>() << 40 << 70 << 60 << 70 << 80 << 80 << 80;
		//loadHeaderState("torrent_sorter", m_pItemSorterView, defaultSizes);*/
		int role = m_pSettings->valueInt("Window", "torrent_sorter_sort_role");
		m_pTorrentFilterProxyModel->setSortRole(role);
		int sortRole = m_pTorrentFilterProxyModel->sortRole();
		m_pSortButton->setSortRole(sortRole);
		Qt::SortOrder sortOrder = static_cast<Qt::SortOrder>(m_pSettings->valueInt("Window", "torrent_sorter_sort_order"));
		m_pSortButton->setSortOrder(sortOrder);
		updateSortDirection();
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
		m_pTorrentListView->setHeaderHidden(true);
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
		TorentLimitsContainer->setVisible(false);
		//saveHeaderState("torrent_sorter", m_pItemSorterView);
		SortersContainer->setVisible(true);
		m_ignoreSortByIndexChange = true;
		m_pSortButton->setModel(m_searchSorterModel);
		m_ignoreSortByIndexChange = false;
		/*m_pItemSorterView->setVisible(true);
		m_pItemSorterView->setModel(m_searchSorterModel);
		QList<int> defaultSearchSize = QList<int>() << 80 << 80 << 80 << 80;
		//loadHeaderState("search_sorter", m_pItemSorterView, defaultSearchSize);*/
		int role = m_pSettings->valueInt("Window", "search_sorter_sort_role");
		m_pSearchFilterModel->setSortRole(role);
		int sortRole = m_pSearchFilterModel->sortRole();
		m_pSortButton->setSortRole(sortRole);
		Qt::SortOrder sortOrder = static_cast<Qt::SortOrder>(m_pSettings->valueInt("Window", "search_sorter_sort_order"));
		m_pSortButton->setSortOrder(sortOrder);
		updateSortDirection();
	}
}

void CuteTorrentMainWindow::switchToRssModel() const
{
	if (m_pTorrentListView->model() != m_pRssDisplayModel)
	{
		m_pTorrentListView->setHeaderHidden(true);
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
		SortersContainer->setVisible(false);
	}
}

void CuteTorrentMainWindow::openSearchItemDescribtion() const
{
	m_pSearchDisplayModel->openDescription();
}


void CuteTorrentMainWindow::RemoveTracker() const
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

		for (int j = 0; j < selectedRows.size(); j++)
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

void CuteTorrentMainWindow::UpdateTracker() const
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

void CuteTorrentMainWindow::removeRssFeed() const
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
			boost::scoped_ptr<RssFeedSettingsDialog> pDialog(new RssFeedSettingsDialog);
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

void CuteTorrentMainWindow::UpdateRssInfo(const QItemSelection& /*selection*/) const
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

void CuteTorrentMainWindow::toggleInfoTabsVisibility(bool visibility) const
{
	int index = verticalSplitter->indexOf(m_pInfoPlaneContainer);
	if (index > -1 && m_splitterInitFinished)
	{
		QList<int> currentSizes = verticalSplitter->sizes();
		bool currentVisibility = currentSizes[index] != 0;
		if (visibility)
		{
			if (currentVisibility)
				return;

			QList<int> sizes = m_pSettings->value("Window", "precollapsed_vertical_sizes", m_pSettings->value("Window", "vertical_sizes")).value<QList<int>>();
			verticalSplitter->setSizes(sizes);
		}
		else
		{
			if (!currentVisibility)
				return;
			int itemsCount = verticalSplitter->count();
			QList<int> sizes = QList<int>::fromVector(QVector<int>(itemsCount, 1));
			m_pSettings->setValue("Window", "precollapsed_vertical_sizes", QVariant::fromValue(verticalSplitter->sizes()));
			sizes[index] = 0;
			verticalSplitter->setSizes(sizes);
		}
	}
}

void CuteTorrentMainWindow::toggleToolBarVisibility(bool b)
{
	m_pToolBarsContainer->setVisible(b);
	m_pSettings->setValue("Window", "toolbar_visible", b);
}

void CuteTorrentMainWindow::toggleStatusBarVisibility(bool b) const
{
	mystatusBar->setVisible(b);
	m_pSettings->setValue("Window", "statusbar_visible", b);
}

void CuteTorrentMainWindow::toggleGroupsVisibility(bool visibility) const
{
	int index = horizontalSplitter->indexOf(m_pGroupTreeView);
	if (index > -1 && m_splitterInitFinished)
	{
		QList<int> currentSizes = horizontalSplitter->sizes();
		bool currentVisibility = currentSizes[index] != 0;
		if (visibility)
		{
			if (currentVisibility)
				return;

			QList<int> sizes = m_pSettings->value("Window", "precollapsed_horizontal_sizes", m_pSettings->value("Window", "horizontal_sizes")).value<QList<int>>();
			horizontalSplitter->setSizes(sizes);
		}
		else
		{
			if (!currentVisibility)
				return;
			int itemsCount = horizontalSplitter->count();
			QList<int> sizes = QList<int>::fromVector(QVector<int>(itemsCount, 1));
			m_pSettings->setValue("Window", "precollapsed_horizontal_sizes", QVariant::fromValue(horizontalSplitter->sizes()));
			sizes[index] = 0;
			horizontalSplitter->setSizes(sizes);
		}
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
		CustomMessageBox::information("CuteTorrent", tr("INVALID_COMMAND_RECIVED"));
	}
}

void CuteTorrentMainWindow::setupRssInfoTab()
{
	m_pFeedItemDescribtionEdit = new HtmlView(this);
	m_pRssInfoLayout->addWidget(m_pFeedItemDescribtionEdit, 0, 0, 1, 1);
}
