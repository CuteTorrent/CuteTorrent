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
#include <QIcon>
#include <QProcess>
#include <QShortcut>
#include <QSortFilterProxyModel>
#include <QTreeWidgetItem>
#include <QUrl>
#include <QtNetwork/QHostAddress>
#include <exception>

#include "CreateTorrentDialog.h"
#include "CuteTorrent.h"
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
#include "tracker/torrentracker.h"
#include "versionInfo.h"
#include "webControll/RconWebService.h"
#include "messagebox.h"
#include "QBaloon.h"
#include "backupwizard/backupwizard.h"
#include "RssFeedSettingsDialog.h"
#include "HtmlView.h"
class Application;
class ISerachProvider;
class SearchResult;

Q_DECLARE_METATYPE(QList<int>)

CuteTorrent::CuteTorrent(QWidget* parent)
	: BaseWindow(BaseWindow::FullTitle, BaseWindow::AllowResize)
{
	m_pSettings = QApplicationSettings::getInstance();
	Application::setLanguage(m_pSettings->valueString("System", "Lang", "ru_RU"));
	Application::setLanguageQt(m_pSettings->valueString("System", "Lang", "ru_RU"));
	m_pStyleEngine = StyleEngene::getInstance();
	m_pSearchEngine = new SearchEngine();
	m_pStyleEngine->setStyle(m_pSettings->valueString("System", "Style", "CuteTorrent"));
	setWindowModality(Qt::NonModal);
	setupCustomeWindow();
	m_pTorrentFilterProxyModel = new QTorrentFilterProxyModel(this);
	m_pTorrentDisplayModel = new QTorrentDisplayModel(m_pTorrentListView, m_pTorrentFilterProxyModel, this);
	m_pTorrentFilterProxyModel->setSourceModel(m_pTorrentDisplayModel);
	m_pTorrentFilterProxyModel->setDynamicSortFilter(true);
	m_pSearchDisplayModel = new QSearchDisplayModel(m_pSearchEngine, m_pTorrentListView);
	m_pSearchItemDelegate = new QSearchItemDelegate();
	m_pTorrentManager = TorrentManager::getInstance();
	m_pRssDisplayModel = new QRssDisplayModel(m_pTorrentListView, this);
	m_pRssItemDelegate = new QRssItemDelegate();
	m_pUpdateNotifier = new UpdateNotifier();
	mayShowNotifies = false;
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
	setupKeyMappings();
	initWindowIcons();
	m_pTracker = TorrentTracker::getInstance();
	m_pRcon = RconWebService::getInstance();

	if(m_pSettings->valueBool("WebControl", "webui_enabled", false))
	{
		m_pRcon->Start();

		if(m_pSettings->valueBool("WebControl", "enable_ipfilter", false))
		{
			m_pRcon->parseIpFilter(m_pSettings->valueString("WebControl", "ipfilter"));
		}

		if(m_pSettings->valueBool("WebControl", "enable_upnp", false))
		{
			int port = m_pSettings->valueInt("WebControl", "port", 8080);
			m_pTorrentManager->AddPortMapping(session::tcp, port, port);
		}
	}

	if(m_pSettings->valueBool("TorrentTracker", "enabled", false))
	{
		m_pTracker->start();
	}

	m_centralWidget->installEventFilter(this);
	m_pTorrentListView->installEventFilter(this);
	m_pGroupTreeWidget->installEventFilter(this);
	m_pTabWidget->installEventFilter(this);
	m_pTorrentManager->InitSession();
	m_pTorrents = TorrentStorrage::getInstance();
	QTimer::singleShot(10000, this, SLOT(CheckForUpdates()));
	Scheduller* sch = Scheduller::getInstance();
}
void CuteTorrent::CheckForUpdates()
{
	m_pUpdateNotifier->fetch();
}
void CuteTorrent::ShowAbout()
{
	MyMessageBox::about(this, tr("ABAUT_TITLE"), tr("ABAUT_TEXT").arg(VER_FILE_VERSION_STR));
}
void CuteTorrent::ShowUpdateNitify(const QString& newVersion)
{
	QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information;
	QBalloonTip::showBalloon("CuteTorrent", tr("CT_NEW_VERSION %1").arg(newVersion), QBalloonTip::UpdateNotyfy, qVariantFromValue(0), icon,
	                         5 * 1000);
}
void CuteTorrent::initStatusBarIcons()
{
	upLabel->setPixmap(m_pStyleEngine->getIcon("status_bar_upload").pixmap(16, 16));
	downLabel->setPixmap(m_pStyleEngine->getIcon("status_bar_download").pixmap(16, 16));
}

void CuteTorrent::setupStatusBar()
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
	gridLayout_4->addWidget(m_pPieceView, 0, 0, 1, 5);
	initStatusBarIcons();
}
void CuteTorrent::setupListView()
{
	QTorrentItemDelegat::max_width = width() - QApplication::style()->pixelMetric(QStyle::PM_LargeIconSize) - 35 - (m_pTorrentListView->verticalScrollBar()->isVisible() ?
	                                 m_pTorrentListView->autoScrollMargin() : 0) - m_pGroupTreeWidget->width();
	QSearchItemDelegate::max_width = width() - QApplication::style()->pixelMetric(QStyle::PM_ToolBarIconSize) - 35 - (m_pTorrentListView->verticalScrollBar()->isVisible() ?
	                                 m_pTorrentListView->autoScrollMargin() : 0) - m_pGroupTreeWidget->width();
	m_pTorrentItemDelegate = new QTorrentItemDelegat(this);
	m_pTorrentListView->setItemDelegate(m_pTorrentItemDelegate);
	m_pTorrentListView->setModel(m_pTorrentFilterProxyModel);
}
void CuteTorrent::setupTabelWidgets()
{
	QList<int> tracker_column_sizes = m_pSettings->value("Window", "trackers_sizes").value<QList<int>>();

	if(tracker_column_sizes.count() > 0)
	{
		for(int i = 0; i < tracker_column_sizes.count(); i++)
		{
			trackerTableWidget->setColumnWidth(i, tracker_column_sizes.at(i));
		}
	}
	else
	{
		trackerTableWidget->setColumnWidth(0, 320);
		trackerTableWidget->setColumnWidth(2, 120);
		trackerTableWidget->setColumnWidth(3, 100);
	}

	trackerTableWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
	addTracker = new QAction(m_pStyleEngine->getIcon("add_torrent"), tr("ADD_TRACKER"), trackerTableWidget);
	addTracker->setObjectName("ACTION_TRACKER_ADD");
	removeTracker = new QAction(m_pStyleEngine->getIcon("delete"), tr("REMOVE_TRACKER"), trackerTableWidget);
	removeTracker->setObjectName("ACTION_TRACKER_REMOVE");
	editTracker = new QAction(m_pStyleEngine->getIcon("delete"), tr("EDIT_TRACKER"), trackerTableWidget);
	editTracker->setObjectName("ACTION_TRACKER_EDIT");
	QObject::connect(addTracker, SIGNAL(triggered()), this, SLOT(AddTracker()));
	QObject::connect(removeTracker, SIGNAL(triggered()), this, SLOT(RemoveTracker()));
	QObject::connect(editTracker, SIGNAL(triggered()), this, SLOT(EditTracker()));
	trackerTableWidget->addAction(addTracker);
	trackerTableWidget->addAction(removeTracker);
	trackerTableWidget->addAction(editTracker);
	QList<int> peer_column_sizes = m_pSettings->value("Window", "peers_sizes").value<QList<int>>();

	if(peer_column_sizes.count() > 0)
	{
		for(int i = 0; i < peer_column_sizes.count(); i++)
		{
			peerTableWidget->setColumnWidth(i, peer_column_sizes.at(i));
		}
	}
	else
	{
		peerTableWidget->setColumnWidth(2, 50);
	}

	peerTableWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
	addPeer = new QAction(m_pStyleEngine->getIcon("add_torrent"), tr("ADD_PEER"), peerTableWidget);
	addPeer->setObjectName("ACTION_PEER_ADD");
	addWebSeed = new QAction(m_pStyleEngine->getIcon("add_torrent"), tr("ADD_WEB_SEED"), peerTableWidget);
	addWebSeed->setObjectName("ACTION_PEER_ADD_WEB_SEED");
	QObject::connect(addPeer, SIGNAL(triggered()), this, SLOT(AddPeer()));
	QObject::connect(addWebSeed, SIGNAL(triggered()), this, SLOT(AddWebSeed()));
	peerTableWidget->addAction(addPeer);
	peerTableWidget->addAction(addWebSeed);
}
void CuteTorrent::initToolbarIcons()
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
}

