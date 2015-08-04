/********************************************************************************
** Form generated from reading UI file 'CustomWindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUSTOMWINDOW_H
#define UI_CUSTOMWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSplitter>
#include <QtGui/QStackedWidget>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTableView>
#include <QtGui/QTableWidget>
#include <QtGui/QToolBar>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CustomWindow
{
public:
    QAction *ACTION_MENU_OPEN_TORRENT;
    QAction *ACTION_MENU_OPEN_MAGNET;
    QAction *ACTION_MENU_CREATE;
    QAction *ACTION_MENU_EXIT;
    QAction *ACTION_MENU_SETTINGS;
    QAction *ACTION_TOOLBAR_PAUSE;
    QAction *ACTION_TOOLBAR_DOWNLOAD;
    QAction *ACTION_TOOLBAR_OPEN_URL;
    QAction *ACTION_TOOLBAR_START;
    QAction *ACTION_TOOLBAR_REMOVE;
    QAction *ACTION_MENU_ABAUT_CT;
    QAction *ACTION_MENU_CHECK_UPDATE;
    QAction *ACTION_TOOLBAR_SETTINGS;
    QAction *ACTION_MENU_BACKUP;
    QAction *ACTION_TOOLBAR_RSS_ADD;
    QAction *ACTION_TOOLBAR_RSS_REMOVE;
    QAction *ACTION_TOOLBAR_RSS_EDIT;
    QAction *ACTION_MENU_ABOUT_QT;
    QVBoxLayout *verticalLayout;
    QWidget *titleBar;
    QHBoxLayout *horizontalLayout;
    QLabel *tbMenu;
    QLabel *LTitle;
    QPushButton *pbMin;
    QPushButton *pbMax;
    QPushButton *pbClose;
    QWidget *m_centralWidget;
    QGridLayout *gridLayout_3;
    QStatusBar *mystatusBar;
    QGridLayout *gridLayout_6;
    QSplitter *spliiter;
    QSplitter *spliiter1;
    QTreeView *m_pGroupTreeView;
    QWidget *widget_4;
    QVBoxLayout *torrentViewLayout;
    QTreeView *m_pTorrentListView;
    QStackedWidget *m_pInfoPlaneContainer;
    QWidget *torrentPage;
    QGridLayout *gridLayout_11;
    QTabWidget *m_pTabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_7;
    QLabel *describtionLabel;
    QLabel *activetimeLabel;
    QLabel *label_7;
    QLabel *label_2;
    QLabel *timeleftLabel;
    QLabel *totalSizeLabel;
    QLabel *label;
    QGridLayout *gridLayout_4;
    QLabel *availibilityLabel;
    QLabel *progressLabel;
    QWidget *widget_2;
    QWidget *widget;
    QLabel *label_4;
    QLabel *downloadSpeedLabel;
    QLabel *pathLabel;
    QLabel *label_8;
    QLabel *peerCoutLabel;
    QLabel *label_9;
    QLabel *downloadedBytesLabel;
    QLabel *label_10;
    QLabel *seedCoutLabel;
    QLabel *uploadedBytesLabel;
    QLabel *label_5;
    QLabel *uploadSpeedLabel;
    QLabel *label_3;
    QLabel *label_6;
    QLabel *label_11;
    QWidget *tab_3;
    QGridLayout *gridLayout_2;
    QTableView *peerTableView;
    QWidget *tab_2;
    QGridLayout *gridLayout;
    QTableWidget *trackerTableWidget;
    QWidget *tab_4;
    QGridLayout *gridLayout_5;
    QTreeView *fileTableView;
    QWidget *rssPage;
    QGridLayout *m_pRssInfoLayout;
    QWidget *widget_3;
    QMenuBar *mainMenuBar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_CuteTorrent;
    QMenu *menuMENU_TOOLS;
    QStackedWidget *m_pToolBarsContainer;
    QToolBar *m_pTorrentToolBar;
    QToolBar *m_pSearchToolBar;
    QToolBar *m_pRssToolBar;

    void setupUi(QWidget *CustomWindow)
    {
        if (CustomWindow->objectName().isEmpty())
            CustomWindow->setObjectName(QString::fromUtf8("CustomWindow"));
        CustomWindow->resize(563, 681);
        CustomWindow->setMouseTracking(true);
        ACTION_MENU_OPEN_TORRENT = new QAction(CustomWindow);
        ACTION_MENU_OPEN_TORRENT->setObjectName(QString::fromUtf8("ACTION_MENU_OPEN_TORRENT"));
        ACTION_MENU_OPEN_TORRENT->setShortcutContext(Qt::WindowShortcut);
        ACTION_MENU_OPEN_TORRENT->setSoftKeyRole(QAction::NoSoftKey);
        ACTION_MENU_OPEN_MAGNET = new QAction(CustomWindow);
        ACTION_MENU_OPEN_MAGNET->setObjectName(QString::fromUtf8("ACTION_MENU_OPEN_MAGNET"));
        ACTION_MENU_CREATE = new QAction(CustomWindow);
        ACTION_MENU_CREATE->setObjectName(QString::fromUtf8("ACTION_MENU_CREATE"));
        ACTION_MENU_EXIT = new QAction(CustomWindow);
        ACTION_MENU_EXIT->setObjectName(QString::fromUtf8("ACTION_MENU_EXIT"));
        ACTION_MENU_EXIT->setMenuRole(QAction::QuitRole);
        ACTION_MENU_SETTINGS = new QAction(CustomWindow);
        ACTION_MENU_SETTINGS->setObjectName(QString::fromUtf8("ACTION_MENU_SETTINGS"));
        ACTION_MENU_SETTINGS->setMenuRole(QAction::PreferencesRole);
        ACTION_TOOLBAR_PAUSE = new QAction(CustomWindow);
        ACTION_TOOLBAR_PAUSE->setObjectName(QString::fromUtf8("ACTION_TOOLBAR_PAUSE"));
        ACTION_TOOLBAR_DOWNLOAD = new QAction(CustomWindow);
        ACTION_TOOLBAR_DOWNLOAD->setObjectName(QString::fromUtf8("ACTION_TOOLBAR_DOWNLOAD"));
        ACTION_TOOLBAR_OPEN_URL = new QAction(CustomWindow);
        ACTION_TOOLBAR_OPEN_URL->setObjectName(QString::fromUtf8("ACTION_TOOLBAR_OPEN_URL"));
        ACTION_TOOLBAR_START = new QAction(CustomWindow);
        ACTION_TOOLBAR_START->setObjectName(QString::fromUtf8("ACTION_TOOLBAR_START"));
        ACTION_TOOLBAR_REMOVE = new QAction(CustomWindow);
        ACTION_TOOLBAR_REMOVE->setObjectName(QString::fromUtf8("ACTION_TOOLBAR_REMOVE"));
        ACTION_MENU_ABAUT_CT = new QAction(CustomWindow);
        ACTION_MENU_ABAUT_CT->setObjectName(QString::fromUtf8("ACTION_MENU_ABAUT_CT"));
        ACTION_MENU_ABAUT_CT->setMenuRole(QAction::AboutRole);
        ACTION_MENU_CHECK_UPDATE = new QAction(CustomWindow);
        ACTION_MENU_CHECK_UPDATE->setObjectName(QString::fromUtf8("ACTION_MENU_CHECK_UPDATE"));
        ACTION_TOOLBAR_SETTINGS = new QAction(CustomWindow);
        ACTION_TOOLBAR_SETTINGS->setObjectName(QString::fromUtf8("ACTION_TOOLBAR_SETTINGS"));
        ACTION_MENU_BACKUP = new QAction(CustomWindow);
        ACTION_MENU_BACKUP->setObjectName(QString::fromUtf8("ACTION_MENU_BACKUP"));
        ACTION_TOOLBAR_RSS_ADD = new QAction(CustomWindow);
        ACTION_TOOLBAR_RSS_ADD->setObjectName(QString::fromUtf8("ACTION_TOOLBAR_RSS_ADD"));
        ACTION_TOOLBAR_RSS_REMOVE = new QAction(CustomWindow);
        ACTION_TOOLBAR_RSS_REMOVE->setObjectName(QString::fromUtf8("ACTION_TOOLBAR_RSS_REMOVE"));
        ACTION_TOOLBAR_RSS_EDIT = new QAction(CustomWindow);
        ACTION_TOOLBAR_RSS_EDIT->setObjectName(QString::fromUtf8("ACTION_TOOLBAR_RSS_EDIT"));
        ACTION_MENU_ABOUT_QT = new QAction(CustomWindow);
        ACTION_MENU_ABOUT_QT->setObjectName(QString::fromUtf8("ACTION_MENU_ABOUT_QT"));
        verticalLayout = new QVBoxLayout(CustomWindow);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(2, 1, 2, 2);
        titleBar = new QWidget(CustomWindow);
        titleBar->setObjectName(QString::fromUtf8("titleBar"));
        titleBar->setMinimumSize(QSize(0, 23));
        titleBar->setMaximumSize(QSize(16777215, 23));
        titleBar->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout = new QHBoxLayout(titleBar);
        horizontalLayout->setSpacing(4);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(5, 0, 1, 0);
        tbMenu = new QLabel(titleBar);
        tbMenu->setObjectName(QString::fromUtf8("tbMenu"));
        tbMenu->setMaximumSize(QSize(15, 15));
        tbMenu->setPixmap(QPixmap(QString::fromUtf8(":/icons/app.ico")));
        tbMenu->setScaledContents(true);

        horizontalLayout->addWidget(tbMenu);

        LTitle = new QLabel(titleBar);
        LTitle->setObjectName(QString::fromUtf8("LTitle"));
        LTitle->setMinimumSize(QSize(100, 22));
        LTitle->setMaximumSize(QSize(16777215, 22));
        LTitle->setStyleSheet(QString::fromUtf8(""));
        LTitle->setTextFormat(Qt::AutoText);

        horizontalLayout->addWidget(LTitle);

        pbMin = new QPushButton(titleBar);
        pbMin->setObjectName(QString::fromUtf8("pbMin"));
        pbMin->setMinimumSize(QSize(3, 22));
        pbMin->setMaximumSize(QSize(22, 22));
        pbMin->setIconSize(QSize(22, 22));
        pbMin->setFlat(true);

        horizontalLayout->addWidget(pbMin);

        pbMax = new QPushButton(titleBar);
        pbMax->setObjectName(QString::fromUtf8("pbMax"));
        pbMax->setMinimumSize(QSize(3, 22));
        pbMax->setMaximumSize(QSize(22, 22));
        pbMax->setIconSize(QSize(22, 22));
        pbMax->setFlat(true);

        horizontalLayout->addWidget(pbMax);

        pbClose = new QPushButton(titleBar);
        pbClose->setObjectName(QString::fromUtf8("pbClose"));
        pbClose->setMinimumSize(QSize(3, 22));
        pbClose->setMaximumSize(QSize(22, 22));
        pbClose->setIconSize(QSize(22, 22));
        pbClose->setFlat(true);

        horizontalLayout->addWidget(pbClose);


        verticalLayout->addWidget(titleBar);

        m_centralWidget = new QWidget(CustomWindow);
        m_centralWidget->setObjectName(QString::fromUtf8("m_centralWidget"));
        m_centralWidget->setMouseTracking(true);
        m_centralWidget->setStyleSheet(QString::fromUtf8(""));
        gridLayout_3 = new QGridLayout(m_centralWidget);
        gridLayout_3->setSpacing(0);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(6, 1, 6, 6);
        mystatusBar = new QStatusBar(m_centralWidget);
        mystatusBar->setObjectName(QString::fromUtf8("mystatusBar"));
        mystatusBar->setMaximumSize(QSize(16777215, 19));
        mystatusBar->setStyleSheet(QString::fromUtf8(""));

        gridLayout_3->addWidget(mystatusBar, 6, 0, 1, 1);

        gridLayout_6 = new QGridLayout();
        gridLayout_6->setSpacing(0);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout_6->setContentsMargins(0, -1, 0, -1);
        spliiter = new QSplitter(m_centralWidget);
        spliiter->setObjectName(QString::fromUtf8("spliiter"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(spliiter->sizePolicy().hasHeightForWidth());
        spliiter->setSizePolicy(sizePolicy);
        spliiter->setMouseTracking(true);
        spliiter->setOrientation(Qt::Vertical);
        spliiter1 = new QSplitter(spliiter);
        spliiter1->setObjectName(QString::fromUtf8("spliiter1"));
        spliiter1->setMouseTracking(true);
        spliiter1->setStyleSheet(QString::fromUtf8(""));
        spliiter1->setOrientation(Qt::Horizontal);
        m_pGroupTreeView = new QTreeView(spliiter1);
        m_pGroupTreeView->setObjectName(QString::fromUtf8("m_pGroupTreeView"));
        m_pGroupTreeView->setMouseTracking(true);
        m_pGroupTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_pGroupTreeView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        m_pGroupTreeView->setRootIsDecorated(false);
        m_pGroupTreeView->setAnimated(true);
        m_pGroupTreeView->setHeaderHidden(true);
        spliiter1->addWidget(m_pGroupTreeView);
        widget_4 = new QWidget(spliiter1);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        torrentViewLayout = new QVBoxLayout(widget_4);
        torrentViewLayout->setSpacing(0);
        torrentViewLayout->setContentsMargins(0, 0, 0, 0);
        torrentViewLayout->setObjectName(QString::fromUtf8("torrentViewLayout"));
        m_pTorrentListView = new QTreeView(widget_4);
        m_pTorrentListView->setObjectName(QString::fromUtf8("m_pTorrentListView"));
        m_pTorrentListView->setMouseTracking(true);
        m_pTorrentListView->setContextMenuPolicy(Qt::CustomContextMenu);
        m_pTorrentListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_pTorrentListView->setDragDropMode(QAbstractItemView::DropOnly);
        m_pTorrentListView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        m_pTorrentListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        m_pTorrentListView->setRootIsDecorated(false);
        m_pTorrentListView->setSortingEnabled(true);
        m_pTorrentListView->setAnimated(true);
        m_pTorrentListView->setHeaderHidden(true);

        torrentViewLayout->addWidget(m_pTorrentListView);

        spliiter1->addWidget(widget_4);
        spliiter->addWidget(spliiter1);
        m_pInfoPlaneContainer = new QStackedWidget(spliiter);
        m_pInfoPlaneContainer->setObjectName(QString::fromUtf8("m_pInfoPlaneContainer"));
        torrentPage = new QWidget();
        torrentPage->setObjectName(QString::fromUtf8("torrentPage"));
        gridLayout_11 = new QGridLayout(torrentPage);
        gridLayout_11->setSpacing(6);
        gridLayout_11->setContentsMargins(11, 11, 11, 11);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        gridLayout_11->setContentsMargins(2, 0, 2, 0);
        m_pTabWidget = new QTabWidget(torrentPage);
        m_pTabWidget->setObjectName(QString::fromUtf8("m_pTabWidget"));
        m_pTabWidget->setMouseTracking(true);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        tab->setMouseTracking(true);
        gridLayout_7 = new QGridLayout(tab);
        gridLayout_7->setSpacing(6);
        gridLayout_7->setContentsMargins(11, 11, 11, 11);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        describtionLabel = new QLabel(tab);
        describtionLabel->setObjectName(QString::fromUtf8("describtionLabel"));
        describtionLabel->setMaximumSize(QSize(16777215, 16));
        describtionLabel->setContextMenuPolicy(Qt::ActionsContextMenu);
        describtionLabel->setWordWrap(true);
        describtionLabel->setOpenExternalLinks(true);

        gridLayout_7->addWidget(describtionLabel, 4, 1, 1, 4);

        activetimeLabel = new QLabel(tab);
        activetimeLabel->setObjectName(QString::fromUtf8("activetimeLabel"));

        gridLayout_7->addWidget(activetimeLabel, 1, 1, 1, 2);

        label_7 = new QLabel(tab);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_7->addWidget(label_7, 1, 3, 1, 1);

        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_7->addWidget(label_2, 2, 0, 1, 1);

        timeleftLabel = new QLabel(tab);
        timeleftLabel->setObjectName(QString::fromUtf8("timeleftLabel"));

        gridLayout_7->addWidget(timeleftLabel, 1, 4, 1, 1);

        totalSizeLabel = new QLabel(tab);
        totalSizeLabel->setObjectName(QString::fromUtf8("totalSizeLabel"));

        gridLayout_7->addWidget(totalSizeLabel, 6, 1, 1, 2);

        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_7->addWidget(label, 1, 0, 1, 1);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setSpacing(6);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        availibilityLabel = new QLabel(tab);
        availibilityLabel->setObjectName(QString::fromUtf8("availibilityLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(availibilityLabel->sizePolicy().hasHeightForWidth());
        availibilityLabel->setSizePolicy(sizePolicy1);
        availibilityLabel->setMaximumSize(QSize(48, 16777215));

        gridLayout_4->addWidget(availibilityLabel, 1, 1, 1, 1);

        progressLabel = new QLabel(tab);
        progressLabel->setObjectName(QString::fromUtf8("progressLabel"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(progressLabel->sizePolicy().hasHeightForWidth());
        progressLabel->setSizePolicy(sizePolicy2);
        progressLabel->setMaximumSize(QSize(48, 16777215));

        gridLayout_4->addWidget(progressLabel, 0, 1, 1, 1);

        widget_2 = new QWidget(tab);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy3);
        widget_2->setMinimumSize(QSize(0, 15));

        gridLayout_4->addWidget(widget_2, 1, 0, 1, 1);

        widget = new QWidget(tab);
        widget->setObjectName(QString::fromUtf8("widget"));
        sizePolicy3.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy3);
        widget->setMinimumSize(QSize(0, 20));
        widget->setStyleSheet(QString::fromUtf8(""));

        gridLayout_4->addWidget(widget, 0, 0, 1, 1);


        gridLayout_7->addLayout(gridLayout_4, 0, 0, 1, 5);

        label_4 = new QLabel(tab);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_7->addWidget(label_4, 6, 3, 1, 1);

        downloadSpeedLabel = new QLabel(tab);
        downloadSpeedLabel->setObjectName(QString::fromUtf8("downloadSpeedLabel"));

        gridLayout_7->addWidget(downloadSpeedLabel, 3, 1, 1, 2);

        pathLabel = new QLabel(tab);
        pathLabel->setObjectName(QString::fromUtf8("pathLabel"));

        gridLayout_7->addWidget(pathLabel, 6, 4, 1, 1);

        label_8 = new QLabel(tab);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_7->addWidget(label_8, 2, 3, 1, 1);

        peerCoutLabel = new QLabel(tab);
        peerCoutLabel->setObjectName(QString::fromUtf8("peerCoutLabel"));

        gridLayout_7->addWidget(peerCoutLabel, 7, 4, 1, 1);

        label_9 = new QLabel(tab);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_7->addWidget(label_9, 3, 3, 1, 1);

        downloadedBytesLabel = new QLabel(tab);
        downloadedBytesLabel->setObjectName(QString::fromUtf8("downloadedBytesLabel"));

        gridLayout_7->addWidget(downloadedBytesLabel, 2, 1, 1, 2);

        label_10 = new QLabel(tab);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_7->addWidget(label_10, 7, 0, 1, 1);

        seedCoutLabel = new QLabel(tab);
        seedCoutLabel->setObjectName(QString::fromUtf8("seedCoutLabel"));

        gridLayout_7->addWidget(seedCoutLabel, 7, 1, 1, 2);

        uploadedBytesLabel = new QLabel(tab);
        uploadedBytesLabel->setObjectName(QString::fromUtf8("uploadedBytesLabel"));

        gridLayout_7->addWidget(uploadedBytesLabel, 2, 4, 1, 1);

        label_5 = new QLabel(tab);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_7->addWidget(label_5, 6, 0, 1, 1);

        uploadSpeedLabel = new QLabel(tab);
        uploadSpeedLabel->setObjectName(QString::fromUtf8("uploadSpeedLabel"));

        gridLayout_7->addWidget(uploadSpeedLabel, 3, 4, 1, 1);

        label_3 = new QLabel(tab);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_7->addWidget(label_3, 3, 0, 1, 1);

        label_6 = new QLabel(tab);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_7->addWidget(label_6, 4, 0, 1, 1);

        label_11 = new QLabel(tab);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_7->addWidget(label_11, 7, 3, 1, 1);

        m_pTabWidget->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        tab_3->setMouseTracking(true);
        gridLayout_2 = new QGridLayout(tab_3);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        peerTableView = new QTableView(tab_3);
        peerTableView->setObjectName(QString::fromUtf8("peerTableView"));
        peerTableView->setMouseTracking(true);
        peerTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        peerTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        peerTableView->setShowGrid(false);
        peerTableView->setSortingEnabled(true);
        peerTableView->verticalHeader()->setDefaultSectionSize(18);

        gridLayout_2->addWidget(peerTableView, 0, 0, 1, 1);

        m_pTabWidget->addTab(tab_3, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tab_2->setMouseTracking(true);
        gridLayout = new QGridLayout(tab_2);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        trackerTableWidget = new QTableWidget(tab_2);
        if (trackerTableWidget->columnCount() < 4)
            trackerTableWidget->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        trackerTableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        trackerTableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        trackerTableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        trackerTableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        trackerTableWidget->setObjectName(QString::fromUtf8("trackerTableWidget"));
        trackerTableWidget->setMouseTracking(true);
        trackerTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        trackerTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        trackerTableWidget->setShowGrid(false);
        trackerTableWidget->horizontalHeader()->setDefaultSectionSize(120);
        trackerTableWidget->verticalHeader()->setVisible(false);
        trackerTableWidget->verticalHeader()->setDefaultSectionSize(18);

        gridLayout->addWidget(trackerTableWidget, 0, 0, 1, 1);

        m_pTabWidget->addTab(tab_2, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        tab_4->setMouseTracking(true);
        gridLayout_5 = new QGridLayout(tab_4);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        fileTableView = new QTreeView(tab_4);
        fileTableView->setObjectName(QString::fromUtf8("fileTableView"));
        fileTableView->setMaximumSize(QSize(16777215, 16777215));
        fileTableView->setMouseTracking(true);
        fileTableView->setContextMenuPolicy(Qt::CustomContextMenu);
        fileTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        fileTableView->setSortingEnabled(true);
        fileTableView->setAnimated(true);

        gridLayout_5->addWidget(fileTableView, 0, 0, 1, 1);

        m_pTabWidget->addTab(tab_4, QString());

        gridLayout_11->addWidget(m_pTabWidget, 0, 0, 1, 1);

        m_pInfoPlaneContainer->addWidget(torrentPage);
        rssPage = new QWidget();
        rssPage->setObjectName(QString::fromUtf8("rssPage"));
        m_pRssInfoLayout = new QGridLayout(rssPage);
        m_pRssInfoLayout->setSpacing(6);
        m_pRssInfoLayout->setContentsMargins(11, 11, 11, 11);
        m_pRssInfoLayout->setObjectName(QString::fromUtf8("m_pRssInfoLayout"));
        m_pRssInfoLayout->setContentsMargins(0, 2, 0, 2);
        widget_3 = new QWidget(rssPage);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));

        m_pRssInfoLayout->addWidget(widget_3, 0, 0, 1, 1);

        m_pInfoPlaneContainer->addWidget(rssPage);
        spliiter->addWidget(m_pInfoPlaneContainer);

        gridLayout_6->addWidget(spliiter, 0, 1, 1, 1);


        gridLayout_3->addLayout(gridLayout_6, 5, 0, 1, 1);

        mainMenuBar = new QMenuBar(m_centralWidget);
        mainMenuBar->setObjectName(QString::fromUtf8("mainMenuBar"));
        mainMenuBar->setEnabled(true);
        mainMenuBar->setMinimumSize(QSize(0, 0));
        mainMenuBar->setMaximumSize(QSize(16777215, 16777215));
        mainMenuBar->setStyleSheet(QString::fromUtf8(""));
        mainMenuBar->setDefaultUp(false);
        mainMenuBar->setNativeMenuBar(false);
        menu = new QMenu(mainMenuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu->setStyleSheet(QString::fromUtf8(""));
        menu_2 = new QMenu(mainMenuBar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        menu_CuteTorrent = new QMenu(mainMenuBar);
        menu_CuteTorrent->setObjectName(QString::fromUtf8("menu_CuteTorrent"));
        menuMENU_TOOLS = new QMenu(mainMenuBar);
        menuMENU_TOOLS->setObjectName(QString::fromUtf8("menuMENU_TOOLS"));

        gridLayout_3->addWidget(mainMenuBar, 1, 0, 1, 1);

        m_pToolBarsContainer = new QStackedWidget(m_centralWidget);
        m_pToolBarsContainer->setObjectName(QString::fromUtf8("m_pToolBarsContainer"));
        m_pToolBarsContainer->setMaximumSize(QSize(16777215, 32));
        m_pToolBarsContainer->setMouseTracking(true);
        m_pTorrentToolBar = new QToolBar();
        m_pTorrentToolBar->setObjectName(QString::fromUtf8("m_pTorrentToolBar"));
        m_pTorrentToolBar->setStyleSheet(QString::fromUtf8(" QToolBar {\n"
"               border-bottom: none;\n"
"               border-top: none;\n"
"               spacing:3px;\n"
"               padding-right:2px;\n"
"               }"));
        m_pToolBarsContainer->addWidget(m_pTorrentToolBar);
        m_pSearchToolBar = new QToolBar();
        m_pSearchToolBar->setObjectName(QString::fromUtf8("m_pSearchToolBar"));
        m_pSearchToolBar->setStyleSheet(QString::fromUtf8(" QToolBar {\n"
"               border-bottom: none;\n"
"               border-top: none;\n"
"               spacing:3px;\n"
"               padding-right:2px;\n"
"               }"));
        m_pToolBarsContainer->addWidget(m_pSearchToolBar);
        m_pRssToolBar = new QToolBar();
        m_pRssToolBar->setObjectName(QString::fromUtf8("m_pRssToolBar"));
        m_pRssToolBar->setStyleSheet(QString::fromUtf8(" QToolBar {\n"
"               border-bottom: none;\n"
"               border-top: none;\n"
"               spacing:3px;\n"
"               padding-right:2px;\n"
"               }"));
        m_pToolBarsContainer->addWidget(m_pRssToolBar);

        gridLayout_3->addWidget(m_pToolBarsContainer, 3, 0, 1, 1);


        verticalLayout->addWidget(m_centralWidget);

        QWidget::setTabOrder(m_pGroupTreeView, m_pTabWidget);
        QWidget::setTabOrder(m_pTabWidget, peerTableView);
        QWidget::setTabOrder(peerTableView, trackerTableWidget);
        QWidget::setTabOrder(trackerTableWidget, fileTableView);
        QWidget::setTabOrder(fileTableView, pbMin);
        QWidget::setTabOrder(pbMin, pbMax);
        QWidget::setTabOrder(pbMax, pbClose);

        mainMenuBar->addAction(menu->menuAction());
        mainMenuBar->addAction(menu_2->menuAction());
        mainMenuBar->addAction(menuMENU_TOOLS->menuAction());
        mainMenuBar->addAction(menu_CuteTorrent->menuAction());
        menu->addAction(ACTION_MENU_OPEN_TORRENT);
        menu->addAction(ACTION_MENU_OPEN_MAGNET);
        menu->addSeparator();
        menu->addAction(ACTION_MENU_CREATE);
        menu->addSeparator();
        menu->addAction(ACTION_MENU_EXIT);
        menu_2->addAction(ACTION_MENU_SETTINGS);
        menu_CuteTorrent->addAction(ACTION_MENU_ABAUT_CT);
        menu_CuteTorrent->addAction(ACTION_MENU_ABOUT_QT);
        menu_CuteTorrent->addSeparator();
        menu_CuteTorrent->addAction(ACTION_MENU_CHECK_UPDATE);
        menuMENU_TOOLS->addAction(ACTION_MENU_BACKUP);
        m_pTorrentToolBar->addAction(ACTION_TOOLBAR_START);
        m_pTorrentToolBar->addAction(ACTION_TOOLBAR_PAUSE);
        m_pTorrentToolBar->addAction(ACTION_TOOLBAR_REMOVE);
        m_pTorrentToolBar->addAction(ACTION_TOOLBAR_SETTINGS);
        m_pSearchToolBar->addAction(ACTION_TOOLBAR_DOWNLOAD);
        m_pSearchToolBar->addAction(ACTION_TOOLBAR_OPEN_URL);
        m_pSearchToolBar->addAction(ACTION_TOOLBAR_SETTINGS);
        m_pRssToolBar->addAction(ACTION_TOOLBAR_RSS_ADD);
        m_pRssToolBar->addAction(ACTION_TOOLBAR_RSS_REMOVE);
        m_pRssToolBar->addAction(ACTION_TOOLBAR_RSS_EDIT);
        m_pRssToolBar->addAction(ACTION_TOOLBAR_SETTINGS);

        retranslateUi(CustomWindow);
        QObject::connect(ACTION_MENU_CREATE, SIGNAL(triggered()), CustomWindow, SLOT(ShowCreateTorrentDialog()));
        QObject::connect(ACTION_MENU_OPEN_TORRENT, SIGNAL(triggered()), CustomWindow, SLOT(ShowOpenTorrentDialog()));
        QObject::connect(ACTION_TOOLBAR_START, SIGNAL(triggered()), CustomWindow, SLOT(ResumeSelected()));
        QObject::connect(ACTION_TOOLBAR_PAUSE, SIGNAL(triggered()), CustomWindow, SLOT(PauseSelected()));
        QObject::connect(ACTION_TOOLBAR_REMOVE, SIGNAL(triggered()), CustomWindow, SLOT(DeleteSelected()));
        QObject::connect(ACTION_MENU_SETTINGS, SIGNAL(triggered()), CustomWindow, SLOT(OpenSettingsDialog()));
        QObject::connect(ACTION_MENU_CHECK_UPDATE, SIGNAL(triggered()), CustomWindow, SLOT(CheckForUpdates()));
        QObject::connect(ACTION_MENU_ABAUT_CT, SIGNAL(triggered()), CustomWindow, SLOT(ShowAbout()));
        QObject::connect(ACTION_MENU_OPEN_MAGNET, SIGNAL(triggered()), CustomWindow, SLOT(ProcessMagnet()));
        QObject::connect(ACTION_TOOLBAR_SETTINGS, SIGNAL(triggered()), CustomWindow, SLOT(OpenSettingsDialog()));
        QObject::connect(ACTION_MENU_BACKUP, SIGNAL(triggered()), CustomWindow, SLOT(startBackUpWizard()));
        QObject::connect(ACTION_TOOLBAR_OPEN_URL, SIGNAL(triggered()), CustomWindow, SLOT(openSearchItemDescribtion()));
        QObject::connect(ACTION_TOOLBAR_RSS_ADD, SIGNAL(triggered()), CustomWindow, SLOT(addRssFeed()));
        QObject::connect(ACTION_TOOLBAR_RSS_EDIT, SIGNAL(triggered()), CustomWindow, SLOT(editRssFeed()));
        QObject::connect(ACTION_TOOLBAR_RSS_REMOVE, SIGNAL(triggered()), CustomWindow, SLOT(removeRssFeed()));

        m_pInfoPlaneContainer->setCurrentIndex(0);
        m_pTabWidget->setCurrentIndex(0);
        m_pToolBarsContainer->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(CustomWindow);
    } // setupUi

    void retranslateUi(QWidget *CustomWindow)
    {
        CustomWindow->setWindowTitle(QApplication::translate("CustomWindow", "CuteTorrent", 0, QApplication::UnicodeUTF8));
        ACTION_MENU_OPEN_TORRENT->setText(QApplication::translate("CustomWindow", "MENU_OPEN_TORRENT", 0, QApplication::UnicodeUTF8));
        ACTION_MENU_OPEN_TORRENT->setShortcut(QApplication::translate("CustomWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        ACTION_MENU_OPEN_MAGNET->setText(QApplication::translate("CustomWindow", "MENU_OPEN_MAGNET", 0, QApplication::UnicodeUTF8));
        ACTION_MENU_OPEN_MAGNET->setShortcut(QApplication::translate("CustomWindow", "Ctrl+M", 0, QApplication::UnicodeUTF8));
        ACTION_MENU_CREATE->setText(QApplication::translate("CustomWindow", "MENU_CREATE_TORRENT", 0, QApplication::UnicodeUTF8));
        ACTION_MENU_CREATE->setShortcut(QApplication::translate("CustomWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        ACTION_MENU_EXIT->setText(QApplication::translate("CustomWindow", "MENU_EXIT", 0, QApplication::UnicodeUTF8));
        ACTION_MENU_EXIT->setShortcut(QApplication::translate("CustomWindow", "Alt+X", 0, QApplication::UnicodeUTF8));
        ACTION_MENU_SETTINGS->setText(QApplication::translate("CustomWindow", "MENU_CONFIGURATION", 0, QApplication::UnicodeUTF8));
        ACTION_MENU_SETTINGS->setShortcut(QApplication::translate("CustomWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        ACTION_TOOLBAR_PAUSE->setText(QApplication::translate("CustomWindow", "STR_PAUSE", 0, QApplication::UnicodeUTF8));
        ACTION_TOOLBAR_PAUSE->setIconText(QApplication::translate("CustomWindow", "HINT_PAUSE", 0, QApplication::UnicodeUTF8));
        ACTION_TOOLBAR_DOWNLOAD->setText(QApplication::translate("CustomWindow", "STR_DOWNLOAD", 0, QApplication::UnicodeUTF8));
        ACTION_TOOLBAR_DOWNLOAD->setIconText(QApplication::translate("CustomWindow", "HINT_DOWNLOAD", 0, QApplication::UnicodeUTF8));
        ACTION_TOOLBAR_OPEN_URL->setText(QApplication::translate("CustomWindow", "STR_OPEN_URL", 0, QApplication::UnicodeUTF8));
        ACTION_TOOLBAR_OPEN_URL->setIconText(QApplication::translate("CustomWindow", "HINT_OPEN_URL", 0, QApplication::UnicodeUTF8));
        ACTION_TOOLBAR_START->setText(QApplication::translate("CustomWindow", "STR_START", 0, QApplication::UnicodeUTF8));
        ACTION_TOOLBAR_START->setIconText(QApplication::translate("CustomWindow", "HINT_RESUME", 0, QApplication::UnicodeUTF8));
        ACTION_TOOLBAR_REMOVE->setText(QApplication::translate("CustomWindow", "STR_REMOVE", 0, QApplication::UnicodeUTF8));
        ACTION_TOOLBAR_REMOVE->setIconText(QApplication::translate("CustomWindow", "HINT_REMOVE", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        ACTION_TOOLBAR_REMOVE->setToolTip(QApplication::translate("CustomWindow", "STR_REMOVE", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        ACTION_MENU_ABAUT_CT->setText(QApplication::translate("CustomWindow", "MENU_ABAUT", 0, QApplication::UnicodeUTF8));
        ACTION_MENU_ABAUT_CT->setShortcut(QApplication::translate("CustomWindow", "F1", 0, QApplication::UnicodeUTF8));
        ACTION_MENU_CHECK_UPDATE->setText(QApplication::translate("CustomWindow", "MENU_CHECK_UPDATES", 0, QApplication::UnicodeUTF8));
        ACTION_MENU_CHECK_UPDATE->setShortcut(QApplication::translate("CustomWindow", "Ctrl+U", 0, QApplication::UnicodeUTF8));
        ACTION_TOOLBAR_SETTINGS->setText(QApplication::translate("CustomWindow", "STR_SETTINGS", 0, QApplication::UnicodeUTF8));
        ACTION_MENU_BACKUP->setText(QApplication::translate("CustomWindow", "TOOLS_BACKUP", 0, QApplication::UnicodeUTF8));
        ACTION_TOOLBAR_RSS_ADD->setText(QApplication::translate("CustomWindow", "ACTION_RSS_ADD", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        ACTION_TOOLBAR_RSS_ADD->setToolTip(QApplication::translate("CustomWindow", "ACTION_RSS_ADD", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        ACTION_TOOLBAR_RSS_REMOVE->setText(QApplication::translate("CustomWindow", "ACTION_RSS_REMOVE", 0, QApplication::UnicodeUTF8));
        ACTION_TOOLBAR_RSS_EDIT->setText(QApplication::translate("CustomWindow", "ACTION_RSS_EDIT", 0, QApplication::UnicodeUTF8));
        ACTION_MENU_ABOUT_QT->setText(QApplication::translate("CustomWindow", "MENU_ABOUT_QT", 0, QApplication::UnicodeUTF8));
        tbMenu->setText(QString());
        LTitle->setText(QApplication::translate("CustomWindow", "CuteTorrent", 0, QApplication::UnicodeUTF8));
        describtionLabel->setText(QString());
        activetimeLabel->setText(QString());
        label_7->setText(QApplication::translate("CustomWindow", "INFO_REMAININF_TIME", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CustomWindow", "INFO_DOWNLOADED", 0, QApplication::UnicodeUTF8));
        timeleftLabel->setText(QString());
        totalSizeLabel->setText(QString());
        label->setText(QApplication::translate("CustomWindow", "INFO_ACTIVE_TIME", 0, QApplication::UnicodeUTF8));
        progressLabel->setText(QString());
        label_4->setText(QApplication::translate("CustomWindow", "INFO_PATH", 0, QApplication::UnicodeUTF8));
        downloadSpeedLabel->setText(QString());
        pathLabel->setText(QString());
        label_8->setText(QApplication::translate("CustomWindow", "INFO_UPLOADED", 0, QApplication::UnicodeUTF8));
        peerCoutLabel->setText(QString());
        label_9->setText(QApplication::translate("CustomWindow", "INFO_UPLOAD_SPEED", 0, QApplication::UnicodeUTF8));
        downloadedBytesLabel->setText(QString());
        label_10->setText(QApplication::translate("CustomWindow", "INFO_SEED_COUNT", 0, QApplication::UnicodeUTF8));
        seedCoutLabel->setText(QString());
        uploadedBytesLabel->setText(QString());
        label_5->setText(QApplication::translate("CustomWindow", "INFO_TOTAL_SIZE", 0, QApplication::UnicodeUTF8));
        uploadSpeedLabel->setText(QString());
        label_3->setText(QApplication::translate("CustomWindow", "INFO_DOWNLOAD_SPEED", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("CustomWindow", "INFO_DESCRIBTION", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("CustomWindow", "INFO_PEER_COUNT", 0, QApplication::UnicodeUTF8));
        m_pTabWidget->setTabText(m_pTabWidget->indexOf(tab), QApplication::translate("CustomWindow", "TAB_INFO", 0, QApplication::UnicodeUTF8));
        m_pTabWidget->setTabText(m_pTabWidget->indexOf(tab_3), QApplication::translate("CustomWindow", "TAB_PEERS", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = trackerTableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("CustomWindow", "TRACKER_URL", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = trackerTableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("CustomWindow", "TRACKER_STATUS", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = trackerTableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("CustomWindow", "TRACKER_NEXT_ANNOUNCE", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = trackerTableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("CustomWindow", "TRACKER_PEERS", 0, QApplication::UnicodeUTF8));
        m_pTabWidget->setTabText(m_pTabWidget->indexOf(tab_2), QApplication::translate("CustomWindow", "TAB_TRACKERS", 0, QApplication::UnicodeUTF8));
        m_pTabWidget->setTabText(m_pTabWidget->indexOf(tab_4), QApplication::translate("CustomWindow", "TAB_FILES", 0, QApplication::UnicodeUTF8));
        menu->setTitle(QApplication::translate("CustomWindow", "MENU_FILE", 0, QApplication::UnicodeUTF8));
        menu_2->setTitle(QApplication::translate("CustomWindow", "MENU_SETTINGS", 0, QApplication::UnicodeUTF8));
        menu_CuteTorrent->setTitle(QApplication::translate("CustomWindow", "MENU_HELP", 0, QApplication::UnicodeUTF8));
        menuMENU_TOOLS->setTitle(QApplication::translate("CustomWindow", "MENU_TOOLS", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CustomWindow: public Ui_CustomWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUSTOMWINDOW_H
