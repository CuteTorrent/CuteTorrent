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
#include "StyleEngene.h"
#include "TorrentManager.h"
#include "UpdateNotyfier.h"
#include "application.h"
#include "peicedisplaywidget.h"
#include "torrentracker.h"
#include "ui_CustomWindow.h"
#include "RconCommon.h"
#include "SearchCommon.h"
#include <viewModel/FiltersViewModel.h>
#include <gui/Controls/EditableHeaderView.h>
#include <viewmodels/QSearchFilterModel.h>
#include "TorrentGroupsManager.h"
class CommentsModel;
class CommentItemDelegate;
class RatingWidget;
class OnlineReporter;
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
class StyleEngene;
class TorrentManager;
class TorrentStorrage;
class TorrentTracker;
class UpdateNotifier;
class HtmlView;
class QWinJumpList;
class PeerTableModel;
class PieceAvailabilityWidget;
class QWinTaskbarButton;
class ProgressItemDelegate;
class FileSizeItemDelegate;
class SpeedItemDelegate;
class IpItemDelegate;
class PriorityItemDelegate;
class SortButton;
class CuteTorrentMainWindow : public BaseWindow<QWidget> , Ui::CustomWindow
{
	Q_OBJECT

public:


	CuteTorrentMainWindow(QWidget* parent = 0);
	void ConnectMessageReceved(Application* a) const;
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
	void setupRatingTab();
	void setupItemDelegates();
	void initToolbarIcons() const;
	void initStatusBarIcons() const;
	void initMainMenuIcons() const;
	bool m_initFinished;
	QWinTaskbarButton* m_pTaskBarBtn;
	EditableHeaderView* m_pTrackersHeader;
	EditableHeaderView* m_pPeersHeader;
	EditableHeaderView* m_pFilesHeader;
	QStandardItemModel* m_torrentSortersModel;
	QStandardItemModel* m_searchSorterModel;
	QSearchFilterModel* m_pSearchFilterModel;
	QRegExp m_httpLinkRegexp;
	QTimer* m_pUpdateTimer;
	StyleEngene* m_pStyleEngine;
	QComboBox* m_pSearchCategory;
	EditableHeaderView* m_pTorrentHeader;
	TorrentStorragePtr m_pTorrents;
	PeiceDisplayWidget* m_pPieceView;
	FileViewSortProxyModel* m_pFileViewProxymodel;
	NotificationSystemPtr m_pNotificationSystem;
	FileViewModel* m_pFileViewModel;
	TorrentTrackerPtr m_pTracker;
	QApplicationSettingsPtr m_pSettings;
	RconWebServicePtr m_pRcon;
	UpdateNotifier* m_pUpdateNotifier;
	HtmlView* m_pFeedItemDescribtionEdit;
	QSystemTrayIcon* m_pTrayIcon;
	PieceAvailabilityWidget* m_pieceAvalibilityWidget;
	QMenu* trayIconMenu;
	QAction* minimizeAction;
	QAction* maximizeAction;
	QAction *restoreAction, *pauseAll, *restoreAll;
	QAction* quitAction;
	bool m_ignoreSortByIndexChange;
	QAction *copyContext, *copyInfoHash;
	QAction *addPeer, *addWebSeed, *addTracker, *removeTracker, *editTracker, *updateTracker;
	QLabel *upLabelText, *upLabel, *title, *dhtNodesLabel;
	//	QLabel* uploadLimit, *downloadLimit;
	CommentsModel* m_pCommentsModel;
	CommentItemDelegate* m_pCommentItemDelegate;
	QLabel *downLabelText, *downLabel;
	QRssDisplayModel* m_pRssDisplayModel;
	QRssItemDelegate* m_pRssItemDelegate;
	QTorrentFilterProxyModel* m_pTorrentFilterProxyModel;
	QTorrentDisplayModel* m_pTorrentDisplayModel;
	QTorrentItemDelegat* m_pTorrentItemDelegate;
	QSearchDisplayModel* m_pSearchDisplayModel;
	QSearchItemDelegate* m_pSearchItemDelegate;
	TorrentManagerPtr m_pTorrentManager;
	TorrentGroupsManagerPtr m_pTorrentGroupsManager;
	QLineEdit* m_pTorrentSearchEdit;
	QLineEdit* m_pSearchEdit;
	PeerTableModel* m_pPeerTableModel;
	FiltersViewModel* m_pFiltersViewModel;
	SearchEnginePtr m_pSearchEngine;
	RatingWidget* m_pRatingWidget;
	FileSystemTorrentWatcherPtr m_pTorrentWatcher;
	OnlineReporter* m_pOnlineReporter;
	QStateMachine* m_pSorterStateMachine;
	QState* m_pSortAscendingSortState;
	QState* m_pSortDescendingSortState;
	ProgressItemDelegate* m_pProgressItemDelegate;
	FileSizeItemDelegate* m_pFileSizeItemDelegateShowZero;
	FileSizeItemDelegate* m_pFileSizeItemDelegateNotShowZero;
	SpeedItemDelegate* m_pSpeedItemDelegate;
	IpItemDelegate* m_pIpItemDelegate;
	PriorityItemDelegate* m_pPriorityItemDelegate;
	QActionGroup *m_pLanguageActionGroup, *m_pSkinActionGroup, *m_pAutoshutdownActionGroup;
	QSignalMapper *m_pLanguageSignalMapper, *m_pSkinSignalMapper, *m_pAutoshutdownSignalMapper;
	bool m_splitterInitFinished;
	SortButton* m_pSortButton;
#ifdef Q_WS_WIN
	QWinJumpList* m_pJumpList;
#endif
	void createTrayIcon();
	void createActions();
	void setupTray();
	void setupToolBar();
	void setupAutoShutdownMenu();
	void setupViewMenuState() const;
	void setupConnections() const;
	void setupSearchHeaderModel();
	void setupListView();
	void setupTabelWidgets();
	void setupStatusBar();
	void updateTorrentSorterHeader() const;
	void setupTorrentHeaderModel();
	void updateSearchSorterHeader() const;
	void setupRssInfoTab();
	void setupFileTabel();
	void setupLanguageChoseMenu();
	void setupSkinChoseMenu();
	void setupGroupTreeWidget() const;
#ifdef Q_WS_WIN
	void setupTasksCategory() const;
	void setupJumpList() const;
#endif
	void fillPieceDisplay(QSize) const;
	void setupCustomeWindow();
	void setupKeyMappings() const;
	void switchToTorrentsModel();
	void switchToSearchModel();
	void switchToRssModel() const;
	void resizeWindow(QMouseEvent* e) override;
	void saveWindowState();
	void loadHeaderState(QString prefix, EditableHeaderView* header, QList<int>& defaultColumnSizes) const;
	void saveHeaderState(QString prefix, EditableHeaderView* header) const;
	void UpdateModelSortOrder(QString modelPrefix, QSortFilterProxyModel* pModel) const;
	virtual QPushButton* getMinBtn() override;
	virtual QPushButton* getMaxBtn() override;
	virtual QPushButton* getCloseBtn() override;
	virtual QWidget* getTitleBar() override;
	virtual QWidget* centralWidget() override;
	virtual QLabel* getTitleLabel() override;
	virtual QLabel* getTitleIcon() override;
public slots:
	void queueTorrentsUp() const;
	void queueTorrentsDown() const;
	void updateSorting(int) const;
	void updateSortDirection() const;
	void HandleNewTorrent(const QString&);
	void UpdateUL(int) const;
	void UpdateDL(int) const;
	void ShowAbout();
	void CheckForUpdates() const;
	void Retranslate();
	void ShowCreateTorrentDialog(QString path = "");
	void ShowOpenTorrentDialog();
	void PauseSelected() const;
	void StopSelected() const;
	void ResumeSelected() const;
	void DeleteSelected() const;
	void UpdateInfoTab() const;
	void UpdatePeerTab() const;
	void UpdateFileTab() const;
	void UpadateCommentsTab() const;
#ifdef Q_WS_WIN
	void UpdateTrayIconOverlay() const;
#endif
	void UpadteTrackerTab() const;
	void OpenSettingsDialog();
	void IconActivated(QSystemTrayIcon::ActivationReason reason);