void CuteTorrent::setupToolBar()
{
	initToolbarIcons();
	m_pTorrentSearchCategory = new QComboBox(this);
	m_pSearchCategory = new QComboBox(this);
	QMap<ISerachProvider::SearchCategories, QString> categoriesToStr;
	categoriesToStr[ISerachProvider::Anime] =	 tr("ANIME_CATEGORY");
	categoriesToStr[ISerachProvider::Music] =	 tr("MUSIC_CATEGORY");
	categoriesToStr[ISerachProvider::TV] =		 tr("TV_CATEGORY");
	categoriesToStr[ISerachProvider::Porn] =	 tr("PORN_CATEGORY");
	categoriesToStr[ISerachProvider::Software] = tr("APP_CATEGORY");
	categoriesToStr[ISerachProvider::Games] =	 tr("GAMES_CATEGORY");
	categoriesToStr[ISerachProvider::Books] =	 tr("BOOKS_CATEGORY");
	categoriesToStr[ISerachProvider::Movie] =	 tr("FILMS_CATEGORY");;
	categoriesToStr[ISerachProvider::All] =		 tr("ALL_CATEGORY");;;

	for (int i = categoriesToStr.size() - 1; i >= 0; i--)
	{
		m_pTorrentSearchCategory->addItem(categoriesToStr.values().at(i), categoriesToStr.keys().at(i));
		m_pSearchCategory->addItem(categoriesToStr.values().at(i), categoriesToStr.keys().at(i));
	}

	m_pTorrentSearchCategory->setCurrentIndex(0);
	m_pSearchCategory->setCurrentIndex(0);
	m_pSearchEdit = new QLineEdit(this);
	m_pSearchEdit->setPlaceholderText(tr("Search"));
	m_pTorrentSearchEdit = new QLineEdit(this);
	m_pTorrentSearchEdit->setPlaceholderText(tr("Search"));
	QObject::connect(m_pTorrentSearchEdit, SIGNAL(returnPressed()), this, SLOT(PeformSearch()));
	QObject::connect(m_pSearchEdit, SIGNAL(returnPressed()), this, SLOT(PeformSearch()));
	ul = new QSpinBox(this);
	ul->setSuffix(" Kb\\s");
	ul->setMaximum(12000.0f);
	ul->setSingleStep(10.0);
	ul->setButtonSymbols(QAbstractSpinBox::PlusMinus);
	QObject::connect(ul, SIGNAL(valueChanged(int)), this, SLOT(UpdateUL(int)));
	dl = new QSpinBox(this);
	ul->setSpecialValueText(tr("None"));
	dl->setSpecialValueText(tr("None"));
	dl->setMaximum(12000.0f);
	dl->setSuffix(" Kb\\s");
	dl->setSingleStep(10.0);
	dl->setButtonSymbols(QAbstractSpinBox::PlusMinus);
	QObject::connect(dl, SIGNAL(valueChanged(int)), this, SLOT(UpdateDL(int)));
	uploadLimit = new QLabel(tr("LIMIT_UL"), this);
	uploadLimit->setBuddy(ul);
	downloadLimit = new QLabel(tr("LIMIT_DL"), this);
	downloadLimit->setBuddy(dl);
	m_pSearchEdit->setMaximumWidth(145);
	m_pTorrentSearchEdit->setMaximumWidth(145);
	m_pTorrentSearchCategory->setMinimumWidth(120);
	m_pSearchCategory->setMinimumWidth(120);
	m_pTorrentToolBar->addWidget(uploadLimit);
	m_pTorrentToolBar->addWidget(ul);
	m_pTorrentToolBar->addWidget(downloadLimit);
	m_pTorrentToolBar->addWidget(dl);
	QWidget* spacer = new QWidget();
	spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	m_pSearchToolBar->addWidget(spacer);
	m_pSearchToolBar->addWidget(m_pSearchEdit);
	m_pSearchToolBar->addWidget(m_pSearchCategory);
	m_pTorrentToolBar->addWidget(m_pTorrentSearchEdit);
	m_pTorrentToolBar->addWidget(m_pTorrentSearchCategory);
	UpdateTabWidget(-2);
}
void CuteTorrent::setupConnections()
{
	QObject::connect(m_pTorrentListView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
	                 m_pTorrentDisplayModel, SLOT(UpdateSelectedIndex(const QItemSelection&)));
	QObject::connect(m_pTorrentListView, SIGNAL(customContextMenuRequested(const QPoint&)), m_pTorrentDisplayModel, SLOT(contextualMenu(const QPoint&)));
	QObject::connect(m_pTorrentListView, SIGNAL(customContextMenuRequested(const QPoint&)), m_pSearchDisplayModel, SLOT(contextualMenu(const QPoint&)));
	QObject::connect(m_pTorrentListView, SIGNAL(customContextMenuRequested(const QPoint&)), m_pRssDisplayModel, SLOT(contextualMenu(const QPoint&)));
	QObject::connect(m_pTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
	                 this, SLOT(IconActivated(QSystemTrayIcon::ActivationReason)));
	QObject::connect(ACTION_MENU_EXIT, SIGNAL(triggered()), qApp, SLOT(quit()));
	QObject::connect(m_pTabWidget, SIGNAL(currentChanged(int)), this, SLOT(UpdateTabWidget(int)));
	QObject::connect(m_pTorrentDisplayModel, SIGNAL(updateTabSender(int)), this, SLOT(UpdateTabWidget(int)));
	QObject::connect(m_pTorrentManager, SIGNAL(TorrentError(const QString&, const QString&)), this, SLOT(ShowTorrentError(const QString&, const QString&)));
	QObject::connect(m_pTorrentManager, SIGNAL(TorrentCompleted(const QString&, const QString&)),
	                 this, SLOT(ShowTorrentCompletedNotyfy(const QString, const QString)));
	QObject::connect(m_pTorrentManager, SIGNAL(TorrentInfo(const QString&, const QString&)),
	                 this, SLOT(ShowTorrentInfoNotyfy(const QString, const QString)));
	QObject::connect(m_pUpdateNotifier, SIGNAL(showUpdateNitify(const QString&)), this, SLOT(ShowUpdateNitify(const QString&)));
	QObject::connect(m_pUpdateNotifier, SIGNAL(showNoUpdateNitify(const QString&)), this, SLOT(ShowNoUpdateNitify(const QString&)));
	QObject::connect(fileTableView, SIGNAL(customContextMenuRequested(const QPoint&)), m_pFileViewModel, SLOT(FileTabContextMenu(const QPoint&)));
	QObject::connect(m_pTorrentManager, SIGNAL(initCompleted()), m_pTorrentDisplayModel, SLOT(initSessionFinished()));
	QObject::connect(m_pTorrentDisplayModel, SIGNAL(initCompleted()), this, SLOT(EnableNitifyShow()));
	QObject::connect(m_pGroupTreeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(ChnageTorrentFilter()));
	QObject::connect(m_pStyleEngine, SIGNAL(styleChanged()), this, SLOT(initWindowIcons()));
	QObject::connect(m_pStyleEngine, SIGNAL(styleChanged()), m_pTorrentDisplayModel, SLOT(setupContextMenu()));
	QObject::connect(m_pTorrentListView, SIGNAL(doubleClicked(const QModelIndex&)), m_pTorrentDisplayModel, SLOT(OpenDirSelected()));
	QObject::connect(m_pSearchEngine, SIGNAL(GotResults()), this, SLOT(OnGotSerachResults()));
	QObject::connect(m_pTorrentSearchEdit, SIGNAL(textEdited(const QString&)), m_pSearchEdit, SLOT(setText(const QString&)));
	QObject::connect(m_pSearchEdit, SIGNAL(textEdited(const QString&)), m_pTorrentSearchEdit, SLOT(setText(const QString&)));
	QObject::connect(qApp, SIGNAL(aboutToQuit()), SLOT(OnQuit()));
}
void CuteTorrent::ShowNoUpdateNitify(const QString& ver)
{
}
void CuteTorrent::ShowTorrentError(const QString& name, const QString& error)
{
	if(!mayShowNotifies)
	{
		return;
	}

	QBalloonTip::showBalloon("CuteTorrent", tr("CT_ERROR %1\n%2").arg(name).arg(error), QBalloonTip::Error, qVariantFromValue(0),
	                         QSystemTrayIcon::Critical, 15000, false);
}
void CuteTorrent::ShowTorrentCompletedNotyfy(const QString& name, const QString& path)
{
	if(!mayShowNotifies)
	{
		return;
	}

	QBalloonTip::showBalloon("CuteTorrent", tr("CT_DOWNLOAD_COMPLETE %1").arg(name), QBalloonTip::TorrentCompleted, qVariantFromValue(path + name),
	                         QSystemTrayIcon::Information, 15000, false);
}
void CuteTorrent::UpdateTabWidget(int tabIndex)
{
	m_pTrayIcon->setToolTip("CuteTorrent "VER_FILE_VERSION_STR"\nUpload: " + m_pTorrentManager->GetSessionUploadSpeed() + "\nDownload:" + m_pTorrentManager->GetSessionDownloadSpeed());

	if(this->isMinimized())
	{
		return;
	}

	bool udapteLimits = false;

	if(tabIndex < 0)
	{
		if(tabIndex == -2)
		{
			udapteLimits = true;
		}

		tabIndex = m_pTabWidget->currentIndex();
	}

	try
	{
		switch(tabIndex)
		{
			case 0:
				UpdateInfoTab();
				break;

			case 1:
				UpdatePeerTab();
				break;

			case 2:
				UpadteTrackerTab();
				break;

			case 3:
				UpdateFileTab();
				break;
		}

		if(udapteLimits)
		{
			Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

			if(tor != NULL)
			{
				if(ul->value() != tor->GetUploadLimit() / 1024)
				{
					ul->setValue(tor->GetUploadLimit() / 1024);
				}

				if(dl->value() != tor->GetDownloadLimit() / 1024)
				{
					dl->setValue(tor->GetDownloadLimit() / 1024);
				}
			}
			else
			{
				if(ul->value() != m_pTorrentManager->GetUploadLimit() / 1024)
				{
					ul->setValue(m_pTorrentManager->GetUploadLimit() / 1024);
				}

				if(dl->value() != m_pTorrentManager->GetDownloadLimit() / 1024)
				{
					dl->setValue(m_pTorrentManager->GetDownloadLimit() / 1024);
				}
			}
		}

		upLabelText->setText(QString("%1(%2)").arg(m_pTorrentManager->GetSessionUploaded(), m_pTorrentManager->GetSessionUploadSpeed()));
		downLabelText->setText(QString("%1(%2)").arg(m_pTorrentManager->GetSessionDownloaded(), m_pTorrentManager->GetSessionDownloadSpeed()));
		dhtNodesLabel->setText(tr("DHT: %1 nodes").arg(m_pTorrentManager->GetSessionDHTstate()));
		m_pTorrentFilterProxyModel->Update();
	}
	catch(std::exception e)
	{
		MyMessageBox::warning(this, "Error", QString("CuteTorrent::updateTabWidget()\n") + e.what());
	}
}

