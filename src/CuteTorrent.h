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
#ifndef LTORRENT_H
#define LTORRENT_H

#include <qtsingleapplication.h>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QEvent>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QMutex>
#include <QScrollBar>
#include <QSpinBox>
#include <QString>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QtGui/QMainWindow>
#include <QtNetwork/QHostAddress>

#include "CreateTorrentDialog.h"
#include "CustomWindow.h"
#include "FileViewModel.h"
#include "FileViewSortProxyModel.h"
#include "OpenTorrentDialog.h"
#include "QSearchDisplayModel.h"
#include "QSearchItemDelegate.h"
#include "QTorrentDisplayModel.h"
#include "QTorrentItemDelegat.h"
#include "SearchEngine.h"
#include "SettingsDialog.h"
#include "StyleEngene.h"
#include "TorrentManager.h"
#include "TorrentStorrage.h"
#include "UpdateNotyfier.h"
#include "application.h"
#include "peicedisplaywidget.h"
#include "searchitem.h"
#include "tracker/torrentracker.h"
#include "ui_CustomWindow.h"
#include "webControll/RconWebService.h"

class Application;
class FileViewModel;
class FileViewSortProxyModel;
class PeiceDisplayWidget;
class QApplicationSettings;
class QSearchDisplayModel;
class QSearchItemDelegate;
class QTorrentDisplayModel;
class QTorrentFilterProxyModel;
class QRssDisplayModel;
class QRssItemDelegate;
class RconWebService;
class SearchEngine;
class StyleEngene;
class TorrentManager;
class TorrentStorrage;
class TorrentTracker;
class UpdateNotifier;
class HtmlView;
class CuteTorrent : public BaseWindow<QWidget> , private Ui::CustomWindow
{
	Q_OBJECT
public:
	CuteTorrent(QWidget* parent = 0);
	void ConnectMessageReceved(Application* a);
	~CuteTorrent();
protected:
	bool eventFilter(QObject* obj, QEvent* event);
	void changeEvent(QEvent* event);
	void resizeEvent(QResizeEvent* event);
	void closeEvent(QCloseEvent* ce);
	void dragEnterEvent(QDragEnterEvent* event);
	void dropEvent(QDropEvent* event);
	void keyPressEvent(QKeyEvent* event);

private:
	void initToolbarIcons();
	void initStatusBarIcons();
	void initMainMenuIcons();
	StyleEngene* m_pStyleEngine;
	QList<SearchItem> searchSources;
	QComboBox* m_pTorrentSearchCategory;
	QComboBox* m_pSearchCategory;
	TorrentStorrage* m_pTorrents;
	PeiceDisplayWidget* m_pPieceView;
	FileViewSortProxyModel* m_pFileViewProxymodel;
	FileViewModel* m_pFileViewModel;
	TorrentTracker* m_pTracker;
	QApplicationSettings* m_pSettings;
	RconWebService* m_pRcon;
	UpdateNotifier* m_pUpdateNotifier;
	HtmlView* m_pFeedItemDescribtionEdit;
	QLabel* m_pFeedItemCommentEdit;
	bool mayShowNotifies;
	QSystemTrayIcon* m_pTrayIcon;
	QSpinBox* ul, *dl;
	QMenu* trayIconMenu;
	QAction* minimizeAction;
	QAction* maximizeAction;
	QAction* restoreAction;
	QAction* quitAction;
	QAction* copyContext;
	QAction* addPeer, * addWebSeed, * addTracker, * removeTracker, * editTracker;
	QLabel* upLabelText, *upLabel, *title, *dhtNodesLabel;
	QLabel* uploadLimit, *downloadLimit;
	QLabel* downLabelText, *downLabel;
	QRssDisplayModel* m_pRssDisplayModel;
	QRssItemDelegate* m_pRssItemDelegate;
	QTorrentFilterProxyModel* m_pTorrentFilterProxyModel;
	QTorrentDisplayModel* m_pTorrentDisplayModel;
	QTorrentItemDelegat* m_pTorrentItemDelegate;
	QSearchDisplayModel* m_pSearchDisplayModel;
	QSearchItemDelegate* m_pSearchItemDelegate;
	TorrentManager* m_pTorrentManager;
	QLineEdit* m_pTorrentSearchEdit;
	QLineEdit* m_pSearchEdit;
	QTreeWidgetItem* torrentTreeItem;
	QTreeWidgetItem* dlTreeItem;
	QTreeWidgetItem* ulTreeItem;
	QTreeWidgetItem* completedTreeItem;
	QTreeWidgetItem* activeTreeItem;
	QTreeWidgetItem* inactiveTreeItem;
	QTreeWidgetItem* groupsTreeItem;
	QTreeWidgetItem* rssTreeItem;
	QTreeWidgetItem* searchTreeItem;
	SearchEngine* m_pSearchEngine;
	void createTrayIcon();
	void createActions();
	void setupTray();
	void setupToolBar();
	void setupConnections();
	void setupListView();
	void setupTabelWidgets();
	void setupStatusBar();
	void setupRssInfoTab();
	void setupFileTabel();
	void setupGroupTreeWidget();
	void fillPieceDisplay(QSize);
	void setupCustomeWindow();
	void setupKeyMappings();
	void switchToTorrentsModel();
	void switchToSearchModel();
	void switchToRssModel();
	void resizeWindow(QMouseEvent* e);
	virtual QPushButton* getMinBtn() override;
	virtual QPushButton* getMaxBtn() override;
	virtual QPushButton* getCloseBtn() override;
	virtual QWidget* getTitleBar() override;
	virtual QWidget* centralWidget() override;
	virtual QLabel* getTitleLabel() override;
	virtual QLabel* getTitleIcon() override;
public slots:
	void HandleNewTorrent(const QString&);
	void UpdateUL(int);
	void UpdateDL(int);
	void ShowAbout();
	void CheckForUpdates();
	void ShowUpdateNitify(const QString&);
	void ShowNoUpdateNitify(const QString&);
	void Retranslate();
	void ShowTorrentError(const QString&, const QString&);
	void EnableNitifyShow();
	void ShowTorrentCompletedNotyfy(const QString&, const QString&);
	void ShowTorrentInfoNotyfy(const QString&, const QString&);
	void ShowCreateTorrentDialog();
	void ShowOpenTorrentDialog();
	void PauseSelected();
	void StopSelected();
	void ResumeSelected();
	void DeleteSelected();
	void UpdateInfoTab();
	void UpdatePeerTab();
	void UpdateFileTab();
	void UpadteTrackerTab();
	void OpenSettingsDialog();
	void IconActivated(QSystemTrayIcon::ActivationReason reason);
	void UpdateTabWidget(int);
	void ProcessMagnet();
	void PeformSearch();
	void CopyDiscribtion();
	void ClearPieceDisplay();
	void initWindowIcons();
	void AddTracker();
	void RemoveTracker();
	void EditTracker();
	void AddPeer();
	void AddWebSeed();
	void ChnageTorrentFilter();
	void startBackUpWizard();
	void OnGotSerachResults();
	void maximizeBtnClicked();
	void minimizeBtnClicked();
	void startDownloadTorrent();
	void openSearchItemDescribtion();
	void addRssFeed();
	void removeRssFeed();
	void editRssFeed();
	void OnQuit();
	void UpdateRssInfo(const QItemSelection&);
};

Q_DECLARE_METATYPE(QHostAddress)

#endif // LTORRENT_H