	void UpdateTabWidget() const;
	void UpdateStatusbar() const;
	void UpdateLimits() const;
	void ProcessMagnet();
	void PeformSearch() const;
	void PeformTorrentSearch(const QString& text) const;
	void CopyDiscribtion() const;
	void CopyInfoHash() const;
	void ClearPieceDisplay() const;
	void initWindowIcons();
	void AddTracker();
	void RemoveTracker() const;
	void EditTracker();
	void UpdateTracker() const;
	void AddPeer();
	void AddWebSeed();
	void ChnageTorrentFilter();
	void startBackUpWizard();
	void maximizeBtnClicked() override;
	void minimizeBtnClicked() override;
	void openSearchItemDescribtion() const;
	void addRssFeed();
	void removeRssFeed() const;
	void editRssFeed();
	void OnQuit();
	void UpdateRssInfo(const QItemSelection&) const;
	void OnMessageRecived(QString message);
	void toggleInfoTabsVisibility(bool) const;
	void toggleToolBarVisibility(bool);
	void toggleStatusBarVisibility(bool) const;
	void toggleGroupsVisibility(bool) const;
	void updateGroupVisibilityMenu() const;
	void updateInfoPlaneVsibilityMenu() const;
	void setLanguage(QString) const;
	void setSkin(QString) const;
	void showReportDialog();
	void showImportWiazrd();
	void onAutoShutdownAction(int) const;
	void onResetPowerMenu() const;
	void pauseAllTorrents() const;
	void restoreAllTorrents() const;
	void showRaitingDialog();
	void CheckForDefaultTorrentApp();
	void ShowAddCommentDialog();
	void UpdateStyleMenu();
	void OnCommentRemove(const QModelIndex& index);
	void OnTorrentAction(QModelIndex index, QTorrentItemDelegat::Buttons button);
};

Q_DECLARE_METATYPE(QHostAddress)
#endif // LTORRENT_H