class MyTableWidgetItem : public QTableWidgetItem
{
public:
	MyTableWidgetItem(QString text) : QTableWidgetItem(text)
	{
	}
	bool operator < (const QTableWidgetItem& other) const
	{
		QHostAddress thisadr(text());

		if(!thisadr.isNull())
		{
			QHostAddress otheradr(other.text());
			return thisadr.toIPv4Address() < otheradr.toIPv4Address();
		}
		else if(text().contains(" tb", Qt::CaseInsensitive) || text().contains(" gb", Qt::CaseInsensitive) || text().contains(" kb", Qt::CaseInsensitive) || text().contains(" mb", Qt::CaseInsensitive) ||
		        text().contains(" b", Qt::CaseInsensitive))
		{
			QStringList parts1 = text().split(' ');
			bool ok;
			double speed1 = parts1[0].toDouble(&ok);

			if(ok)
			{
				QStringList parts2 = other.text().split(' ');
				double speed2 = parts2[0].toDouble();

				switch(parts1[1][0].toLower().toLatin1())
				{
					case 'k':
						speed1 *= 1024.0;
						break;

					case 'm':
						speed1 *= 1024 * 1024.0;
						break;

					case 'g':
						speed1 *= 1024 * 1024 * 1024.0;
						break;

					case 't':
						speed1 *= 1024 * 1024 * 1024 * 1024.0;
						break;

					case 'b':
						break;
				}

				switch(parts2[1][0].toLower().toLatin1())
				{
					case 'k':
						speed2 *= 1024;
						break;

					case 'm':
						speed2 *= 1024 * 1024;
						break;

					case 'g':
						speed2 *= 1024 * 1024 * 1024;
						break;

					case 't':
						speed2 *= 1024 * 1024 * 1024 * 1024.0;
						break;

					case 'b':
						break;
				}

				return speed1 < speed2;
			}
		}
		else if(text().endsWith('%'))
		{
			QString perc1 = text().remove('%'), perc2 = other.text().remove('%');
			return perc1.toDouble() < perc2.toDouble();
		}

		return QTableWidgetItem::operator < (other);
	}
};
void CuteTorrent::UpdateFileTab()
{
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if(tor != NULL)
	{
		if(m_pFileViewModel->setDataSource(tor->GetInternalHandle()))
		{
			fileTableView->expand(m_pFileViewProxymodel->index(0, 0));
		}
	}
	else
	{
		m_pFileViewModel->setDataSource(torrent_handle());
	}
}
void CuteTorrent::setupTray()
{
	createActions();
	createTrayIcon();
	m_pTrayIcon->setToolTip("CuteTorrent "VER_FILE_VERSION_STR);
	m_pTrayIcon->show();
}

