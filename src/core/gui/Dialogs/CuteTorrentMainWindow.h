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

#include "CreateTorrentDialog.h"
#include "CustomWindow.h"
#include "FileViewModel.h"
#include "FileViewSortProxyModel.h"
#include "QSearchDisplayModel.h"
#include "QSearchItemDelegate.h"
#include "QTorrentDisplayModel.h"
#include "QTorrentItemDelegat.h"
#include "SearchEngine.h"
#include "StyleEngene.h"
#include "TorrentManager.h"
#include "TorrentStorrage.h"
#include "UpdateNotyfier.h"
#include "application.h"
#include "peicedisplaywidget.h"
#include "torrentracker.h"
#include "ui_CustomWindow.h"
#include "RconWebService.h"


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
class QWinJumpList;
class CuteTorrentMainWindow : public BaseWindow<QWidget> , private Ui::CustomWindow
{
	Q_OBJECT

public:
	CuteTorrentMainWindow(QWidget* parent = 0);
	void ConnectMessageReceved(Application* a);
	void RaiseWindow();
	~CuteTorrentMainWindow();
protected:
	bool eventFilter(QObject* obj, QEvent* event) override;
	void changeEvent(QEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;
	void closeEvent(QCloseEvent* ce) override;
	void dragEnterEvent(QDragEnterEvent* event) override;
	void dropEvent(QDropEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;

private:
	void initToolbarIcons();
	void initStatusBarIcons();
	void initMainMenuIcons();
	QRegExp m_httpLinkRegexp;
	QTimer* m_pUpdateTimer;
	StyleEngene* m_pStyleEngine;
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
	QWinJumpList* m_pJumpList;
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
	void setupTasksCategory();
	void setupJumpList();
	void fillPieceDisplay(QSize);
	void setupCustomeWindow();
	void setupKeyMappings();
	void switchToTorrentsModel();
	void switchToSearchModel();
	void switchToRssModel();
	void resizeWindow(QMouseEvent* e) override;
	void saveWindowState();
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
	void Retranslate();
	void ShowCreateTorrentDialog(QString path = "");
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
	void UpdateTabWidget();
	void UpdateStatusbar();
	void UpdateLimits();
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
	void maximizeBtnClicked() override;
	void minimizeBtnClicked() override;
	void startDownloadTorrent();
	void openSearchItemDescribtion();
	void addRssFeed();
	void removeRssFeed();
	void editRssFeed();
	void OnQuit();
	void UpdateRssInfo(const QItemSelection&);
	void OnMessageRecived(QString message);
};

Q_DECLARE_METATYPE(QHostAddress)

#endif // LTORRENT_H