void CuteTorrent::changeEvent(QEvent* event)
{
	if(event->type() == QEvent::ActivationChange)
	{
		if(!isVisible() && isMinimized())
		{
			setUpdatesEnabled(false);
		}
		else
		{
			setUpdatesEnabled(true);
		}
	}

	if(event->type() == QEvent::LanguageChange)
	{
		retranslateUi(this);
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
		torrentTreeItem->setText(0, tr("TORRENTS_ACTIVITY"));
		dlTreeItem->setText(0, tr("DOWNLOADING_FLTR"));
		ulTreeItem->setText(0, tr("SEEDING_FLTR"));
		completedTreeItem->setText(0, tr("COMPLETED_FLTR"));
		activeTreeItem->setText(0, tr("ACTIVE_FLTR"));
		inactiveTreeItem->setText(0, tr("NOT_ACTIVE_FLTR"));
		groupsTreeItem->setText(0, tr("TORRENT_GROUPS"));
		ul->setSpecialValueText(tr("None"));
		dl->setSpecialValueText(tr("None"));
		m_pFileViewModel->retranslateUI();
		m_pTorrentDisplayModel->retranslate();
		m_pRssDisplayModel->retranslate();
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
		m_pSearchCategory->clear();
		m_pTorrentSearchCategory->clear();

		for (int i = categoriesToStr.size() - 1; i >= 0; i--)
		{
			m_pTorrentSearchCategory->addItem(categoriesToStr.values().at(i), categoriesToStr.keys().at(i));
			m_pSearchCategory->addItem(categoriesToStr.values().at(i), categoriesToStr.keys().at(i));
		}

		m_pTorrentSearchCategory->setCurrentIndex(0);
		m_pSearchCategory->setCurrentIndex(0);
	}

	QWidget::changeEvent(event);
}
void CuteTorrent::IconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch(reason)
	{
		case QSystemTrayIcon::DoubleClick:
			if(isVisible())
			{
				hide();
			}
			else
			{
				showNormal();
				raise();
				activateWindow();
			}

			break;

		default:
			break;
	}
}

void CuteTorrent::createTrayIcon()
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
void CuteTorrent::createActions()
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
	copyContext->setObjectName("ACTION_OTHER_DISCRIBTION");
	connect(copyContext, SIGNAL(triggered()), this, SLOT(CopyDiscribtion()));
	describtionLabel->addAction(copyContext);
}
void CuteTorrent::ConnectMessageReceved(Application* a)
{
	QObject::connect(a, SIGNAL(messageReceived(const QString&)), this, SLOT(HandleNewTorrent(const QString&)));
	QObject::connect(a, SIGNAL(OpenTorrent(QString)), this, SLOT(HandleNewTorrent(QString)));
}
void CuteTorrent::HandleNewTorrent(const QString& path)
{
	// This hack does not give the focus to the app but brings it to front so
	// the user sees it.
	::SetWindowPos(effectiveWinId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::SetWindowPos(effectiveWinId(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	// HACK END
	showNormal();
	setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
	raise();
	activateWindow();
	OpenTorrentDialog dlg(this);
	dlg.SetData(path);
	dlg.execConditional();
	QApplication::alert(&dlg);
}
void CuteTorrent::ShowCreateTorrentDialog(void)
{
	CreateTorrentDialog* dlg = new CreateTorrentDialog();
	dlg->exec();
}
void CuteTorrent::ShowOpenTorrentDialog()
{
	QString lastDir = m_pSettings->valueString("System", "LastOpenTorrentDir", "");
	QString filename =  QFileDialog::getOpenFileName(this, tr("OPEN_TOORENT_DIALOG"),
	                    lastDir , tr("TORRENT_FILES (*.torrent);;Any File (*.*)"));

	if(!filename.isEmpty())
	{
		OpenTorrentDialog* dlg = new OpenTorrentDialog(this);
		m_pSettings->setValue("System", "LastOpenTorrentDir", filename);
		dlg->SetData(filename);
		dlg->execConditional();
		delete dlg;
	}
}
void CuteTorrent::EnableNitifyShow()
{
	mayShowNotifies = true;
}

void CuteTorrent::UpdateInfoTab()
{
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if(tor != NULL)
	{
		fillPieceDisplay(m_pPieceView->size());
		downloadedBytesLabel->setText(tor->GetTotalDownloaded());
		uploadedBytesLabel->setText(tor->GetTotalUploaded());
		downloadSpeedLabel->setText(tor->GetDwonloadSpeed());
		activetimeLabel->setText(tor->GetActiveTime());
		uploadSpeedLabel->setText(tor->GetUploadSpeed());
		pathLabel->setText(tor->GetSavePath());
		totalSizeLabel->setText(tor->GetTotalSize());
		seedCoutLabel->setText(tor->GetSeedString());
		peerCoutLabel->setText(tor->GetPeerString());
		QFontMetrics metrics(describtionLabel->font());
		QString elidedText = metrics.elidedText(tor->GetDiscribtion(), Qt::ElideRight, describtionLabel->width());
		describtionLabel->setText(elidedText);
		timeleftLabel->setText(tor->GetRemainingTime());
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
	}
}
void CuteTorrent::UpdatePeerTab()
{
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if(tor != NULL)
	{
		std::vector<peer_info> peerInfos = tor->GetPeerInfo();
		peerTableWidget->setRowCount(peerInfos.size());

		for(int i = 0; i < peerInfos.size(); i++)
		{
			QString client = QString::fromUtf8(peerInfos[i].client.c_str());

			if((peerInfos[i].flags & peer_info::rc4_encrypted) == peer_info::rc4_encrypted
			        || (peerInfos[i].flags & peer_info::plaintext_encrypted) == peer_info::plaintext_encrypted)
			{
				client = client.append(" *");
			}

			peerTableWidget->setItem(i, 0, new MyTableWidgetItem(QString::fromStdString(peerInfos[i].ip.address().to_string())));
			peerTableWidget->setItem(i, 1, new MyTableWidgetItem(client));
			peerTableWidget->setItem(i, 2, new MyTableWidgetItem(QString::number(peerInfos[i].progress_ppm / 10000.f, 'f', 1) + "%"));
			peerTableWidget->setItem(i, 3, new MyTableWidgetItem(StaticHelpers::toKbMbGb(peerInfos[i].down_speed) + "/s"));
			peerTableWidget->setItem(i, 4, new MyTableWidgetItem(StaticHelpers::toKbMbGb(peerInfos[i].up_speed) + "/s"));
			peerTableWidget->setItem(i, 5, new MyTableWidgetItem(StaticHelpers::toKbMbGb(peerInfos[i].total_download)));
			peerTableWidget->setItem(i, 6, new MyTableWidgetItem(StaticHelpers::toKbMbGb(peerInfos[i].total_upload)));
			peerTableWidget->setItem(i, 7, new MyTableWidgetItem(StaticHelpers::toKbMbGb(peerInfos[i].remote_dl_rate) + "/s"));
		}

		//peerTableWidget->resizeColumnsToContents();
	}
	else
	{
		peerTableWidget->setRowCount(0);
	}
}
void CuteTorrent::UpadteTrackerTab()
{
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if(tor != NULL)
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

		for(size_type i = 0; i < trackers.size(); i++)
		{
			QTableWidgetItem* rootItem = new QTableWidgetItem(trackers[i].url.c_str());
			trackerTableWidget->setItem(i + 3, 0, rootItem);
			QString stateStr;
			if (trackers[i].last_error && trackers[i].last_error.value() != libtorrent::errors::tracker_failure)
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
			trackerTableWidget->setItem(i + 3, 3, new QTableWidgetItem((peer_cnt > 0 ?  QString::number(peer_cnt) : "")));
		}
	}
	else
	{
		trackerTableWidget->setRowCount(0);
	}
}
void CuteTorrent::PauseSelected()
{
	m_pTorrentDisplayModel->ActionOnSelectedItem(QTorrentDisplayModel::pause);
}
void CuteTorrent::ResumeSelected()
{
	m_pTorrentDisplayModel->ActionOnSelectedItem(QTorrentDisplayModel::resume);
}
void CuteTorrent::DeleteSelected()
{
	m_pTorrentDisplayModel->ActionOnSelectedItem(QTorrentDisplayModel::remove);
}
void CuteTorrent::Retranslate()
{
	retranslateUi(this);
}
void CuteTorrent::OpenSettingsDialog()
{
	SettingsDialog* dlg = new SettingsDialog(this);
	QObject::connect(dlg, SIGNAL(needRetranslate()), this, SLOT(Retranslate()));
	dlg->exec();
	delete dlg;
	setupGroupTreeWidget();
	setupKeyMappings();
	UpdateTabWidget(-2);
}
void CuteTorrent::closeEvent(QCloseEvent* ce)
{
	ce->ignore();
	hide();
	QBalloonTip::showBalloon("CuteTorrent", tr("CT_HIDE_MSG"), QBalloonTip::Info, qVariantFromValue(0),
	                         QSystemTrayIcon::Information, 5000, false);
	setUpdatesEnabled(false);
	return;
}

void CuteTorrent::dragEnterEvent(QDragEnterEvent* event)
{
	if(event->mimeData()->hasFormat(QString("text/plain")) || event->mimeData()->hasFormat(QString("text/uri-list")))
	{
		event->acceptProposedAction();
	}
}

void CuteTorrent::dropEvent(QDropEvent* event)
{
	QStringList files;

	if(event->mimeData()->hasUrls())
	{
		const QList<QUrl> urls = event->mimeData()->urls();

		foreach(const QUrl& url, urls)
		{
			if(!url.isEmpty())
			{
				if(url.scheme().compare("file", Qt::CaseInsensitive) == 0)
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
		if(file.startsWith("magnet:", Qt::CaseInsensitive) || file.endsWith(".torrent", Qt::CaseInsensitive))
		{
			OpenTorrentDialog* dlg2 = new OpenTorrentDialog();
			dlg2->SetData(file);
			dlg2->exec();
			delete dlg2;
			event->acceptProposedAction();
		}
		else
		{
		}
	}
}
CuteTorrent::~CuteTorrent()
{
	m_pSettings->setValue("Window", "geometry", geometry());
	m_pSettings->setValue("Window", "maximized", isMaximized());
	m_pSettings->setValue("Window", "selected_tab", m_pTabWidget->currentIndex());
	m_pSettings->setValue("Window", "horizontal_sizes", QVariant::fromValue(spliiter1->sizes()));
	m_pSettings->setValue("Window", "vertical_sizes", QVariant::fromValue(spliiter->sizes()));
	QList<int> peer_columns_sizes;

	for(int i = 0; i < peerTableWidget->columnCount(); i++)
	{
		peer_columns_sizes.append(peerTableWidget->columnWidth(i));
	}

	m_pSettings->setValue("Window", "peers_sizes", QVariant::fromValue(peer_columns_sizes));
	QList<int> tracker_columns_sizes;

	for(int i = 0; i < trackerTableWidget->columnCount(); i++)
	{
		tracker_columns_sizes.append(trackerTableWidget->columnWidth(i));
	}

	m_pSettings->setValue("Window", "trackers_sizes", QVariant::fromValue(tracker_columns_sizes));
	QList<int> file_columns_sizes;

	for (int i = 0; i < fileTableView->model()->columnCount(); i++)
	{
		file_columns_sizes.append(fileTableView->columnWidth(i));
	}

	m_pSettings->setValue("Window", "files_sizes", QVariant::fromValue(file_columns_sizes));
	TorrentTracker::freeInstance();
	RconWebService::freeInstance();
	m_pTrayIcon->hide();
	TorrentManager::freeInstance();
	delete m_pSearchDisplayModel;
	delete m_pSearchEngine;
	delete m_pTorrentDisplayModel;
	delete m_pSearchItemDelegate;
	delete m_pTorrentItemDelegate;
	delete m_pRssDisplayModel;
	delete m_pRssItemDelegate;
	Scheduller::freeInstance();
	QApplicationSettings::FreeInstance();
	delete m_pUpdateNotifier;
	delete m_pTrayIcon;
}

void CuteTorrent::setupFileTabel()
{
	m_pFileViewProxymodel = new FileViewSortProxyModel(this);
	fileTableView->setModel(m_pFileViewProxymodel);
	m_pFileViewModel = new FileViewModel(fileTableView, this);
	m_pFileViewProxymodel->setSourceModel(m_pFileViewModel);
	
	fileTableView->setItemDelegateForColumn(1, new FileSizeItemDelegate(this));
	fileTableView->setItemDelegateForColumn(2, new ProgressItemDelegate(this));
	fileTableView->setItemDelegateForColumn(3, new PriorityItemDelegate(this));
	QList<int> file_column_sizes = m_pSettings->value("Window", "files_sizes").value<QList<int>>();

	if(file_column_sizes.count() > 0)
	{
		for(int i = 0; i < file_column_sizes.count(); i++)
		{
			fileTableView->setColumnWidth(i, file_column_sizes.at(i));
		}
	}
	else
	{
		fileTableView->setColumnWidth(0, 480);
		fileTableView->setColumnWidth(1, 65);
		fileTableView->setColumnWidth(2, 65);
		fileTableView->setColumnWidth(3, 70);
	}

	fileTableView->header()->setDefaultAlignment(Qt::AlignCenter);
	fileTableView->header()->setClickable(true);
}


void CuteTorrent::ProcessMagnet()
{
	bool ok;
	QString magnetLink = QInputDialog::getText(this, tr("MAGNET_LINK_DLG"), tr("MAGNET_LINK:"), QLineEdit::Normal, "", &ok);

	if(ok && !magnetLink.isEmpty())
	{
		OpenTorrentDialog* dlg2 = new OpenTorrentDialog();
		dlg2->SetData(magnetLink);
		dlg2->exec();
		dlg2->deleteLater();
	}
}

void CuteTorrent::PeformSearch()
{
	QString searchText = m_pTorrentSearchEdit->text().trimmed();

	if (!searchText.isEmpty())
	{
		ISerachProvider::SearchCategories category;

		if (m_pTorrentListView->model() == m_pTorrentDisplayModel)
		{
			category = (ISerachProvider::SearchCategories)m_pTorrentSearchCategory->itemData(m_pTorrentSearchCategory->currentIndex()).toInt();
		}
		else
		{
			category = (ISerachProvider::SearchCategories)m_pSearchCategory->itemData(m_pSearchCategory->currentIndex()).toInt();
		}

		m_pSearchEngine->DoSerach(searchText, category, 1);
		m_pGroupTreeWidget->setCurrentItem(searchTreeItem);
	}
}

void CuteTorrent::resizeEvent(QResizeEvent* event)
{
	fillPieceDisplay(m_pPieceView->size());
	QTorrentItemDelegat::max_width = width() - QApplication::style()->pixelMetric(QStyle::PM_MessageBoxIconSize) - 35 - (m_pTorrentListView->verticalScrollBar()->isVisible() ?
	                                 m_pTorrentListView->autoScrollMargin() : 0) - m_pGroupTreeWidget->width();
	QSearchItemDelegate::max_width = width() - QApplication::style()->pixelMetric(QStyle::PM_MessageBoxIconSize) - 35 - (m_pTorrentListView->verticalScrollBar()->isVisible() ?
	                                 m_pTorrentListView->autoScrollMargin() : 0) - m_pGroupTreeWidget->width();
}

void CuteTorrent::ShowTorrentInfoNotyfy(const QString& name, const QString& info)
{
#ifndef Q_WS_MAC
	QBalloonTip::showBalloon("CuteTorrent", QString("%1\n%2").arg(name).arg(info), QBalloonTip::Info, qVariantFromValue(0),
	                         QSystemTrayIcon::Information, 15000, false);
#else
	m_pTrayIcon->showMessage("CuteTorrent", QString("%1\n%2").arg(name).arg(info), QSystemTrayIcon::Information);
#endif
}

void CuteTorrent::keyPressEvent(QKeyEvent* event)
{
	if(!isVisible())
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
		if(QKeySequence(keyMap[key].toString()) == pressedKey)
		{
			QAction* action = this->findChild<QAction*> (key);

			if(action != NULL)
				// qDebug() << "Matched action:" << key;
			{
				action->activate(QAction::Trigger);
			}
		}
	}
}

void CuteTorrent::fillPieceDisplay(QSize size)
{
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if(tor != NULL)
	{
		int piece_count = tor->GetPieceCount();
		QVector<int> avaliablePieces = tor->GetDownloadedPieces();
		QVector<int> dwonloadingPieces = tor->GetDownloadingPieces();
		m_pPieceView->resize(size);
		m_pPieceView->setDowloadedParts(avaliablePieces);
		m_pPieceView->setDowloadingParts(dwonloadingPieces);
		m_pPieceView->setPiceCount(piece_count);
		m_pPieceView->update();
	}
}

void CuteTorrent::CopyDiscribtion()
{
	QClipboard* clipboard = QApplication::clipboard();
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if(tor != NULL)
	{
		clipboard->setText(tor->GetDiscribtion());
	}
}

void CuteTorrent::ClearPieceDisplay()
{
	m_pPieceView->setPiceCount(0);
	/*QGraphicsScene *scene = new QGraphicsScene(this);
	scene->clear();
	piceDwonloadedView->scene()->deleteLater();
	piceDwonloadedView->setScene(scene);
	piceDwonloadedView->show();*/
}

void CuteTorrent::UpdateUL(int kbps)
{
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if(tor != NULL)
	{
		tor->SetUlLimit(kbps * 1024);
	}
	else
	{
		//settings = QApplicationSettings::getInstance();
		m_pSettings->setValue("Torrent", "upload_rate_limit", kbps * 1024);
		//QApplicationSettings::FreeInstance();
		m_pTorrentManager->SetUlLimit(kbps * 1024);
	}
}

void CuteTorrent::UpdateDL(int kbps)
{
	Torrent* tor = m_pTorrentDisplayModel->GetSelectedTorrent();

	if(tor != NULL)
	{
		tor->SetDlLimit(kbps * 1024);
	}
	else
	{
		//	QApplicationSettings* settings = QApplicationSettings::getInstance();
		m_pSettings->setValue("Torrent", "download_rate_limit", kbps * 1024);
		//	QApplicationSettings::FreeInstance();
		m_pTorrentManager->SetDlLimit(kbps * 1024);
	}
}

void CuteTorrent::AddPeer()
{
	Torrent* torrent = m_pTorrentDisplayModel->GetSelectedTorrent();

	if(torrent != NULL)
	{
		bool ok;
		QString peerStr = QInputDialog::getText(this, tr("ADD_PEER_DLG"), tr("PEER:"), QLineEdit::Normal, "", &ok);

		if(ok && !peerStr.isEmpty())
		{
			QStringList parts = peerStr.split(':');

			if(parts.count() == 2)
			{
				QHostAddress adr(parts[0]);

				if(!adr.isNull())
				{
					torrent->AddPeer(adr, parts[1].toUInt());
					return;
				}
			}

			MyMessageBox::critical(this, tr("PEER_ERR"), tr("INVALID_IP_STRING"));
		}
	}
}

void CuteTorrent::AddTracker()
{
	Torrent* torrent = m_pTorrentDisplayModel->GetSelectedTorrent();

	if(torrent != NULL)
	{
		bool ok;
		QString trackerUrl = QInputDialog::getText(this, tr("ADD_TRACKER_DLG"), tr("TRACKER:"), QLineEdit::Normal, "", &ok);

		if(ok && !trackerUrl.isEmpty())
		{
			torrent->AddTracker(trackerUrl);
		}
	}
}

void CuteTorrent::StopSelected()
{
	m_pTorrentDisplayModel->ActionOnSelectedItem(QTorrentDisplayModel::stop);
}

void CuteTorrent::setupGroupTreeWidget()
{
	m_pGroupTreeWidget->clear();
	QIcon torrentsIcon = m_pStyleEngine->getIcon("torrents");
	QIcon downloadingIcon = m_pStyleEngine->getIcon("downloading");
	QIcon seedingIcon = m_pStyleEngine->getIcon("uploading");
	QIcon completedIcon = m_pStyleEngine->getIcon("completed");
	QIcon activeIcon = m_pStyleEngine->getIcon("active");
	QIcon groupsIcon = m_pStyleEngine->getIcon("groups");
	QIcon inactiveIcon(activeIcon.pixmap(QSize(16, 16), QIcon::Disabled, QIcon::On));
	QIcon rssIcon = m_pStyleEngine->getIcon("rss");
	torrentTreeItem = new QTreeWidgetItem(m_pGroupTreeWidget);
	torrentTreeItem->setIcon(0, torrentsIcon);
	torrentTreeItem->setText(0, tr("TORRENTS_ACTIVITY"));
	torrentTreeItem->setData(0, Qt::UserRole + 1, TORRENT);
	torrentTreeItem->setData(0, Qt::UserRole + 2, EMPTY);
	dlTreeItem = new QTreeWidgetItem(torrentTreeItem);
	dlTreeItem->setIcon(0, downloadingIcon);
	dlTreeItem->setText(0, tr("DOWNLOADING_FLTR"));
	dlTreeItem->setData(0, Qt::UserRole + 1, TORRENT);
	dlTreeItem->setData(0, Qt::UserRole + 2, DOWNLOADING);
	ulTreeItem = new QTreeWidgetItem(torrentTreeItem);
	ulTreeItem->setIcon(0, seedingIcon);
	ulTreeItem->setText(0, tr("SEEDING_FLTR"));
	ulTreeItem->setData(0, Qt::UserRole + 1, TORRENT);
	ulTreeItem->setData(0, Qt::UserRole + 2, SEEDING);
	completedTreeItem = new QTreeWidgetItem(torrentTreeItem);
	completedTreeItem->setIcon(0, completedIcon);
	completedTreeItem->setText(0, tr("COMPLETED_FLTR"));
	completedTreeItem->setData(0, Qt::UserRole + 1, TORRENT);
	completedTreeItem->setData(0, Qt::UserRole + 2, COMPLETED);
	activeTreeItem = new QTreeWidgetItem(torrentTreeItem);
	activeTreeItem->setIcon(0, activeIcon);
	activeTreeItem->setText(0, tr("ACTIVE_FLTR"));
	activeTreeItem->setData(0, Qt::UserRole + 1, TORRENT);
	activeTreeItem->setData(0, Qt::UserRole + 2, ACTIVE);
	inactiveTreeItem = new QTreeWidgetItem(torrentTreeItem);
	inactiveTreeItem->setIcon(0, inactiveIcon);
	inactiveTreeItem->setText(0, tr("NOT_ACTIVE_FLTR"));
	inactiveTreeItem->setData(0, Qt::UserRole + 1, TORRENT);
	inactiveTreeItem->setData(0, Qt::UserRole + 2, NOT_ACTIVE);
	groupsTreeItem = new QTreeWidgetItem(m_pGroupTreeWidget);
	groupsTreeItem->setText(0, tr("TORRENT_GROUPS"));
	groupsTreeItem->setData(0, Qt::UserRole + 1, GROUP_FILTER_TYPE);
	groupsTreeItem->setIcon(0, groupsIcon);
	QList<GroupForFileFiltering> groups = m_pSettings->GetFileFilterGroups();
	QString type;

	for(int i = 0; i < groups.count(); i++)
	{
		QTreeWidgetItem* item = new QTreeWidgetItem(groupsTreeItem);
		item->setText(0, groups[i].Name());
		item->setIcon(0, m_pStyleEngine->guessMimeIcon(groups[i].Extensions().split('|') [0], type));
		item->setData(0, Qt::UserRole + 1, GROUP_FILTER_TYPE);
		item->setData(0, Qt::UserRole + 2, groups[i].Name());
	}

	rssTreeItem = new QTreeWidgetItem(m_pGroupTreeWidget);
	rssTreeItem->setIcon(0, rssIcon);
	rssTreeItem->setText(0, tr("RSS_CHANELS"));
	rssTreeItem->setData(0, Qt::UserRole + 1, RSS_FEED);
	searchTreeItem = new QTreeWidgetItem(m_pGroupTreeWidget);
	searchTreeItem->setIcon(0, m_pStyleEngine->getIcon("settings_search"));
	searchTreeItem->setText(0, tr("TAB_SEARCH"));
	searchTreeItem->setData(0, Qt::UserRole + 1, SEARCH);
	searchTreeItem->setData(0, Qt::UserRole + 2, "");
	QList<ISerachProvider*>  searchProviders = m_pSearchEngine->GetSearchProviders();
	int nSearchProvidersCount = searchProviders.length();

	for(int i = 0; i < nSearchProvidersCount; i++)
	{
		ISerachProvider* current = searchProviders.at(i);
		QTreeWidgetItem* item = new QTreeWidgetItem(searchTreeItem);
		QString engineName = current->Name();
		item->setText(0, engineName);
		item->setIcon(0, m_pStyleEngine->getIcon(engineName));
		item->setData(0, Qt::UserRole + 1, SEARCH);
		item->setData(0, Qt::UserRole + 2, engineName);
	}

	m_pGroupTreeWidget->resizeColumnToContents(0);
	m_pGroupTreeWidget->expandAll();
}


void CuteTorrent::ChnageTorrentFilter()
{
	QTreeWidgetItem* item = m_pGroupTreeWidget->currentItem();
	FilterType filterType = (FilterType) item->data(0, Qt::UserRole + 1).toInt();
	m_pTorrentListView->selectionModel()->reset();
	switch(filterType)
	{
		case GROUP_FILTER_TYPE:
		{
			QString gropupFilter = item->data(0, Qt::UserRole + 2).toString();
			m_pTorrentFilterProxyModel->setGroupFilter(gropupFilter);
			switchToTorrentsModel();
			break;
		}

		case SEARCH:
		{
			QString engineName = item->data(0, Qt::UserRole + 2).toString();
			m_pSearchEngine->GetResults()->setFilter(engineName);
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
			TorrentFilterType torrentFilter = (TorrentFilterType) item->data(0, Qt::UserRole + 2).toInt();
			m_pTorrentFilterProxyModel->setTorrentFilter(torrentFilter);
			switchToTorrentsModel();
			break;
		}

		case NONE:
			break;
	}

	m_pTorrentDisplayModel->updateVisibleTorrents();
}

void CuteTorrent::startBackUpWizard()
{
	BackupWizard* wizard = new BackupWizard(this);
	wizard->show();
}
void CuteTorrent::initMainMenuIcons()
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

void CuteTorrent::initWindowIcons()
{
	pbMin->setIcon(m_pStyleEngine->getIcon("app_min"));
	pbMax->setIcon(m_pStyleEngine->getIcon("app_max"));
	pbClose->setIcon(m_pStyleEngine->getIcon("app_close"));
	setupGroupTreeWidget();
	initToolbarIcons();
	initStatusBarIcons();
	initMainMenuIcons();
}
void CuteTorrent::setupCustomeWindow()
{
	setupUi(this);
	BaseWindow::setupCustomWindow();
	QRect geo = m_pSettings->value("Window", "geometry", QRect(0, 0, 683, 643)).toRect();

	if(geo.height() > 0 && geo.x() < QApplication::desktop()->width() && geo.width() > 0 && geo.y() < QApplication::desktop()->height())
	{
		setGeometry(geo);
	}

	if(m_pSettings->value("Window", "maximized", false).toBool())
	{
		showMaximized();
		pbMax->setIcon(m_pStyleEngine->getIcon("app_rest"));
	}

	int selectedTab = m_pSettings->valueInt("Window", "selected_tab", 0);

	if(selectedTab >= 0 && selectedTab < m_pTabWidget->count())
	{
		m_pTabWidget->setCurrentIndex(selectedTab);
	}

	QList<int> horizontal_sizes = m_pSettings->value("Window", "horizontal_sizes", qVariantFromValue(QList<int>() << 130 << 538)).value<QList<int>>();

	if(horizontal_sizes.size() > 0)
	{
		spliiter1->setSizes(horizontal_sizes);
	}

	QList<int> vertical_sizes = m_pSettings->value("Window", "vertical_sizes", qVariantFromValue(QList<int>() << 530 << 195)).value<QList<int>>();

	if(vertical_sizes.size() > 0)
	{
		spliiter->setSizes(vertical_sizes);
	}
}

void CuteTorrent::setupKeyMappings()
{
	QList<QAction*> actions = this->findChildren<QAction*>();
	QMap<QString, QVariant> keyMap = m_pSettings->getGroupValues("KeyMap");

	foreach(QAction* action, actions)
	{
		QString objName = action->objectName();

		if(!objName.isEmpty() && objName.startsWith("ACTION_") && !keyMap.contains(objName))
		{
			keyMap.insert(action->objectName(), action->shortcut().toString());
		}
		else
		{
			if(objName.startsWith("ACTION_") && keyMap.contains(objName))
			{
				action->setShortcut(QKeySequence(keyMap[objName].toString()));
			}
		}
	}

	m_pSettings->setGroupValues("KeyMap", keyMap);
	qDebug() << keyMap;
}


void CuteTorrent::resizeWindow(QMouseEvent* e)
{
	BaseWindow::resizeWindow(e);
	m_pPieceView->update();
}

QPushButton* CuteTorrent::getMinBtn()
{
	return pbMin;
}

QPushButton* CuteTorrent::getMaxBtn()
{
	return pbMax;
}

QPushButton* CuteTorrent::getCloseBtn()
{
	return pbClose;
}

QWidget* CuteTorrent::getTitleBar()
{
	return titleBar;
}

QWidget* CuteTorrent::centralWidget()
{
	return m_centralWidget;
}

QLabel* CuteTorrent::getTitleLabel()
{
	return LTitle;
}

QLabel* CuteTorrent::getTitleIcon()
{
	return tbMenu;
}

void CuteTorrent::maximizeBtnClicked()
{
	BaseWindow::maximizeBtnClicked();
}

void CuteTorrent::minimizeBtnClicked()
{
	BaseWindow::minimizeBtnClicked();
}

void CuteTorrent::OnGotSerachResults()
{
	//ToDo: implement showing of search results
}

bool CuteTorrent::eventFilter(QObject* obj, QEvent* event)
{
	switch (event->type())
	{
		case QEvent::MouseButtonDblClick:
		{
			mouseMoveEvent((static_cast<QMouseEvent*>(event)));
			return true;
		}

		case QEvent::MouseButtonRelease:
		{
			mouseMoveEvent((static_cast<QMouseEvent*>(event)));
			return true;
		}

		case QEvent::MouseButtonPress:
		{
			mouseMoveEvent((static_cast<QMouseEvent*>(event)));
			return true;
		}

		case QEvent::MouseMove:
		{
			mouseMoveEvent((static_cast<QMouseEvent*>(event)));
			return true;
		}

		case QEvent::Resize:
		{
			resizeEvent(static_cast<QResizeEvent*>(event));
			return QObject::eventFilter(obj, event);
		}

		default:
		{
			return QObject::eventFilter(obj, event);
		}
	}
}

void CuteTorrent::switchToTorrentsModel()
{
	if (m_pTorrentListView->model() != m_pTorrentFilterProxyModel)
	{
		m_pTorrentListView->setModel(m_pTorrentFilterProxyModel);
		m_pTorrentDisplayModel->Update();
		m_pTorrentListView->setItemDelegate(m_pTorrentItemDelegate);
		m_pToolBarsContainer->setCurrentIndex(0);
		m_pInfoPlaneContainer->setCurrentIndex(0);
		QObject::connect(m_pTorrentListView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
		                 m_pTorrentDisplayModel, SLOT(UpdateSelectedIndex(const QItemSelection&)));
		m_pTorrentSearchCategory->setCurrentIndex(m_pSearchCategory->currentIndex());
	}
}

void CuteTorrent::switchToSearchModel()
{
	if(m_pTorrentListView->model() != m_pSearchDisplayModel)
	{
		QObject::disconnect(m_pTorrentListView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
		                    m_pTorrentDisplayModel, SLOT(UpdateSelectedIndex(const QItemSelection&)));
		m_pTorrentListView->setModel(m_pSearchDisplayModel);
		m_pTorrentListView->setItemDelegate(m_pSearchItemDelegate);
		m_pToolBarsContainer->setCurrentIndex(1);
		m_pSearchCategory->setCurrentIndex(m_pTorrentSearchCategory->currentIndex());
		m_pInfoPlaneContainer->setCurrentIndex(0);
	}
}

void CuteTorrent::switchToRssModel()
{
	if (m_pTorrentListView->model() != m_pRssDisplayModel)
	{
		QObject::disconnect(m_pTorrentListView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
			m_pTorrentDisplayModel, SLOT(UpdateSelectedIndex(const QItemSelection&)));
		m_pTorrentListView->setModel(m_pRssDisplayModel);
		m_pTorrentListView->setItemDelegate(m_pRssItemDelegate);
		QObject::connect(m_pTorrentListView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
			this, SLOT(UpdateRssInfo(const QItemSelection&)));
		m_pToolBarsContainer->setCurrentIndex(2);
		m_pInfoPlaneContainer->setCurrentIndex(1);
	}
}

void CuteTorrent::openSearchItemDescribtion()
{
	if (m_pTorrentListView->model() != m_pSearchDisplayModel)
	{
		return;
	}

	QModelIndex index = m_pTorrentListView->currentIndex();
	SearchResult* searchResult = index.data(QSearchDisplayModel::SearchItemRole).value<SearchResult*>();
	QDesktopServices::openUrl(searchResult->TorrentDescUrl);
}

void CuteTorrent::startDownloadTorrent()
{
	if (m_pTorrentListView->model() != m_pSearchDisplayModel)
	{
		return;
	}

	QModelIndex index = m_pTorrentListView->currentIndex();
	SearchResult* searchResult = index.data(QSearchDisplayModel::SearchItemRole).value<SearchResult*>();
	QDesktopServices::openUrl(searchResult->TorrentFileUrl);
}

void CuteTorrent::RemoveTracker()
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
		for each (QTableWidgetSelectionRange selection in selectedRows)
		{
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

void CuteTorrent::EditTracker()
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

void CuteTorrent::AddWebSeed()
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

void CuteTorrent::addRssFeed()
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
			m_pRssDisplayModel->UpdateModel();
		}
	}
}

void CuteTorrent::removeRssFeed()
{
	if (m_pTorrentListView->model() == m_pRssDisplayModel)
	{
		
		QList<RssFeed*> selectedItems = m_pRssDisplayModel->SelectedFeeds();
		for each (RssFeed* feed in selectedItems)
		{
//			m_pTorrentManager->RemoveFeed(feed);
		}
	}
}

void CuteTorrent::editRssFeed()
{
	if (m_pTorrentListView->model() == m_pRssDisplayModel)
	{
		RssFeed* selecteFeed = m_pRssDisplayModel->SelectedIFeed();
		if (selecteFeed != NULL)
		{
			RssFeedSettingsDialog* pDialog = new RssFeedSettingsDialog(this);
			pDialog->SetFeed(selecteFeed);
			pDialog->exec();
			delete pDialog;
		}
		
	}
}

void CuteTorrent::OnQuit()
{
	if (m_pTorrentManager != NULL)
	{
		m_pTorrentManager->SaveSession();
	}
	RssManagerPtr pRssManager = RssManager::getInstance();
	pRssManager->SaveFeeds();
}

void CuteTorrent::UpdateRssInfo(const QItemSelection& selection)
{
	if (m_pTorrentListView->model() != m_pRssDisplayModel)
	{
		return;
	}

	RssItem currentItem = m_pRssDisplayModel->SelectedFeedItem();
	if (!currentItem.isEmpty())
	{
		m_pFeedItemCommentEdit->setText(currentItem["comment"].toString());
		m_pFeedItemDescribtionEdit->setHtml(currentItem["description"].toString());
	}
	else
	{
		m_pFeedItemCommentEdit->setText("");
		m_pFeedItemDescribtionEdit->setText("");
	}
}

void CuteTorrent::setupRssInfoTab()
{
	m_pFeedItemDescribtionEdit = new HtmlView(this);
	m_pFeedItemCommentEdit = new QLabel(this);
	m_pRssInfoLayout->addWidget(m_pFeedItemDescribtionEdit, 0, 1, 1, 1);
	m_pRssInfoLayout->addWidget(m_pFeedItemCommentEdit, 1, 1, 1, 1);
}
