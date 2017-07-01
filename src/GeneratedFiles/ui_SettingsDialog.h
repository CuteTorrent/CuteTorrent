/********************************************************************************
** Form generated from reading UI file 'SettingsDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSDIALOG_H
#define UI_SETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingsDialog
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *titleBar;
    QHBoxLayout *horizontalLayout_3;
    QLabel *tbMenu;
    QLabel *LTitle;
    QPushButton *pbClose;
    QWidget *m_centralWidget;
    QGridLayout *gridLayout_4;
    QListWidget *listWidget;
    QDialogButtonBox *buttonBox;
    QStackedWidget *stackedWidget;
    QWidget *torrentTab;
    QGridLayout *gridLayout_22;
    QGroupBox *useNotificationsCheckBox;
    QGridLayout *gridLayout_13;
    QCheckBox *showTrackerErrorsCheckBox;
    QCheckBox *showDiskErrorsCheckBox;
    QCheckBox *showRssErrorsCheckBox;
    QGroupBox *trackerGroupBox;
    QGridLayout *gridLayout_2;
    QLabel *label_23;
    QLineEdit *trackerPortEdit;
    QComboBox *styleComboBox;
    QComboBox *localeComboBox;
    QCheckBox *asociationCheckBox;
    QCheckBox *winShelItegrationCheckBox;
    QLabel *label_26;
    QCheckBox *startMinimizedCheckBox;
    QCheckBox *scriptDebugingCheckBox;
    QCheckBox *magnetAssociationCheckBox;
    QCheckBox *runOnbootCheckBox;
    QLabel *label_8;
    QSpacerItem *verticalSpacer_9;
    QCheckBox *closeOnHideCheckBox;
    QSpacerItem *horizontalSpacer_4;
    QGroupBox *watchDirEnabledCheckbox;
    QGridLayout *gridLayout_35;
    QGridLayout *gridLayout_23;
    QRadioButton *useStaticPathRadioButton;
    QRadioButton *showDialogRadioButton;
    QRadioButton *usetTorrentFilteringRadioButton;
    QPushButton *browseWatchStaticPathButton;
    QLineEdit *staticSavePathEdit;
    QLineEdit *watchDirPathEdit;
    QLabel *label_43;
    QPushButton *browseWatchDirButton;
    QCheckBox *checkBox;
    QWidget *restrictionTab;
    QGridLayout *gridLayout_30;
    QGroupBox *groupBox_12;
    QGridLayout *gridLayout_29;
    QCheckBox *usePExCheckBox;
    QCheckBox *useLSDCheckBox;
    QCheckBox *useDHTCheckBox;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_31;
    QLabel *label_6;
    QSpinBox *uploadLimitEdit;
    QLabel *label_7;
    QSpinBox *downloadLimitEdit;
    QLabel *label_33;
    QSpinBox *dhtLimitEdit;
    QGroupBox *limitLocalConnectionCheckBox;
    QGridLayout *gridLayout_32;
    QLabel *label_31;
    QSpinBox *localUploadLimitEdit;
    QLabel *label_32;
    QSpinBox *localDownloadLimitEdit;
    QCheckBox *limitUtpCheckBox;
    QDoubleSpinBox *seedGlobalRatioEdit;
    QSpinBox *maxConnectionsPerTorrentEdit;
    QLabel *label_30;
    QTimeEdit *seedTimeLimitEdit;
    QLabel *label_34;
    QLabel *label_41;
    QGroupBox *restrictionsGroupBox;
    QGridLayout *gridLayout_3;
    QSpinBox *activeSeedLimitEdit;
    QSpinBox *activeLimitEdit;
    QSpinBox *activeDownloadLimitEdit;
    QLabel *label_17;
    QLabel *label_16;
    QLabel *label_15;
    QSpacerItem *verticalSpacer_6;
    QLabel *label_45;
    QSpinBox *maxConnectionsEdit;
    QWidget *page;
    QGridLayout *gridLayout_20;
    QLineEdit *portEdit;
    QLabel *label;
    QGroupBox *groupBox_10;
    QGridLayout *gridLayout_28;
    QLabel *label_27;
    QComboBox *inEncPolicyComboBox;
    QLabel *label_28;
    QComboBox *outEncPolicyComboBox;
    QLabel *label_29;
    QComboBox *encLevelComboBox;
    QCheckBox *preferFullEncCheckBox;
    QGroupBox *proxyGroupBox;
    QGridLayout *gridLayout;
    QLabel *label_3;
    QLineEdit *proxyHostEdit;
    QLineEdit *proxyUsernameEdit;
    QLabel *label_5;
    QLabel *label_4;
    QLabel *label_2;
    QLineEdit *proxyPwdEdit;
    QComboBox *proxyTypeComboBox;
    QLabel *label_44;
    QLineEdit *proxyPortEdit;
    QSpacerItem *verticalSpacer_5;
    QSpacerItem *verticalSpacer_4;
    QCheckBox *useRandomPortCheckBox;
    QCheckBox *portMappingsCheckBox;
    QWidget *hddTab;
    QGridLayout *gridLayout_12;
    QCheckBox *useDiskReadAheadCheckBox;
    QCheckBox *alowReorderedOpsCheckBox;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_42;
    QSpinBox *casheSizeLineEdit;
    QComboBox *diskAllocationComboBox;
    QCheckBox *lockFilesCheckBox;
    QCheckBox *useReadCasheCheckBox;
    QCheckBox *lowPrioDiskCheckBox;
    QLabel *label_19;
    QComboBox *diskIOCasheModeComboBox;
    QLabel *label_20;
    QSpacerItem *verticalSpacer_3;
    QWidget *filteringTab;
    QGridLayout *gridLayout_5;
    QGroupBox *groupBox_9;
    QGridLayout *gridLayout_8;
    QLineEdit *groupSavePathEdit;
    QPushButton *BrowseGroupSavePathButton;
    QLabel *label_47;
    QLabel *label_48;
    QTextEdit *extensionsEdit;
    QLineEdit *newGroupNameEdit;
    QLabel *label_46;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *addNewGroupButton;
    QPushButton *updateGroupButton;
    QPushButton *removeGroupButton;
    QSpacerItem *horizontalSpacer_9;
    QTreeWidget *GroupsTreeWidget;
    QWidget *dtTab;
    QGridLayout *gridLayout_25;
    QGroupBox *groupBox_6;
    QGridLayout *gridLayout_9;
    QLineEdit *DTPathEdit;
    QPushButton *browseDTPathButton;
    QGroupBox *groupBox_8;
    QGridLayout *gridLayout_11;
    QComboBox *driveNumberComboBox;
    QGroupBox *customMoutGroupBox;
    QGridLayout *gridLayout_10;
    QLineEdit *customCommandEdit;
    QSpacerItem *verticalSpacer_2;
    QWidget *schedulingTab;
    QGridLayout *gridLayout_26;
    QGroupBox *groupBox_11;
    QGridLayout *gridLayout_14;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_15;
    QLabel *label_11;
    QLineEdit *dlLimitEdit;
    QLabel *label_12;
    QLineEdit *ulLimitEdit;
    QRadioButton *startAllRadioButton;
    QRadioButton *limitUlRadioButton;
    QRadioButton *limitDlRadioButton;
    QRadioButton *pauseAllRadioButton;
    QDateTimeEdit *beginDateTimeEdit;
    QLineEdit *taskNameLineEdit;
    QLabel *label_9;
    QLabel *label_13;
    QCalendarWidget *calendarWidget;
    QSpacerItem *verticalSpacer;
    QComboBox *tasksComboBox;
    QLabel *label_14;
    QPushButton *addTaskButton;
    QPushButton *deleteTaskButton;
    QWidget *webControllTab;
    QGridLayout *gridLayout_27;
    QSpacerItem *verticalSpacer_8;
    QGroupBox *webUIGroupBox;
    QGridLayout *gridLayout_18;
    QGroupBox *IPFilterGroupBox;
    QGridLayout *gridLayout_16;
    QTextEdit *ipFilterTextEdit;
    QGroupBox *webUILogginGroupBox;
    QGridLayout *gridLayout_21;
    QHBoxLayout *horizontalLayout;
    QLabel *label_22;
    QLineEdit *logLineEdit;
    QLabel *label_10;
    QLabel *label_18;
    QCheckBox *upnpCheckBox;
    QLineEdit *passwordLineEdit;
    QLineEdit *loginLineEdit;
    QLineEdit *webPortLineEdit;
    QLabel *label_21;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *startRconButton;
    QPushButton *stopRconButton;
    QLabel *RunningLabel;
    QPushButton *openWebUiButton;
    QWidget *page_2;
    QGridLayout *gridLayout_33;
    QGroupBox *autosrtEmailNotificationCheckBox;
    QGridLayout *gridLayout_34;
    QLabel *label_36;
    QLabel *label_38;
    QLineEdit *rssSmtpPortEdit;
    QLineEdit *rssSmtpLoginEdit;
    QComboBox *rssAuthTypeComboBox;
    QLabel *label_25;
    QLineEdit *rssSmtpPasswordEdit;
    QLabel *label_35;
    QLineEdit *rssSmtpServerEdit;
    QLabel *label_37;
    QLabel *label_39;
    QLineEdit *rssRecepientEmailEdit;
    QLabel *label_40;
    QComboBox *rssSmtpConnTypeCombobox;
    QLabel *label_24;
    QSpacerItem *verticalSpacer_7;
    QPushButton *addRssDLRuleButton;
    QPushButton *removeRssDLRuleButton;
    QPushButton *importRssRulesButton;
    QListWidget *rssRulesListWidget;
    QSpacerItem *horizontalSpacer;
    QPushButton *exportRssRulesButton;
    QTimeEdit *rssRefrashRateEdit;
    QWidget *keyMappingTab;
    QGridLayout *gridLayout_17;
    QScrollArea *hotKeyScrollArea;
    QWidget *keyMapContainer;
    QHBoxLayout *horizontalLayout_5;

    void setupUi(QDialog *SettingsDialog)
    {
        if (SettingsDialog->objectName().isEmpty())
            SettingsDialog->setObjectName(QString::fromUtf8("SettingsDialog"));
        SettingsDialog->resize(843, 407);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/app.ico"), QSize(), QIcon::Normal, QIcon::Off);
        SettingsDialog->setWindowIcon(icon);
        SettingsDialog->setStyleSheet(QString::fromUtf8(""));
        verticalLayout = new QVBoxLayout(SettingsDialog);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(1, 1, 1, 0);
        titleBar = new QWidget(SettingsDialog);
        titleBar->setObjectName(QString::fromUtf8("titleBar"));
        titleBar->setMinimumSize(QSize(0, 23));
        titleBar->setMaximumSize(QSize(16777215, 23));
        titleBar->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_3 = new QHBoxLayout(titleBar);
        horizontalLayout_3->setSpacing(4);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(5, 0, 1, 0);
        tbMenu = new QLabel(titleBar);
        tbMenu->setObjectName(QString::fromUtf8("tbMenu"));
        tbMenu->setMaximumSize(QSize(15, 15));
        tbMenu->setPixmap(QPixmap(QString::fromUtf8(":/MenuIcons/preferencies.ico")));
        tbMenu->setScaledContents(true);

        horizontalLayout_3->addWidget(tbMenu);

        LTitle = new QLabel(titleBar);
        LTitle->setObjectName(QString::fromUtf8("LTitle"));
        LTitle->setMinimumSize(QSize(100, 22));
        LTitle->setMaximumSize(QSize(16777215, 22));
        LTitle->setStyleSheet(QString::fromUtf8(""));
        LTitle->setTextFormat(Qt::AutoText);

        horizontalLayout_3->addWidget(LTitle);

        pbClose = new QPushButton(titleBar);
        pbClose->setObjectName(QString::fromUtf8("pbClose"));
        pbClose->setMinimumSize(QSize(3, 22));
        pbClose->setMaximumSize(QSize(22, 22));
        pbClose->setIconSize(QSize(22, 22));
        pbClose->setFlat(true);

        horizontalLayout_3->addWidget(pbClose);


        verticalLayout->addWidget(titleBar);

        m_centralWidget = new QWidget(SettingsDialog);
        m_centralWidget->setObjectName(QString::fromUtf8("m_centralWidget"));
        gridLayout_4 = new QGridLayout(m_centralWidget);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setHorizontalSpacing(0);
        gridLayout_4->setContentsMargins(-1, 0, -1, 7);
        listWidget = new QListWidget(m_centralWidget);
        new QListWidgetItem(listWidget);
        new QListWidgetItem(listWidget);
        new QListWidgetItem(listWidget);
        new QListWidgetItem(listWidget);
        new QListWidgetItem(listWidget);
        new QListWidgetItem(listWidget);
        new QListWidgetItem(listWidget);
        new QListWidgetItem(listWidget);
        new QListWidgetItem(listWidget);
        new QListWidgetItem(listWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(listWidget->sizePolicy().hasHeightForWidth());
        listWidget->setSizePolicy(sizePolicy);
        listWidget->setMinimumSize(QSize(160, 0));
        listWidget->setMaximumSize(QSize(16777215, 16777215));
        listWidget->setStyleSheet(QString::fromUtf8("#listWidget{\n"
"    border-bottom:none;\n"
"    border-left:none;\n"
"}\n"
"QListView::item{\n"
"    min-height:28px;\n"
"}\n"
"                  "));
        listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listWidget->setResizeMode(QListView::Adjust);

        gridLayout_4->addWidget(listWidget, 0, 0, 3, 1);

        buttonBox = new QDialogButtonBox(m_centralWidget);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout_4->addWidget(buttonBox, 2, 1, 1, 1);

        stackedWidget = new QStackedWidget(m_centralWidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        torrentTab = new QWidget();
        torrentTab->setObjectName(QString::fromUtf8("torrentTab"));
        gridLayout_22 = new QGridLayout(torrentTab);
        gridLayout_22->setObjectName(QString::fromUtf8("gridLayout_22"));
        useNotificationsCheckBox = new QGroupBox(torrentTab);
        useNotificationsCheckBox->setObjectName(QString::fromUtf8("useNotificationsCheckBox"));
        useNotificationsCheckBox->setCheckable(true);
        useNotificationsCheckBox->setChecked(false);
        gridLayout_13 = new QGridLayout(useNotificationsCheckBox);
        gridLayout_13->setObjectName(QString::fromUtf8("gridLayout_13"));
        showTrackerErrorsCheckBox = new QCheckBox(useNotificationsCheckBox);
        showTrackerErrorsCheckBox->setObjectName(QString::fromUtf8("showTrackerErrorsCheckBox"));

        gridLayout_13->addWidget(showTrackerErrorsCheckBox, 0, 0, 1, 1);

        showDiskErrorsCheckBox = new QCheckBox(useNotificationsCheckBox);
        showDiskErrorsCheckBox->setObjectName(QString::fromUtf8("showDiskErrorsCheckBox"));

        gridLayout_13->addWidget(showDiskErrorsCheckBox, 1, 0, 1, 1);

        showRssErrorsCheckBox = new QCheckBox(useNotificationsCheckBox);
        showRssErrorsCheckBox->setObjectName(QString::fromUtf8("showRssErrorsCheckBox"));

        gridLayout_13->addWidget(showRssErrorsCheckBox, 2, 0, 1, 1);


        gridLayout_22->addWidget(useNotificationsCheckBox, 2, 2, 4, 2);

        trackerGroupBox = new QGroupBox(torrentTab);
        trackerGroupBox->setObjectName(QString::fromUtf8("trackerGroupBox"));
        trackerGroupBox->setCheckable(true);
        trackerGroupBox->setChecked(false);
        gridLayout_2 = new QGridLayout(trackerGroupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_23 = new QLabel(trackerGroupBox);
        label_23->setObjectName(QString::fromUtf8("label_23"));

        gridLayout_2->addWidget(label_23, 0, 0, 1, 1);

        trackerPortEdit = new QLineEdit(trackerGroupBox);
        trackerPortEdit->setObjectName(QString::fromUtf8("trackerPortEdit"));

        gridLayout_2->addWidget(trackerPortEdit, 0, 1, 1, 1);


        gridLayout_22->addWidget(trackerGroupBox, 0, 2, 2, 2);

        styleComboBox = new QComboBox(torrentTab);
        styleComboBox->setObjectName(QString::fromUtf8("styleComboBox"));

        gridLayout_22->addWidget(styleComboBox, 1, 1, 1, 1);

        localeComboBox = new QComboBox(torrentTab);
        localeComboBox->setObjectName(QString::fromUtf8("localeComboBox"));

        gridLayout_22->addWidget(localeComboBox, 0, 1, 1, 1);

        asociationCheckBox = new QCheckBox(torrentTab);
        asociationCheckBox->setObjectName(QString::fromUtf8("asociationCheckBox"));

        gridLayout_22->addWidget(asociationCheckBox, 4, 0, 1, 2);

        winShelItegrationCheckBox = new QCheckBox(torrentTab);
        winShelItegrationCheckBox->setObjectName(QString::fromUtf8("winShelItegrationCheckBox"));

        gridLayout_22->addWidget(winShelItegrationCheckBox, 6, 0, 1, 2);

        label_26 = new QLabel(torrentTab);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        gridLayout_22->addWidget(label_26, 1, 0, 1, 1);

        startMinimizedCheckBox = new QCheckBox(torrentTab);
        startMinimizedCheckBox->setObjectName(QString::fromUtf8("startMinimizedCheckBox"));

        gridLayout_22->addWidget(startMinimizedCheckBox, 3, 0, 1, 2);

        scriptDebugingCheckBox = new QCheckBox(torrentTab);
        scriptDebugingCheckBox->setObjectName(QString::fromUtf8("scriptDebugingCheckBox"));

        gridLayout_22->addWidget(scriptDebugingCheckBox, 6, 2, 1, 2);

        magnetAssociationCheckBox = new QCheckBox(torrentTab);
        magnetAssociationCheckBox->setObjectName(QString::fromUtf8("magnetAssociationCheckBox"));

        gridLayout_22->addWidget(magnetAssociationCheckBox, 5, 0, 1, 2);

        runOnbootCheckBox = new QCheckBox(torrentTab);
        runOnbootCheckBox->setObjectName(QString::fromUtf8("runOnbootCheckBox"));

        gridLayout_22->addWidget(runOnbootCheckBox, 2, 0, 1, 2);

        label_8 = new QLabel(torrentTab);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_22->addWidget(label_8, 0, 0, 1, 1);

        verticalSpacer_9 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_22->addItem(verticalSpacer_9, 9, 0, 1, 1);

        closeOnHideCheckBox = new QCheckBox(torrentTab);
        closeOnHideCheckBox->setObjectName(QString::fromUtf8("closeOnHideCheckBox"));

        gridLayout_22->addWidget(closeOnHideCheckBox, 7, 2, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_22->addItem(horizontalSpacer_4, 8, 2, 1, 1);

        watchDirEnabledCheckbox = new QGroupBox(torrentTab);
        watchDirEnabledCheckbox->setObjectName(QString::fromUtf8("watchDirEnabledCheckbox"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(watchDirEnabledCheckbox->sizePolicy().hasHeightForWidth());
        watchDirEnabledCheckbox->setSizePolicy(sizePolicy1);
        watchDirEnabledCheckbox->setCheckable(true);
        watchDirEnabledCheckbox->setChecked(false);
        gridLayout_35 = new QGridLayout(watchDirEnabledCheckbox);
        gridLayout_35->setObjectName(QString::fromUtf8("gridLayout_35"));
        gridLayout_23 = new QGridLayout();
        gridLayout_23->setObjectName(QString::fromUtf8("gridLayout_23"));
        useStaticPathRadioButton = new QRadioButton(watchDirEnabledCheckbox);
        useStaticPathRadioButton->setObjectName(QString::fromUtf8("useStaticPathRadioButton"));

        gridLayout_23->addWidget(useStaticPathRadioButton, 1, 0, 1, 1);

        showDialogRadioButton = new QRadioButton(watchDirEnabledCheckbox);
        showDialogRadioButton->setObjectName(QString::fromUtf8("showDialogRadioButton"));

        gridLayout_23->addWidget(showDialogRadioButton, 0, 0, 1, 1);

        usetTorrentFilteringRadioButton = new QRadioButton(watchDirEnabledCheckbox);
        usetTorrentFilteringRadioButton->setObjectName(QString::fromUtf8("usetTorrentFilteringRadioButton"));

        gridLayout_23->addWidget(usetTorrentFilteringRadioButton, 0, 1, 1, 3);

        browseWatchStaticPathButton = new QPushButton(watchDirEnabledCheckbox);
        browseWatchStaticPathButton->setObjectName(QString::fromUtf8("browseWatchStaticPathButton"));

        gridLayout_23->addWidget(browseWatchStaticPathButton, 1, 3, 1, 1);

        staticSavePathEdit = new QLineEdit(watchDirEnabledCheckbox);
        staticSavePathEdit->setObjectName(QString::fromUtf8("staticSavePathEdit"));

        gridLayout_23->addWidget(staticSavePathEdit, 1, 2, 1, 1);


        gridLayout_35->addLayout(gridLayout_23, 2, 0, 1, 3);

        watchDirPathEdit = new QLineEdit(watchDirEnabledCheckbox);
        watchDirPathEdit->setObjectName(QString::fromUtf8("watchDirPathEdit"));

        gridLayout_35->addWidget(watchDirPathEdit, 0, 1, 1, 1);

        label_43 = new QLabel(watchDirEnabledCheckbox);
        label_43->setObjectName(QString::fromUtf8("label_43"));

        gridLayout_35->addWidget(label_43, 0, 0, 1, 1);

        browseWatchDirButton = new QPushButton(watchDirEnabledCheckbox);
        browseWatchDirButton->setObjectName(QString::fromUtf8("browseWatchDirButton"));

        gridLayout_35->addWidget(browseWatchDirButton, 0, 2, 1, 1);

        checkBox = new QCheckBox(watchDirEnabledCheckbox);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        gridLayout_35->addWidget(checkBox, 3, 0, 1, 2);


        gridLayout_22->addWidget(watchDirEnabledCheckbox, 7, 0, 2, 2);

        stackedWidget->addWidget(torrentTab);
        restrictionTab = new QWidget();
        restrictionTab->setObjectName(QString::fromUtf8("restrictionTab"));
        gridLayout_30 = new QGridLayout(restrictionTab);
        gridLayout_30->setObjectName(QString::fromUtf8("gridLayout_30"));
        groupBox_12 = new QGroupBox(restrictionTab);
        groupBox_12->setObjectName(QString::fromUtf8("groupBox_12"));
        gridLayout_29 = new QGridLayout(groupBox_12);
        gridLayout_29->setObjectName(QString::fromUtf8("gridLayout_29"));
        usePExCheckBox = new QCheckBox(groupBox_12);
        usePExCheckBox->setObjectName(QString::fromUtf8("usePExCheckBox"));

        gridLayout_29->addWidget(usePExCheckBox, 1, 0, 1, 1);

        useLSDCheckBox = new QCheckBox(groupBox_12);
        useLSDCheckBox->setObjectName(QString::fromUtf8("useLSDCheckBox"));

        gridLayout_29->addWidget(useLSDCheckBox, 2, 0, 1, 1);

        useDHTCheckBox = new QCheckBox(groupBox_12);
        useDHTCheckBox->setObjectName(QString::fromUtf8("useDHTCheckBox"));

        gridLayout_29->addWidget(useDHTCheckBox, 0, 0, 1, 1);


        gridLayout_30->addWidget(groupBox_12, 1, 0, 2, 2);

        groupBox_2 = new QGroupBox(restrictionTab);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_31 = new QGridLayout(groupBox_2);
        gridLayout_31->setObjectName(QString::fromUtf8("gridLayout_31"));
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_31->addWidget(label_6, 0, 0, 1, 1);

        uploadLimitEdit = new QSpinBox(groupBox_2);
        uploadLimitEdit->setObjectName(QString::fromUtf8("uploadLimitEdit"));
        uploadLimitEdit->setMaximum(99999);

        gridLayout_31->addWidget(uploadLimitEdit, 0, 1, 1, 1);

        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_31->addWidget(label_7, 1, 0, 1, 1);

        downloadLimitEdit = new QSpinBox(groupBox_2);
        downloadLimitEdit->setObjectName(QString::fromUtf8("downloadLimitEdit"));
        downloadLimitEdit->setMaximum(99999);

        gridLayout_31->addWidget(downloadLimitEdit, 1, 1, 1, 1);

        label_33 = new QLabel(groupBox_2);
        label_33->setObjectName(QString::fromUtf8("label_33"));

        gridLayout_31->addWidget(label_33, 2, 0, 1, 1);

        dhtLimitEdit = new QSpinBox(groupBox_2);
        dhtLimitEdit->setObjectName(QString::fromUtf8("dhtLimitEdit"));
        dhtLimitEdit->setMaximum(99999);

        gridLayout_31->addWidget(dhtLimitEdit, 2, 1, 1, 1);

        limitLocalConnectionCheckBox = new QGroupBox(groupBox_2);
        limitLocalConnectionCheckBox->setObjectName(QString::fromUtf8("limitLocalConnectionCheckBox"));
        limitLocalConnectionCheckBox->setCheckable(true);
        limitLocalConnectionCheckBox->setChecked(false);
        gridLayout_32 = new QGridLayout(limitLocalConnectionCheckBox);
        gridLayout_32->setObjectName(QString::fromUtf8("gridLayout_32"));
        label_31 = new QLabel(limitLocalConnectionCheckBox);
        label_31->setObjectName(QString::fromUtf8("label_31"));

        gridLayout_32->addWidget(label_31, 0, 0, 1, 1);

        localUploadLimitEdit = new QSpinBox(limitLocalConnectionCheckBox);
        localUploadLimitEdit->setObjectName(QString::fromUtf8("localUploadLimitEdit"));
        localUploadLimitEdit->setMaximum(99999);

        gridLayout_32->addWidget(localUploadLimitEdit, 0, 1, 1, 1);

        label_32 = new QLabel(limitLocalConnectionCheckBox);
        label_32->setObjectName(QString::fromUtf8("label_32"));

        gridLayout_32->addWidget(label_32, 1, 0, 1, 1);

        localDownloadLimitEdit = new QSpinBox(limitLocalConnectionCheckBox);
        localDownloadLimitEdit->setObjectName(QString::fromUtf8("localDownloadLimitEdit"));
        localDownloadLimitEdit->setMaximum(99999);

        gridLayout_32->addWidget(localDownloadLimitEdit, 1, 1, 1, 1);


        gridLayout_31->addWidget(limitLocalConnectionCheckBox, 3, 0, 1, 2);

        limitUtpCheckBox = new QCheckBox(groupBox_2);
        limitUtpCheckBox->setObjectName(QString::fromUtf8("limitUtpCheckBox"));

        gridLayout_31->addWidget(limitUtpCheckBox, 4, 0, 1, 2);


        gridLayout_30->addWidget(groupBox_2, 0, 2, 3, 2);

        seedGlobalRatioEdit = new QDoubleSpinBox(restrictionTab);
        seedGlobalRatioEdit->setObjectName(QString::fromUtf8("seedGlobalRatioEdit"));

        gridLayout_30->addWidget(seedGlobalRatioEdit, 3, 1, 1, 1);

        maxConnectionsPerTorrentEdit = new QSpinBox(restrictionTab);
        maxConnectionsPerTorrentEdit->setObjectName(QString::fromUtf8("maxConnectionsPerTorrentEdit"));
        maxConnectionsPerTorrentEdit->setMinimum(50);
        maxConnectionsPerTorrentEdit->setMaximum(65536);
        maxConnectionsPerTorrentEdit->setSingleStep(10);

        gridLayout_30->addWidget(maxConnectionsPerTorrentEdit, 4, 1, 1, 1);

        label_30 = new QLabel(restrictionTab);
        label_30->setObjectName(QString::fromUtf8("label_30"));

        gridLayout_30->addWidget(label_30, 3, 2, 1, 1);

        seedTimeLimitEdit = new QTimeEdit(restrictionTab);
        seedTimeLimitEdit->setObjectName(QString::fromUtf8("seedTimeLimitEdit"));
        seedTimeLimitEdit->setDisplayFormat(QString::fromUtf8("HH:mm:ss"));

        gridLayout_30->addWidget(seedTimeLimitEdit, 3, 3, 1, 1);

        label_34 = new QLabel(restrictionTab);
        label_34->setObjectName(QString::fromUtf8("label_34"));

        gridLayout_30->addWidget(label_34, 3, 0, 1, 1);

        label_41 = new QLabel(restrictionTab);
        label_41->setObjectName(QString::fromUtf8("label_41"));

        gridLayout_30->addWidget(label_41, 4, 0, 1, 1);

        restrictionsGroupBox = new QGroupBox(restrictionTab);
        restrictionsGroupBox->setObjectName(QString::fromUtf8("restrictionsGroupBox"));
        gridLayout_3 = new QGridLayout(restrictionsGroupBox);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        activeSeedLimitEdit = new QSpinBox(restrictionsGroupBox);
        activeSeedLimitEdit->setObjectName(QString::fromUtf8("activeSeedLimitEdit"));
        activeSeedLimitEdit->setMinimum(-1);
        activeSeedLimitEdit->setMaximum(999999);
        activeSeedLimitEdit->setSingleStep(5);
        activeSeedLimitEdit->setValue(-1);

        gridLayout_3->addWidget(activeSeedLimitEdit, 1, 1, 1, 1);

        activeLimitEdit = new QSpinBox(restrictionsGroupBox);
        activeLimitEdit->setObjectName(QString::fromUtf8("activeLimitEdit"));
        activeLimitEdit->setMinimum(-1);
        activeLimitEdit->setMaximum(999999);
        activeLimitEdit->setSingleStep(5);
        activeLimitEdit->setValue(-1);

        gridLayout_3->addWidget(activeLimitEdit, 0, 1, 1, 1);

        activeDownloadLimitEdit = new QSpinBox(restrictionsGroupBox);
        activeDownloadLimitEdit->setObjectName(QString::fromUtf8("activeDownloadLimitEdit"));
        activeDownloadLimitEdit->setMinimum(-1);
        activeDownloadLimitEdit->setMaximum(999999);
        activeDownloadLimitEdit->setSingleStep(5);
        activeDownloadLimitEdit->setValue(-1);

        gridLayout_3->addWidget(activeDownloadLimitEdit, 2, 1, 1, 1);

        label_17 = new QLabel(restrictionsGroupBox);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        gridLayout_3->addWidget(label_17, 2, 0, 1, 1);

        label_16 = new QLabel(restrictionsGroupBox);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        gridLayout_3->addWidget(label_16, 1, 0, 1, 1);

        label_15 = new QLabel(restrictionsGroupBox);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout_3->addWidget(label_15, 0, 0, 1, 1);


        gridLayout_30->addWidget(restrictionsGroupBox, 0, 0, 1, 2);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_30->addItem(verticalSpacer_6, 5, 0, 1, 4);

        label_45 = new QLabel(restrictionTab);
        label_45->setObjectName(QString::fromUtf8("label_45"));

        gridLayout_30->addWidget(label_45, 4, 2, 1, 1);

        maxConnectionsEdit = new QSpinBox(restrictionTab);
        maxConnectionsEdit->setObjectName(QString::fromUtf8("maxConnectionsEdit"));
        maxConnectionsEdit->setMaximum(65536);

        gridLayout_30->addWidget(maxConnectionsEdit, 4, 3, 1, 1);

        stackedWidget->addWidget(restrictionTab);
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        gridLayout_20 = new QGridLayout(page);
        gridLayout_20->setObjectName(QString::fromUtf8("gridLayout_20"));
        portEdit = new QLineEdit(page);
        portEdit->setObjectName(QString::fromUtf8("portEdit"));
        portEdit->setMaxLength(5);

        gridLayout_20->addWidget(portEdit, 0, 1, 1, 1);

        label = new QLabel(page);
        label->setObjectName(QString::fromUtf8("label"));
        label->setLayoutDirection(Qt::LeftToRight);
        label->setAutoFillBackground(false);
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_20->addWidget(label, 0, 0, 1, 1);

        groupBox_10 = new QGroupBox(page);
        groupBox_10->setObjectName(QString::fromUtf8("groupBox_10"));
        groupBox_10->setCheckable(false);
        gridLayout_28 = new QGridLayout(groupBox_10);
        gridLayout_28->setObjectName(QString::fromUtf8("gridLayout_28"));
        label_27 = new QLabel(groupBox_10);
        label_27->setObjectName(QString::fromUtf8("label_27"));

        gridLayout_28->addWidget(label_27, 0, 0, 1, 1);

        inEncPolicyComboBox = new QComboBox(groupBox_10);
        inEncPolicyComboBox->setObjectName(QString::fromUtf8("inEncPolicyComboBox"));
        inEncPolicyComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        gridLayout_28->addWidget(inEncPolicyComboBox, 0, 1, 1, 1);

        label_28 = new QLabel(groupBox_10);
        label_28->setObjectName(QString::fromUtf8("label_28"));

        gridLayout_28->addWidget(label_28, 1, 0, 1, 1);

        outEncPolicyComboBox = new QComboBox(groupBox_10);
        outEncPolicyComboBox->setObjectName(QString::fromUtf8("outEncPolicyComboBox"));
        outEncPolicyComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        gridLayout_28->addWidget(outEncPolicyComboBox, 1, 1, 1, 1);

        label_29 = new QLabel(groupBox_10);
        label_29->setObjectName(QString::fromUtf8("label_29"));

        gridLayout_28->addWidget(label_29, 2, 0, 1, 1);

        encLevelComboBox = new QComboBox(groupBox_10);
        encLevelComboBox->setObjectName(QString::fromUtf8("encLevelComboBox"));
        encLevelComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        gridLayout_28->addWidget(encLevelComboBox, 2, 1, 1, 1);

        preferFullEncCheckBox = new QCheckBox(groupBox_10);
        preferFullEncCheckBox->setObjectName(QString::fromUtf8("preferFullEncCheckBox"));

        gridLayout_28->addWidget(preferFullEncCheckBox, 3, 0, 1, 2);


        gridLayout_20->addWidget(groupBox_10, 0, 3, 3, 1);

        proxyGroupBox = new QGroupBox(page);
        proxyGroupBox->setObjectName(QString::fromUtf8("proxyGroupBox"));
        proxyGroupBox->setEnabled(true);
        proxyGroupBox->setCheckable(true);
        proxyGroupBox->setChecked(false);
        gridLayout = new QGridLayout(proxyGroupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_3 = new QLabel(proxyGroupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        proxyHostEdit = new QLineEdit(proxyGroupBox);
        proxyHostEdit->setObjectName(QString::fromUtf8("proxyHostEdit"));
        proxyHostEdit->setMaxLength(21);

        gridLayout->addWidget(proxyHostEdit, 0, 1, 1, 1);

        proxyUsernameEdit = new QLineEdit(proxyGroupBox);
        proxyUsernameEdit->setObjectName(QString::fromUtf8("proxyUsernameEdit"));

        gridLayout->addWidget(proxyUsernameEdit, 2, 1, 1, 1);

        label_5 = new QLabel(proxyGroupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 4, 0, 1, 1);

        label_4 = new QLabel(proxyGroupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        label_2 = new QLabel(proxyGroupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        proxyPwdEdit = new QLineEdit(proxyGroupBox);
        proxyPwdEdit->setObjectName(QString::fromUtf8("proxyPwdEdit"));

        gridLayout->addWidget(proxyPwdEdit, 3, 1, 1, 1);

        proxyTypeComboBox = new QComboBox(proxyGroupBox);
        proxyTypeComboBox->setObjectName(QString::fromUtf8("proxyTypeComboBox"));

        gridLayout->addWidget(proxyTypeComboBox, 4, 1, 1, 1);

        label_44 = new QLabel(proxyGroupBox);
        label_44->setObjectName(QString::fromUtf8("label_44"));

        gridLayout->addWidget(label_44, 1, 0, 1, 1);

        proxyPortEdit = new QLineEdit(proxyGroupBox);
        proxyPortEdit->setObjectName(QString::fromUtf8("proxyPortEdit"));

        gridLayout->addWidget(proxyPortEdit, 1, 1, 1, 1);


        gridLayout_20->addWidget(proxyGroupBox, 2, 0, 2, 3);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_20->addItem(verticalSpacer_5, 3, 3, 2, 1);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_20->addItem(verticalSpacer_4, 4, 0, 1, 3);

        useRandomPortCheckBox = new QCheckBox(page);
        useRandomPortCheckBox->setObjectName(QString::fromUtf8("useRandomPortCheckBox"));

        gridLayout_20->addWidget(useRandomPortCheckBox, 0, 2, 1, 1);

        portMappingsCheckBox = new QCheckBox(page);
        portMappingsCheckBox->setObjectName(QString::fromUtf8("portMappingsCheckBox"));

        gridLayout_20->addWidget(portMappingsCheckBox, 1, 0, 1, 3);

        stackedWidget->addWidget(page);
        hddTab = new QWidget();
        hddTab->setObjectName(QString::fromUtf8("hddTab"));
        gridLayout_12 = new QGridLayout(hddTab);
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        useDiskReadAheadCheckBox = new QCheckBox(hddTab);
        useDiskReadAheadCheckBox->setObjectName(QString::fromUtf8("useDiskReadAheadCheckBox"));

        gridLayout_12->addWidget(useDiskReadAheadCheckBox, 3, 0, 1, 1);

        alowReorderedOpsCheckBox = new QCheckBox(hddTab);
        alowReorderedOpsCheckBox->setObjectName(QString::fromUtf8("alowReorderedOpsCheckBox"));

        gridLayout_12->addWidget(alowReorderedOpsCheckBox, 3, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(212, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_12->addItem(horizontalSpacer_2, 3, 2, 1, 1);

        label_42 = new QLabel(hddTab);
        label_42->setObjectName(QString::fromUtf8("label_42"));

        gridLayout_12->addWidget(label_42, 2, 0, 1, 1);

        casheSizeLineEdit = new QSpinBox(hddTab);
        casheSizeLineEdit->setObjectName(QString::fromUtf8("casheSizeLineEdit"));
        casheSizeLineEdit->setMaximum(512000);

        gridLayout_12->addWidget(casheSizeLineEdit, 1, 1, 1, 1);

        diskAllocationComboBox = new QComboBox(hddTab);
        diskAllocationComboBox->setObjectName(QString::fromUtf8("diskAllocationComboBox"));

        gridLayout_12->addWidget(diskAllocationComboBox, 2, 1, 1, 1);

        lockFilesCheckBox = new QCheckBox(hddTab);
        lockFilesCheckBox->setObjectName(QString::fromUtf8("lockFilesCheckBox"));

        gridLayout_12->addWidget(lockFilesCheckBox, 5, 0, 1, 1);

        useReadCasheCheckBox = new QCheckBox(hddTab);
        useReadCasheCheckBox->setObjectName(QString::fromUtf8("useReadCasheCheckBox"));

        gridLayout_12->addWidget(useReadCasheCheckBox, 4, 1, 1, 1);

        lowPrioDiskCheckBox = new QCheckBox(hddTab);
        lowPrioDiskCheckBox->setObjectName(QString::fromUtf8("lowPrioDiskCheckBox"));

        gridLayout_12->addWidget(lowPrioDiskCheckBox, 4, 0, 1, 1);

        label_19 = new QLabel(hddTab);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        gridLayout_12->addWidget(label_19, 0, 0, 1, 1);

        diskIOCasheModeComboBox = new QComboBox(hddTab);
        diskIOCasheModeComboBox->setObjectName(QString::fromUtf8("diskIOCasheModeComboBox"));

        gridLayout_12->addWidget(diskIOCasheModeComboBox, 0, 1, 1, 1);

        label_20 = new QLabel(hddTab);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        gridLayout_12->addWidget(label_20, 1, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_12->addItem(verticalSpacer_3, 6, 0, 1, 1);

        stackedWidget->addWidget(hddTab);
        filteringTab = new QWidget();
        filteringTab->setObjectName(QString::fromUtf8("filteringTab"));
        gridLayout_5 = new QGridLayout(filteringTab);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        groupBox_9 = new QGroupBox(filteringTab);
        groupBox_9->setObjectName(QString::fromUtf8("groupBox_9"));
        gridLayout_8 = new QGridLayout(groupBox_9);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        groupSavePathEdit = new QLineEdit(groupBox_9);
        groupSavePathEdit->setObjectName(QString::fromUtf8("groupSavePathEdit"));

        gridLayout_8->addWidget(groupSavePathEdit, 3, 1, 1, 1);

        BrowseGroupSavePathButton = new QPushButton(groupBox_9);
        BrowseGroupSavePathButton->setObjectName(QString::fromUtf8("BrowseGroupSavePathButton"));

        gridLayout_8->addWidget(BrowseGroupSavePathButton, 3, 2, 1, 1);

        label_47 = new QLabel(groupBox_9);
        label_47->setObjectName(QString::fromUtf8("label_47"));
        label_47->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label_47->setWordWrap(true);

        gridLayout_8->addWidget(label_47, 1, 0, 2, 1);

        label_48 = new QLabel(groupBox_9);
        label_48->setObjectName(QString::fromUtf8("label_48"));

        gridLayout_8->addWidget(label_48, 3, 0, 1, 1);

        extensionsEdit = new QTextEdit(groupBox_9);
        extensionsEdit->setObjectName(QString::fromUtf8("extensionsEdit"));

        gridLayout_8->addWidget(extensionsEdit, 1, 1, 2, 2);

        newGroupNameEdit = new QLineEdit(groupBox_9);
        newGroupNameEdit->setObjectName(QString::fromUtf8("newGroupNameEdit"));

        gridLayout_8->addWidget(newGroupNameEdit, 0, 1, 1, 2);

        label_46 = new QLabel(groupBox_9);
        label_46->setObjectName(QString::fromUtf8("label_46"));

        gridLayout_8->addWidget(label_46, 0, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);

        addNewGroupButton = new QPushButton(groupBox_9);
        addNewGroupButton->setObjectName(QString::fromUtf8("addNewGroupButton"));

        horizontalLayout_4->addWidget(addNewGroupButton);

        updateGroupButton = new QPushButton(groupBox_9);
        updateGroupButton->setObjectName(QString::fromUtf8("updateGroupButton"));

        horizontalLayout_4->addWidget(updateGroupButton);

        removeGroupButton = new QPushButton(groupBox_9);
        removeGroupButton->setObjectName(QString::fromUtf8("removeGroupButton"));

        horizontalLayout_4->addWidget(removeGroupButton);


        gridLayout_8->addLayout(horizontalLayout_4, 4, 0, 1, 3);


        gridLayout_5->addWidget(groupBox_9, 0, 1, 1, 1);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_9, 1, 2, 1, 1);

        GroupsTreeWidget = new QTreeWidget(filteringTab);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        GroupsTreeWidget->setHeaderItem(__qtreewidgetitem);
        GroupsTreeWidget->setObjectName(QString::fromUtf8("GroupsTreeWidget"));
        GroupsTreeWidget->setMinimumSize(QSize(150, 0));
        GroupsTreeWidget->setRootIsDecorated(false);
        GroupsTreeWidget->header()->setVisible(false);

        gridLayout_5->addWidget(GroupsTreeWidget, 0, 0, 1, 1);

        stackedWidget->addWidget(filteringTab);
        dtTab = new QWidget();
        dtTab->setObjectName(QString::fromUtf8("dtTab"));
        gridLayout_25 = new QGridLayout(dtTab);
        gridLayout_25->setObjectName(QString::fromUtf8("gridLayout_25"));
        groupBox_6 = new QGroupBox(dtTab);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        gridLayout_9 = new QGridLayout(groupBox_6);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        DTPathEdit = new QLineEdit(groupBox_6);
        DTPathEdit->setObjectName(QString::fromUtf8("DTPathEdit"));

        gridLayout_9->addWidget(DTPathEdit, 0, 0, 1, 1);

        browseDTPathButton = new QPushButton(groupBox_6);
        browseDTPathButton->setObjectName(QString::fromUtf8("browseDTPathButton"));

        gridLayout_9->addWidget(browseDTPathButton, 0, 1, 1, 1);


        gridLayout_25->addWidget(groupBox_6, 0, 0, 1, 1);

        groupBox_8 = new QGroupBox(dtTab);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        gridLayout_11 = new QGridLayout(groupBox_8);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        driveNumberComboBox = new QComboBox(groupBox_8);
        driveNumberComboBox->insertItems(0, QStringList()
         << QString::fromUtf8("1")
         << QString::fromUtf8("2")
         << QString::fromUtf8("3")
         << QString::fromUtf8("4")
         << QString::fromUtf8("5")
         << QString::fromUtf8("6")
         << QString::fromUtf8("7")
         << QString::fromUtf8("8")
         << QString::fromUtf8("9")
         << QString::fromUtf8("10")
        );
        driveNumberComboBox->setObjectName(QString::fromUtf8("driveNumberComboBox"));

        gridLayout_11->addWidget(driveNumberComboBox, 0, 0, 1, 1);


        gridLayout_25->addWidget(groupBox_8, 0, 1, 1, 1);

        customMoutGroupBox = new QGroupBox(dtTab);
        customMoutGroupBox->setObjectName(QString::fromUtf8("customMoutGroupBox"));
        customMoutGroupBox->setEnabled(true);
        customMoutGroupBox->setCheckable(true);
        customMoutGroupBox->setChecked(false);
        gridLayout_10 = new QGridLayout(customMoutGroupBox);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        customCommandEdit = new QLineEdit(customMoutGroupBox);
        customCommandEdit->setObjectName(QString::fromUtf8("customCommandEdit"));

        gridLayout_10->addWidget(customCommandEdit, 0, 0, 1, 1);


        gridLayout_25->addWidget(customMoutGroupBox, 1, 0, 1, 2);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_25->addItem(verticalSpacer_2, 2, 0, 1, 1);

        stackedWidget->addWidget(dtTab);
        schedulingTab = new QWidget();
        schedulingTab->setObjectName(QString::fromUtf8("schedulingTab"));
        gridLayout_26 = new QGridLayout(schedulingTab);
        gridLayout_26->setObjectName(QString::fromUtf8("gridLayout_26"));
        groupBox_11 = new QGroupBox(schedulingTab);
        groupBox_11->setObjectName(QString::fromUtf8("groupBox_11"));
        gridLayout_14 = new QGridLayout(groupBox_11);
        gridLayout_14->setObjectName(QString::fromUtf8("gridLayout_14"));
        groupBox = new QGroupBox(groupBox_11);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_15 = new QGridLayout(groupBox);
        gridLayout_15->setObjectName(QString::fromUtf8("gridLayout_15"));
        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_15->addWidget(label_11, 2, 0, 1, 1);

        dlLimitEdit = new QLineEdit(groupBox);
        dlLimitEdit->setObjectName(QString::fromUtf8("dlLimitEdit"));
        dlLimitEdit->setEnabled(true);

        gridLayout_15->addWidget(dlLimitEdit, 2, 1, 1, 1);

        label_12 = new QLabel(groupBox);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_15->addWidget(label_12, 2, 2, 1, 1);

        ulLimitEdit = new QLineEdit(groupBox);
        ulLimitEdit->setObjectName(QString::fromUtf8("ulLimitEdit"));
        ulLimitEdit->setEnabled(true);

        gridLayout_15->addWidget(ulLimitEdit, 2, 3, 1, 1);

        startAllRadioButton = new QRadioButton(groupBox);
        startAllRadioButton->setObjectName(QString::fromUtf8("startAllRadioButton"));

        gridLayout_15->addWidget(startAllRadioButton, 0, 2, 1, 2);

        limitUlRadioButton = new QRadioButton(groupBox);
        limitUlRadioButton->setObjectName(QString::fromUtf8("limitUlRadioButton"));

        gridLayout_15->addWidget(limitUlRadioButton, 1, 2, 1, 2);

        limitDlRadioButton = new QRadioButton(groupBox);
        limitDlRadioButton->setObjectName(QString::fromUtf8("limitDlRadioButton"));

        gridLayout_15->addWidget(limitDlRadioButton, 1, 0, 1, 2);

        pauseAllRadioButton = new QRadioButton(groupBox);
        pauseAllRadioButton->setObjectName(QString::fromUtf8("pauseAllRadioButton"));

        gridLayout_15->addWidget(pauseAllRadioButton, 0, 0, 1, 2);


        gridLayout_14->addWidget(groupBox, 2, 0, 1, 2);

        beginDateTimeEdit = new QDateTimeEdit(groupBox_11);
        beginDateTimeEdit->setObjectName(QString::fromUtf8("beginDateTimeEdit"));

        gridLayout_14->addWidget(beginDateTimeEdit, 1, 1, 1, 1);

        taskNameLineEdit = new QLineEdit(groupBox_11);
        taskNameLineEdit->setObjectName(QString::fromUtf8("taskNameLineEdit"));
        taskNameLineEdit->setMaximumSize(QSize(16777215, 16777215));

        gridLayout_14->addWidget(taskNameLineEdit, 0, 1, 1, 1);

        label_9 = new QLabel(groupBox_11);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_14->addWidget(label_9, 1, 0, 1, 1);

        label_13 = new QLabel(groupBox_11);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_14->addWidget(label_13, 0, 0, 1, 1);

        calendarWidget = new QCalendarWidget(groupBox_11);
        calendarWidget->setObjectName(QString::fromUtf8("calendarWidget"));

        gridLayout_14->addWidget(calendarWidget, 0, 2, 3, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_14->addItem(verticalSpacer, 3, 0, 1, 2);

        label_9->raise();
        beginDateTimeEdit->raise();
        groupBox->raise();
        label_13->raise();
        taskNameLineEdit->raise();
        calendarWidget->raise();

        gridLayout_26->addWidget(groupBox_11, 1, 0, 1, 4);

        tasksComboBox = new QComboBox(schedulingTab);
        tasksComboBox->setObjectName(QString::fromUtf8("tasksComboBox"));
        tasksComboBox->setMaximumSize(QSize(150, 16777215));

        gridLayout_26->addWidget(tasksComboBox, 0, 1, 1, 1);

        label_14 = new QLabel(schedulingTab);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setMaximumSize(QSize(150, 16777215));

        gridLayout_26->addWidget(label_14, 0, 0, 1, 1);

        addTaskButton = new QPushButton(schedulingTab);
        addTaskButton->setObjectName(QString::fromUtf8("addTaskButton"));

        gridLayout_26->addWidget(addTaskButton, 0, 2, 1, 1);

        deleteTaskButton = new QPushButton(schedulingTab);
        deleteTaskButton->setObjectName(QString::fromUtf8("deleteTaskButton"));

        gridLayout_26->addWidget(deleteTaskButton, 0, 3, 1, 1);

        stackedWidget->addWidget(schedulingTab);
        webControllTab = new QWidget();
        webControllTab->setObjectName(QString::fromUtf8("webControllTab"));
        gridLayout_27 = new QGridLayout(webControllTab);
        gridLayout_27->setObjectName(QString::fromUtf8("gridLayout_27"));
        verticalSpacer_8 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_27->addItem(verticalSpacer_8, 1, 0, 1, 1);

        webUIGroupBox = new QGroupBox(webControllTab);
        webUIGroupBox->setObjectName(QString::fromUtf8("webUIGroupBox"));
        webUIGroupBox->setMaximumSize(QSize(100500, 100500));
        webUIGroupBox->setCheckable(true);
        webUIGroupBox->setChecked(false);
        gridLayout_18 = new QGridLayout(webUIGroupBox);
        gridLayout_18->setObjectName(QString::fromUtf8("gridLayout_18"));
        IPFilterGroupBox = new QGroupBox(webUIGroupBox);
        IPFilterGroupBox->setObjectName(QString::fromUtf8("IPFilterGroupBox"));
        IPFilterGroupBox->setCheckable(true);
        IPFilterGroupBox->setChecked(false);
        gridLayout_16 = new QGridLayout(IPFilterGroupBox);
        gridLayout_16->setObjectName(QString::fromUtf8("gridLayout_16"));
        ipFilterTextEdit = new QTextEdit(IPFilterGroupBox);
        ipFilterTextEdit->setObjectName(QString::fromUtf8("ipFilterTextEdit"));

        gridLayout_16->addWidget(ipFilterTextEdit, 0, 0, 1, 1);


        gridLayout_18->addWidget(IPFilterGroupBox, 0, 3, 8, 1);

        webUILogginGroupBox = new QGroupBox(webUIGroupBox);
        webUILogginGroupBox->setObjectName(QString::fromUtf8("webUILogginGroupBox"));
        webUILogginGroupBox->setMaximumSize(QSize(16777215, 16777215));
        webUILogginGroupBox->setCheckable(true);
        webUILogginGroupBox->setChecked(false);
        gridLayout_21 = new QGridLayout(webUILogginGroupBox);
        gridLayout_21->setObjectName(QString::fromUtf8("gridLayout_21"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_22 = new QLabel(webUILogginGroupBox);
        label_22->setObjectName(QString::fromUtf8("label_22"));

        horizontalLayout->addWidget(label_22);

        logLineEdit = new QLineEdit(webUILogginGroupBox);
        logLineEdit->setObjectName(QString::fromUtf8("logLineEdit"));

        horizontalLayout->addWidget(logLineEdit);


        gridLayout_21->addLayout(horizontalLayout, 1, 0, 1, 3);


        gridLayout_18->addWidget(webUILogginGroupBox, 7, 0, 1, 3);

        label_10 = new QLabel(webUIGroupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_18->addWidget(label_10, 0, 0, 1, 1);

        label_18 = new QLabel(webUIGroupBox);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        gridLayout_18->addWidget(label_18, 1, 0, 1, 1);

        upnpCheckBox = new QCheckBox(webUIGroupBox);
        upnpCheckBox->setObjectName(QString::fromUtf8("upnpCheckBox"));

        gridLayout_18->addWidget(upnpCheckBox, 3, 0, 1, 1);

        passwordLineEdit = new QLineEdit(webUIGroupBox);
        passwordLineEdit->setObjectName(QString::fromUtf8("passwordLineEdit"));
        passwordLineEdit->setEchoMode(QLineEdit::Password);

        gridLayout_18->addWidget(passwordLineEdit, 1, 1, 1, 2);

        loginLineEdit = new QLineEdit(webUIGroupBox);
        loginLineEdit->setObjectName(QString::fromUtf8("loginLineEdit"));

        gridLayout_18->addWidget(loginLineEdit, 0, 1, 1, 2);

        webPortLineEdit = new QLineEdit(webUIGroupBox);
        webPortLineEdit->setObjectName(QString::fromUtf8("webPortLineEdit"));

        gridLayout_18->addWidget(webPortLineEdit, 2, 1, 1, 2);

        label_21 = new QLabel(webUIGroupBox);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        gridLayout_18->addWidget(label_21, 2, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        startRconButton = new QPushButton(webUIGroupBox);
        startRconButton->setObjectName(QString::fromUtf8("startRconButton"));

        horizontalLayout_2->addWidget(startRconButton);

        stopRconButton = new QPushButton(webUIGroupBox);
        stopRconButton->setObjectName(QString::fromUtf8("stopRconButton"));

        horizontalLayout_2->addWidget(stopRconButton);

        RunningLabel = new QLabel(webUIGroupBox);
        RunningLabel->setObjectName(QString::fromUtf8("RunningLabel"));
        RunningLabel->setEnabled(false);
        RunningLabel->setMaximumSize(QSize(16, 16));
        RunningLabel->setPixmap(QPixmap(QString::fromUtf8(":/icons/upload.ico")));
        RunningLabel->setScaledContents(true);
        RunningLabel->setWordWrap(false);

        horizontalLayout_2->addWidget(RunningLabel);


        gridLayout_18->addLayout(horizontalLayout_2, 5, 0, 1, 3);

        openWebUiButton = new QPushButton(webUIGroupBox);
        openWebUiButton->setObjectName(QString::fromUtf8("openWebUiButton"));

        gridLayout_18->addWidget(openWebUiButton, 3, 1, 1, 2);


        gridLayout_27->addWidget(webUIGroupBox, 0, 0, 1, 1);

        stackedWidget->addWidget(webControllTab);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        gridLayout_33 = new QGridLayout(page_2);
        gridLayout_33->setObjectName(QString::fromUtf8("gridLayout_33"));
        autosrtEmailNotificationCheckBox = new QGroupBox(page_2);
        autosrtEmailNotificationCheckBox->setObjectName(QString::fromUtf8("autosrtEmailNotificationCheckBox"));
        autosrtEmailNotificationCheckBox->setCheckable(true);
        autosrtEmailNotificationCheckBox->setChecked(false);
        gridLayout_34 = new QGridLayout(autosrtEmailNotificationCheckBox);
        gridLayout_34->setObjectName(QString::fromUtf8("gridLayout_34"));
        label_36 = new QLabel(autosrtEmailNotificationCheckBox);
        label_36->setObjectName(QString::fromUtf8("label_36"));

        gridLayout_34->addWidget(label_36, 3, 0, 1, 1);

        label_38 = new QLabel(autosrtEmailNotificationCheckBox);
        label_38->setObjectName(QString::fromUtf8("label_38"));

        gridLayout_34->addWidget(label_38, 5, 0, 1, 1);

        rssSmtpPortEdit = new QLineEdit(autosrtEmailNotificationCheckBox);
        rssSmtpPortEdit->setObjectName(QString::fromUtf8("rssSmtpPortEdit"));

        gridLayout_34->addWidget(rssSmtpPortEdit, 1, 1, 1, 1);

        rssSmtpLoginEdit = new QLineEdit(autosrtEmailNotificationCheckBox);
        rssSmtpLoginEdit->setObjectName(QString::fromUtf8("rssSmtpLoginEdit"));

        gridLayout_34->addWidget(rssSmtpLoginEdit, 4, 1, 1, 1);

        rssAuthTypeComboBox = new QComboBox(autosrtEmailNotificationCheckBox);
        rssAuthTypeComboBox->setObjectName(QString::fromUtf8("rssAuthTypeComboBox"));

        gridLayout_34->addWidget(rssAuthTypeComboBox, 3, 1, 1, 1);

        label_25 = new QLabel(autosrtEmailNotificationCheckBox);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        gridLayout_34->addWidget(label_25, 0, 0, 1, 1);

        rssSmtpPasswordEdit = new QLineEdit(autosrtEmailNotificationCheckBox);
        rssSmtpPasswordEdit->setObjectName(QString::fromUtf8("rssSmtpPasswordEdit"));
        rssSmtpPasswordEdit->setEchoMode(QLineEdit::Password);

        gridLayout_34->addWidget(rssSmtpPasswordEdit, 5, 1, 1, 1);

        label_35 = new QLabel(autosrtEmailNotificationCheckBox);
        label_35->setObjectName(QString::fromUtf8("label_35"));

        gridLayout_34->addWidget(label_35, 1, 0, 1, 1);

        rssSmtpServerEdit = new QLineEdit(autosrtEmailNotificationCheckBox);
        rssSmtpServerEdit->setObjectName(QString::fromUtf8("rssSmtpServerEdit"));

        gridLayout_34->addWidget(rssSmtpServerEdit, 0, 1, 1, 1);

        label_37 = new QLabel(autosrtEmailNotificationCheckBox);
        label_37->setObjectName(QString::fromUtf8("label_37"));

        gridLayout_34->addWidget(label_37, 4, 0, 1, 1);

        label_39 = new QLabel(autosrtEmailNotificationCheckBox);
        label_39->setObjectName(QString::fromUtf8("label_39"));

        gridLayout_34->addWidget(label_39, 6, 0, 1, 1);

        rssRecepientEmailEdit = new QLineEdit(autosrtEmailNotificationCheckBox);
        rssRecepientEmailEdit->setObjectName(QString::fromUtf8("rssRecepientEmailEdit"));

        gridLayout_34->addWidget(rssRecepientEmailEdit, 6, 1, 1, 1);

        label_40 = new QLabel(autosrtEmailNotificationCheckBox);
        label_40->setObjectName(QString::fromUtf8("label_40"));

        gridLayout_34->addWidget(label_40, 2, 0, 1, 1);

        rssSmtpConnTypeCombobox = new QComboBox(autosrtEmailNotificationCheckBox);
        rssSmtpConnTypeCombobox->setObjectName(QString::fromUtf8("rssSmtpConnTypeCombobox"));

        gridLayout_34->addWidget(rssSmtpConnTypeCombobox, 2, 1, 1, 1);


        gridLayout_33->addWidget(autosrtEmailNotificationCheckBox, 1, 5, 1, 3);

        label_24 = new QLabel(page_2);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        gridLayout_33->addWidget(label_24, 0, 5, 1, 1);

        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_33->addItem(verticalSpacer_7, 2, 5, 1, 1);

        addRssDLRuleButton = new QPushButton(page_2);
        addRssDLRuleButton->setObjectName(QString::fromUtf8("addRssDLRuleButton"));
        addRssDLRuleButton->setMinimumSize(QSize(30, 0));
        addRssDLRuleButton->setMaximumSize(QSize(30, 16777215));

        gridLayout_33->addWidget(addRssDLRuleButton, 3, 0, 1, 1);

        removeRssDLRuleButton = new QPushButton(page_2);
        removeRssDLRuleButton->setObjectName(QString::fromUtf8("removeRssDLRuleButton"));
        removeRssDLRuleButton->setMaximumSize(QSize(30, 16777215));

        gridLayout_33->addWidget(removeRssDLRuleButton, 3, 1, 1, 1);

        importRssRulesButton = new QPushButton(page_2);
        importRssRulesButton->setObjectName(QString::fromUtf8("importRssRulesButton"));

        gridLayout_33->addWidget(importRssRulesButton, 3, 3, 1, 1);

        rssRulesListWidget = new QListWidget(page_2);
        rssRulesListWidget->setObjectName(QString::fromUtf8("rssRulesListWidget"));
        rssRulesListWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
        rssRulesListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

        gridLayout_33->addWidget(rssRulesListWidget, 0, 0, 3, 5);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_33->addItem(horizontalSpacer, 3, 4, 1, 1);

        exportRssRulesButton = new QPushButton(page_2);
        exportRssRulesButton->setObjectName(QString::fromUtf8("exportRssRulesButton"));

        gridLayout_33->addWidget(exportRssRulesButton, 3, 2, 1, 1);

        rssRefrashRateEdit = new QTimeEdit(page_2);
        rssRefrashRateEdit->setObjectName(QString::fromUtf8("rssRefrashRateEdit"));
        rssRefrashRateEdit->setDisplayFormat(QString::fromUtf8("HH:mm:ss"));

        gridLayout_33->addWidget(rssRefrashRateEdit, 0, 6, 1, 2);

        stackedWidget->addWidget(page_2);
        keyMappingTab = new QWidget();
        keyMappingTab->setObjectName(QString::fromUtf8("keyMappingTab"));
        sizePolicy.setHeightForWidth(keyMappingTab->sizePolicy().hasHeightForWidth());
        keyMappingTab->setSizePolicy(sizePolicy);
        gridLayout_17 = new QGridLayout(keyMappingTab);
        gridLayout_17->setContentsMargins(0, 0, 0, 0);
        gridLayout_17->setObjectName(QString::fromUtf8("gridLayout_17"));
        hotKeyScrollArea = new QScrollArea(keyMappingTab);
        hotKeyScrollArea->setObjectName(QString::fromUtf8("hotKeyScrollArea"));
        hotKeyScrollArea->setStyleSheet(QString::fromUtf8(""));
        hotKeyScrollArea->setWidgetResizable(true);
        keyMapContainer = new QWidget();
        keyMapContainer->setObjectName(QString::fromUtf8("keyMapContainer"));
        keyMapContainer->setGeometry(QRect(0, 0, 98, 28));
        horizontalLayout_5 = new QHBoxLayout(keyMapContainer);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        hotKeyScrollArea->setWidget(keyMapContainer);

        gridLayout_17->addWidget(hotKeyScrollArea, 0, 0, 1, 1);

        stackedWidget->addWidget(keyMappingTab);

        gridLayout_4->addWidget(stackedWidget, 0, 1, 1, 1);


        verticalLayout->addWidget(m_centralWidget);

        QWidget::setTabOrder(localeComboBox, styleComboBox);
        QWidget::setTabOrder(styleComboBox, runOnbootCheckBox);
        QWidget::setTabOrder(runOnbootCheckBox, startMinimizedCheckBox);
        QWidget::setTabOrder(startMinimizedCheckBox, asociationCheckBox);
        QWidget::setTabOrder(asociationCheckBox, magnetAssociationCheckBox);
        QWidget::setTabOrder(magnetAssociationCheckBox, trackerGroupBox);
        QWidget::setTabOrder(trackerGroupBox, trackerPortEdit);
        QWidget::setTabOrder(trackerPortEdit, useNotificationsCheckBox);
        QWidget::setTabOrder(useNotificationsCheckBox, showTrackerErrorsCheckBox);
        QWidget::setTabOrder(showTrackerErrorsCheckBox, showDiskErrorsCheckBox);
        QWidget::setTabOrder(showDiskErrorsCheckBox, showRssErrorsCheckBox);
        QWidget::setTabOrder(showRssErrorsCheckBox, winShelItegrationCheckBox);
        QWidget::setTabOrder(winShelItegrationCheckBox, activeLimitEdit);
        QWidget::setTabOrder(activeLimitEdit, activeSeedLimitEdit);
        QWidget::setTabOrder(activeSeedLimitEdit, activeDownloadLimitEdit);
        QWidget::setTabOrder(activeDownloadLimitEdit, useDHTCheckBox);
        QWidget::setTabOrder(useDHTCheckBox, usePExCheckBox);
        QWidget::setTabOrder(usePExCheckBox, useLSDCheckBox);
        QWidget::setTabOrder(useLSDCheckBox, seedGlobalRatioEdit);
        QWidget::setTabOrder(seedGlobalRatioEdit, uploadLimitEdit);
        QWidget::setTabOrder(uploadLimitEdit, downloadLimitEdit);
        QWidget::setTabOrder(downloadLimitEdit, dhtLimitEdit);
        QWidget::setTabOrder(dhtLimitEdit, limitLocalConnectionCheckBox);
        QWidget::setTabOrder(limitLocalConnectionCheckBox, localUploadLimitEdit);
        QWidget::setTabOrder(localUploadLimitEdit, localDownloadLimitEdit);
        QWidget::setTabOrder(localDownloadLimitEdit, limitUtpCheckBox);
        QWidget::setTabOrder(limitUtpCheckBox, seedTimeLimitEdit);
        QWidget::setTabOrder(seedTimeLimitEdit, portEdit);
        QWidget::setTabOrder(portEdit, proxyGroupBox);
        QWidget::setTabOrder(proxyGroupBox, proxyHostEdit);
        QWidget::setTabOrder(proxyHostEdit, proxyUsernameEdit);
        QWidget::setTabOrder(proxyUsernameEdit, proxyPwdEdit);
        QWidget::setTabOrder(proxyPwdEdit, proxyTypeComboBox);
        QWidget::setTabOrder(proxyTypeComboBox, inEncPolicyComboBox);
        QWidget::setTabOrder(inEncPolicyComboBox, outEncPolicyComboBox);
        QWidget::setTabOrder(outEncPolicyComboBox, encLevelComboBox);
        QWidget::setTabOrder(encLevelComboBox, preferFullEncCheckBox);
        QWidget::setTabOrder(preferFullEncCheckBox, diskIOCasheModeComboBox);
        QWidget::setTabOrder(diskIOCasheModeComboBox, casheSizeLineEdit);
        QWidget::setTabOrder(casheSizeLineEdit, useDiskReadAheadCheckBox);
        QWidget::setTabOrder(useDiskReadAheadCheckBox, lowPrioDiskCheckBox);
        QWidget::setTabOrder(lowPrioDiskCheckBox, lockFilesCheckBox);
        QWidget::setTabOrder(lockFilesCheckBox, alowReorderedOpsCheckBox);
        QWidget::setTabOrder(alowReorderedOpsCheckBox, useReadCasheCheckBox);
        QWidget::setTabOrder(useReadCasheCheckBox, newGroupNameEdit);
        QWidget::setTabOrder(newGroupNameEdit, DTPathEdit);
        QWidget::setTabOrder(DTPathEdit, browseDTPathButton);
        QWidget::setTabOrder(browseDTPathButton, driveNumberComboBox);
        QWidget::setTabOrder(driveNumberComboBox, customMoutGroupBox);
        QWidget::setTabOrder(customMoutGroupBox, customCommandEdit);
        QWidget::setTabOrder(customCommandEdit, tasksComboBox);
        QWidget::setTabOrder(tasksComboBox, addTaskButton);
        QWidget::setTabOrder(addTaskButton, deleteTaskButton);
        QWidget::setTabOrder(deleteTaskButton, taskNameLineEdit);
        QWidget::setTabOrder(taskNameLineEdit, beginDateTimeEdit);
        QWidget::setTabOrder(beginDateTimeEdit, pauseAllRadioButton);
        QWidget::setTabOrder(pauseAllRadioButton, limitDlRadioButton);
        QWidget::setTabOrder(limitDlRadioButton, startAllRadioButton);
        QWidget::setTabOrder(startAllRadioButton, limitUlRadioButton);
        QWidget::setTabOrder(limitUlRadioButton, dlLimitEdit);
        QWidget::setTabOrder(dlLimitEdit, ulLimitEdit);
        QWidget::setTabOrder(ulLimitEdit, calendarWidget);
        QWidget::setTabOrder(calendarWidget, webUIGroupBox);
        QWidget::setTabOrder(webUIGroupBox, loginLineEdit);
        QWidget::setTabOrder(loginLineEdit, passwordLineEdit);
        QWidget::setTabOrder(passwordLineEdit, webPortLineEdit);
        QWidget::setTabOrder(webPortLineEdit, openWebUiButton);
        QWidget::setTabOrder(openWebUiButton, upnpCheckBox);
        QWidget::setTabOrder(upnpCheckBox, startRconButton);
        QWidget::setTabOrder(startRconButton, stopRconButton);
        QWidget::setTabOrder(stopRconButton, webUILogginGroupBox);
        QWidget::setTabOrder(webUILogginGroupBox, logLineEdit);
        QWidget::setTabOrder(logLineEdit, IPFilterGroupBox);
        QWidget::setTabOrder(IPFilterGroupBox, ipFilterTextEdit);
        QWidget::setTabOrder(ipFilterTextEdit, rssRulesListWidget);
        QWidget::setTabOrder(rssRulesListWidget, addRssDLRuleButton);
        QWidget::setTabOrder(addRssDLRuleButton, removeRssDLRuleButton);
        QWidget::setTabOrder(removeRssDLRuleButton, exportRssRulesButton);
        QWidget::setTabOrder(exportRssRulesButton, importRssRulesButton);
        QWidget::setTabOrder(importRssRulesButton, autosrtEmailNotificationCheckBox);
        QWidget::setTabOrder(autosrtEmailNotificationCheckBox, rssSmtpServerEdit);
        QWidget::setTabOrder(rssSmtpServerEdit, rssSmtpPortEdit);
        QWidget::setTabOrder(rssSmtpPortEdit, rssSmtpConnTypeCombobox);
        QWidget::setTabOrder(rssSmtpConnTypeCombobox, rssAuthTypeComboBox);
        QWidget::setTabOrder(rssAuthTypeComboBox, rssSmtpLoginEdit);
        QWidget::setTabOrder(rssSmtpLoginEdit, rssSmtpPasswordEdit);
        QWidget::setTabOrder(rssSmtpPasswordEdit, rssRecepientEmailEdit);
        QWidget::setTabOrder(rssRecepientEmailEdit, buttonBox);
        QWidget::setTabOrder(buttonBox, pbClose);
        QWidget::setTabOrder(pbClose, listWidget);
        QWidget::setTabOrder(listWidget, hotKeyScrollArea);

        retranslateUi(SettingsDialog);
        QObject::connect(addTaskButton, SIGNAL(clicked()), SettingsDialog, SLOT(AddTask()));
        QObject::connect(BrowseGroupSavePathButton, SIGNAL(clicked()), SettingsDialog, SLOT(browseSavepathGroup()));
        QObject::connect(deleteTaskButton, SIGNAL(clicked()), SettingsDialog, SLOT(DeleteTask()));
        QObject::connect(listWidget, SIGNAL(currentRowChanged(int)), stackedWidget, SLOT(setCurrentIndex(int)));
        QObject::connect(startRconButton, SIGNAL(clicked()), SettingsDialog, SLOT(StartRcon()));
        QObject::connect(stopRconButton, SIGNAL(clicked()), SettingsDialog, SLOT(StopRcon()));
        QObject::connect(limitUlRadioButton, SIGNAL(clicked()), ulLimitEdit, SLOT(setFocus()));
        QObject::connect(removeGroupButton, SIGNAL(clicked()), SettingsDialog, SLOT(removeGroup()));
        QObject::connect(addNewGroupButton, SIGNAL(clicked()), SettingsDialog, SLOT(addGroup()));
        QObject::connect(browseDTPathButton, SIGNAL(clicked()), SettingsDialog, SLOT(browseDTPath()));
        QObject::connect(limitDlRadioButton, SIGNAL(clicked()), dlLimitEdit, SLOT(setFocus()));
        QObject::connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), SettingsDialog, SLOT(chooseAction(QAbstractButton*)));
        QObject::connect(openWebUiButton, SIGNAL(clicked()), SettingsDialog, SLOT(OpenWebUI()));
        QObject::connect(addRssDLRuleButton, SIGNAL(clicked()), SettingsDialog, SLOT(addRssRule()));
        QObject::connect(rssRulesListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), SettingsDialog, SLOT(onEditRssRule()));
        QObject::connect(removeRssDLRuleButton, SIGNAL(clicked()), SettingsDialog, SLOT(onDeleteRssRule()));
        QObject::connect(exportRssRulesButton, SIGNAL(clicked()), SettingsDialog, SLOT(onExportRssRules()));
        QObject::connect(importRssRulesButton, SIGNAL(clicked()), SettingsDialog, SLOT(onImportRssRules()));
        QObject::connect(browseWatchDirButton, SIGNAL(clicked()), SettingsDialog, SLOT(onBrowseWatchDir()));
        QObject::connect(browseWatchStaticPathButton, SIGNAL(clicked()), SettingsDialog, SLOT(onBrowseWatchStaticPath()));
        QObject::connect(GroupsTreeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), SettingsDialog, SLOT(showSelectedGroup(QTreeWidgetItem*,QTreeWidgetItem*)));
        QObject::connect(updateGroupButton, SIGNAL(clicked()), SettingsDialog, SLOT(editGroup()));
        QObject::connect(useRandomPortCheckBox, SIGNAL(toggled(bool)), portEdit, SLOT(setDisabled(bool)));

        listWidget->setCurrentRow(0);
        stackedWidget->setCurrentIndex(0);
        inEncPolicyComboBox->setCurrentIndex(0);
        outEncPolicyComboBox->setCurrentIndex(0);
        encLevelComboBox->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
        SettingsDialog->setWindowTitle(QApplication::translate("SettingsDialog", "SETTINGS_DIALOG", 0));
        tbMenu->setText(QString());
        LTitle->setText(QApplication::translate("SettingsDialog", "SETTINGS_DIALOG", 0));

        const bool __sortingEnabled = listWidget->isSortingEnabled();
        listWidget->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = listWidget->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("SettingsDialog", "TAB_GENERAL", 0));
        QListWidgetItem *___qlistwidgetitem1 = listWidget->item(1);
        ___qlistwidgetitem1->setText(QApplication::translate("SettingsDialog", "TAB_RESTRICTIONS", 0));
        QListWidgetItem *___qlistwidgetitem2 = listWidget->item(2);
        ___qlistwidgetitem2->setText(QApplication::translate("SettingsDialog", "TAB_NETWORK", 0));
        QListWidgetItem *___qlistwidgetitem3 = listWidget->item(3);
        ___qlistwidgetitem3->setText(QApplication::translate("SettingsDialog", "TAB_HDD", 0));
        QListWidgetItem *___qlistwidgetitem4 = listWidget->item(4);
        ___qlistwidgetitem4->setText(QApplication::translate("SettingsDialog", "TAB_TORRENT_FILTERING", 0));
        QListWidgetItem *___qlistwidgetitem5 = listWidget->item(5);
        ___qlistwidgetitem5->setText(QApplication::translate("SettingsDialog", "TAB_DAEMONTOOLS", 0));
        QListWidgetItem *___qlistwidgetitem6 = listWidget->item(6);
        ___qlistwidgetitem6->setText(QApplication::translate("SettingsDialog", "TAB_SCEDULER", 0));
        QListWidgetItem *___qlistwidgetitem7 = listWidget->item(7);
        ___qlistwidgetitem7->setText(QApplication::translate("SettingsDialog", "TAB_WEB_CONTROL", 0));
        QListWidgetItem *___qlistwidgetitem8 = listWidget->item(8);
        ___qlistwidgetitem8->setText(QApplication::translate("SettingsDialog", "TAB_RSS", 0));
        QListWidgetItem *___qlistwidgetitem9 = listWidget->item(9);
        ___qlistwidgetitem9->setText(QApplication::translate("SettingsDialog", "TAB_KEY_MAPPINGS", 0));
        listWidget->setSortingEnabled(__sortingEnabled);

        useNotificationsCheckBox->setTitle(QApplication::translate("SettingsDialog", "ENABLE_NOTIFICATIONS", 0));
        showTrackerErrorsCheckBox->setText(QApplication::translate("SettingsDialog", "NOTIFY_TRACKER_ERRORS", 0));
        showDiskErrorsCheckBox->setText(QApplication::translate("SettingsDialog", "NOTIFY_IO_ERRORS", 0));
        showRssErrorsCheckBox->setText(QApplication::translate("SettingsDialog", "NOTIFY_RSS_ERRORS", 0));
        trackerGroupBox->setTitle(QApplication::translate("SettingsDialog", "START_TRACKER", 0));
        label_23->setText(QApplication::translate("SettingsDialog", "TRACKER_PORT", 0));
#ifndef QT_NO_WHATSTHIS
        asociationCheckBox->setWhatsThis(QApplication::translate("SettingsDialog", "TORRENT_FILES_ASSOCIATION_WIT", 0));
#endif // QT_NO_WHATSTHIS
        asociationCheckBox->setText(QApplication::translate("SettingsDialog", "TORRENT_FILES_ASSOCIATION", 0));
        winShelItegrationCheckBox->setText(QApplication::translate("SettingsDialog", "ENABLE_SHELL_INTEGRATION", 0));
        label_26->setText(QApplication::translate("SettingsDialog", "STYLE", 0));
#ifndef QT_NO_WHATSTHIS
        startMinimizedCheckBox->setWhatsThis(QApplication::translate("SettingsDialog", "START_MINIMIZED_WIT", 0));
#endif // QT_NO_WHATSTHIS
        startMinimizedCheckBox->setText(QApplication::translate("SettingsDialog", "START_MINIMIZED", 0));
        scriptDebugingCheckBox->setText(QApplication::translate("SettingsDialog", "ENABLE_SEARCH_SCRIPT_DEBUGING", 0));
#ifndef QT_NO_WHATSTHIS
        magnetAssociationCheckBox->setWhatsThis(QApplication::translate("SettingsDialog", "MAGNET_ASSOCIATION_WIT", 0));
#endif // QT_NO_WHATSTHIS
        magnetAssociationCheckBox->setText(QApplication::translate("SettingsDialog", "MAGNET_ASSOCIATION", 0));
#ifndef QT_NO_WHATSTHIS
        runOnbootCheckBox->setWhatsThis(QApplication::translate("SettingsDialog", "START_ON_BOOT_WIT", 0));
#endif // QT_NO_WHATSTHIS
        runOnbootCheckBox->setText(QApplication::translate("SettingsDialog", "START_ON_BOOT", 0));
        label_8->setText(QApplication::translate("SettingsDialog", "LANGUAGE", 0));
        closeOnHideCheckBox->setText(QApplication::translate("SettingsDialog", "HIDE_ON_CLOSE", 0));
        watchDirEnabledCheckbox->setTitle(QApplication::translate("SettingsDialog", "WATCH_DIR_FOR_NEW_TORRENTS", 0));
        useStaticPathRadioButton->setText(QApplication::translate("SettingsDialog", "AUTO_ADD_USING_STATIC_PATH", 0));
        showDialogRadioButton->setText(QApplication::translate("SettingsDialog", "NOT_AUTO_ADD", 0));
        usetTorrentFilteringRadioButton->setText(QApplication::translate("SettingsDialog", "AUTO_ADD_USING_TOR_FILTERING", 0));
        browseWatchStaticPathButton->setText(QApplication::translate("SettingsDialog", "BROWSE", 0));
        label_43->setText(QApplication::translate("SettingsDialog", "DIR_TO_WATCH", 0));
        browseWatchDirButton->setText(QApplication::translate("SettingsDialog", "BROWSE", 0));
        checkBox->setText(QApplication::translate("SettingsDialog", "DELETE_TORRENTS", 0));
        groupBox_12->setTitle(QApplication::translate("SettingsDialog", "ADDITINAL_PEER_SOUECES", 0));
        usePExCheckBox->setText(QApplication::translate("SettingsDialog", "USE_PEX", 0));
        useLSDCheckBox->setText(QApplication::translate("SettingsDialog", "USE_LSD", 0));
        useDHTCheckBox->setText(QApplication::translate("SettingsDialog", "USE_DHT", 0));
        groupBox_2->setTitle(QApplication::translate("SettingsDialog", "SPEED_RESRICTIONS", 0));
#ifndef QT_NO_WHATSTHIS
        label_6->setWhatsThis(QApplication::translate("SettingsDialog", "UPLOAD_SPEED_LIMIT_WIT", 0));
#endif // QT_NO_WHATSTHIS
        label_6->setText(QApplication::translate("SettingsDialog", "UPLOAD_SPEED_LIMIT", 0));
        uploadLimitEdit->setSpecialValueText(QApplication::translate("SettingsDialog", "None", 0));
        uploadLimitEdit->setSuffix(QApplication::translate("SettingsDialog", " Kb\\s", 0));
        uploadLimitEdit->setPrefix(QString());
#ifndef QT_NO_WHATSTHIS
        label_7->setWhatsThis(QApplication::translate("SettingsDialog", "DOWNLOAD_SPEED_LIMIT_WIT", 0));
#endif // QT_NO_WHATSTHIS
        label_7->setText(QApplication::translate("SettingsDialog", "DOWNLOAD_SPEED_LIMIT", 0));
        downloadLimitEdit->setSpecialValueText(QApplication::translate("SettingsDialog", "None", 0));
        downloadLimitEdit->setSuffix(QApplication::translate("SettingsDialog", " Kb\\s", 0));
        downloadLimitEdit->setPrefix(QString());
        label_33->setText(QApplication::translate("SettingsDialog", "DHT_RATE_LIMIT", 0));
        dhtLimitEdit->setSpecialValueText(QApplication::translate("SettingsDialog", "None", 0));
        dhtLimitEdit->setSuffix(QApplication::translate("SettingsDialog", " Kb\\s", 0));
        dhtLimitEdit->setPrefix(QString());
        limitLocalConnectionCheckBox->setTitle(QApplication::translate("SettingsDialog", "USE_LIMITS_IN_LOCAL_NETWORK", 0));
        label_31->setText(QApplication::translate("SettingsDialog", "LOCAL_UPLOAD_LIMIT", 0));
        localUploadLimitEdit->setSpecialValueText(QApplication::translate("SettingsDialog", "None", 0));
        localUploadLimitEdit->setSuffix(QApplication::translate("SettingsDialog", "Kb\\s", 0));
        label_32->setText(QApplication::translate("SettingsDialog", "LOCAL_DOWNLOAD_LIMIT", 0));
        localDownloadLimitEdit->setSpecialValueText(QApplication::translate("SettingsDialog", "None", 0));
        localDownloadLimitEdit->setSuffix(QApplication::translate("SettingsDialog", "Kb\\s", 0));
        limitUtpCheckBox->setText(QApplication::translate("SettingsDialog", "APPLY_LIMITS_TO_uTP", 0));
        label_30->setText(QApplication::translate("SettingsDialog", "SEED_TIME_LIMIT", 0));
        label_34->setText(QApplication::translate("SettingsDialog", "SEED_GLOBAL_RATIO_LIMIT", 0));
        label_41->setText(QApplication::translate("SettingsDialog", "MAX_CONNECTIONS_PER_TORRENT", 0));
        restrictionsGroupBox->setTitle(QApplication::translate("SettingsDialog", "ACTIVITY_RESTRICTIONS", 0));
#ifndef QT_NO_WHATSTHIS
        activeSeedLimitEdit->setWhatsThis(QApplication::translate("SettingsDialog", "ACTIVE_UPLOADS_LIMIT", 0));
#endif // QT_NO_WHATSTHIS
        activeSeedLimitEdit->setSpecialValueText(QApplication::translate("SettingsDialog", "None", 0));
#ifndef QT_NO_WHATSTHIS
        activeLimitEdit->setWhatsThis(QApplication::translate("SettingsDialog", "ACTIVE_TORRENTS_LIMIT", 0));
#endif // QT_NO_WHATSTHIS
        activeLimitEdit->setSpecialValueText(QApplication::translate("SettingsDialog", "None", 0));
#ifndef QT_NO_WHATSTHIS
        activeDownloadLimitEdit->setWhatsThis(QApplication::translate("SettingsDialog", "ACTIVE_DOWNLOADS_LIMIT", 0));
#endif // QT_NO_WHATSTHIS
        activeDownloadLimitEdit->setSpecialValueText(QApplication::translate("SettingsDialog", "None", 0));
#ifndef QT_NO_WHATSTHIS
        label_17->setWhatsThis(QApplication::translate("SettingsDialog", "ACTIVE_DOWNLOADS_LIMIT_WIT", 0));
#endif // QT_NO_WHATSTHIS
        label_17->setText(QApplication::translate("SettingsDialog", "ACTIVE_DOWNLOADS_LIMIT", 0));
#ifndef QT_NO_WHATSTHIS
        label_16->setWhatsThis(QApplication::translate("SettingsDialog", "ACTIVE_SEEDS_LIMIT_WIT", 0));
#endif // QT_NO_WHATSTHIS
        label_16->setText(QApplication::translate("SettingsDialog", "ACTIVE_SEEDS_LIMIT", 0));
#ifndef QT_NO_WHATSTHIS
        label_15->setWhatsThis(QApplication::translate("SettingsDialog", "ACTIVE_TORRENTS_LIMIT_WIT", 0));
#endif // QT_NO_WHATSTHIS
        label_15->setText(QApplication::translate("SettingsDialog", "ACTIVE_TORRENTS_LIMIT", 0));
        label_45->setText(QApplication::translate("SettingsDialog", "MAX_CONNECTIONS", 0));
        label->setText(QApplication::translate("SettingsDialog", "LISTEN_PORT", 0));
        groupBox_10->setTitle(QApplication::translate("SettingsDialog", "NET_ENC", 0));
        label_27->setText(QApplication::translate("SettingsDialog", "IN_ENC_POLICITY", 0));
        inEncPolicyComboBox->clear();
        inEncPolicyComboBox->insertItems(0, QStringList()
         << QApplication::translate("SettingsDialog", "FORCED", 0)
         << QApplication::translate("SettingsDialog", "ENABLED", 0)
         << QApplication::translate("SettingsDialog", "DISABLED", 0)
        );
        label_28->setText(QApplication::translate("SettingsDialog", "OUT_ENC_POLICITY", 0));
        outEncPolicyComboBox->clear();
        outEncPolicyComboBox->insertItems(0, QStringList()
         << QApplication::translate("SettingsDialog", "FORCED", 0)
         << QApplication::translate("SettingsDialog", "ENABLED", 0)
         << QApplication::translate("SettingsDialog", "DISABLED", 0)
        );
        label_29->setText(QApplication::translate("SettingsDialog", "ENCRYPTION_LEVEL", 0));
        encLevelComboBox->clear();
        encLevelComboBox->insertItems(0, QStringList()
         << QApplication::translate("SettingsDialog", "PROTOCOL_ENC_ONLY", 0)
         << QApplication::translate("SettingsDialog", "FULL_ENC", 0)
         << QApplication::translate("SettingsDialog", "BOTH", 0)
        );
        preferFullEncCheckBox->setText(QApplication::translate("SettingsDialog", "PREFER_FULL_ENCRYPTION", 0));
        proxyGroupBox->setTitle(QApplication::translate("SettingsDialog", "USE_PROXY", 0));
#ifndef QT_NO_WHATSTHIS
        label_3->setWhatsThis(QApplication::translate("SettingsDialog", "PROXY_USER_NAME_WIT", 0));
#endif // QT_NO_WHATSTHIS
        label_3->setText(QApplication::translate("SettingsDialog", "PROXY_USER_NAME", 0));
#ifndef QT_NO_WHATSTHIS
        proxyHostEdit->setWhatsThis(QApplication::translate("SettingsDialog", "PROXY_HOST_WIT", 0));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_WHATSTHIS
        proxyUsernameEdit->setWhatsThis(QApplication::translate("SettingsDialog", "PROXY_USER_NAME_WIT", 0));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_WHATSTHIS
        label_5->setWhatsThis(QApplication::translate("SettingsDialog", "PROXY_TYPE_WIT", 0));
#endif // QT_NO_WHATSTHIS
        label_5->setText(QApplication::translate("SettingsDialog", "PROXY_TYPE", 0));
#ifndef QT_NO_WHATSTHIS
        label_4->setWhatsThis(QApplication::translate("SettingsDialog", "PROXY_PASSWORD_WIT", 0));
#endif // QT_NO_WHATSTHIS
        label_4->setText(QApplication::translate("SettingsDialog", "PROXY_PASSWORD", 0));
#ifndef QT_NO_WHATSTHIS
        label_2->setWhatsThis(QApplication::translate("SettingsDialog", "PROXY_HOST_WIT", 0));
#endif // QT_NO_WHATSTHIS
        label_2->setText(QApplication::translate("SettingsDialog", "PROXY_HOST", 0));
#ifndef QT_NO_WHATSTHIS
        proxyPwdEdit->setWhatsThis(QApplication::translate("SettingsDialog", "PROXY_PASSWORD", 0));
#endif // QT_NO_WHATSTHIS
        proxyTypeComboBox->clear();
        proxyTypeComboBox->insertItems(0, QStringList()
         << QApplication::translate("SettingsDialog", "Socks4", 0)
         << QApplication::translate("SettingsDialog", "Socks5", 0)
         << QApplication::translate("SettingsDialog", "Socks5 \321\201 \320\277\320\260\321\200\320\276\320\273\320\265\320\274", 0)
         << QApplication::translate("SettingsDialog", "http", 0)
         << QApplication::translate("SettingsDialog", "http \321\201 \320\277\320\260\321\200\320\276\320\273\320\265\320\274", 0)
         << QApplication::translate("SettingsDialog", "i2p", 0)
        );
#ifndef QT_NO_WHATSTHIS
        proxyTypeComboBox->setWhatsThis(QApplication::translate("SettingsDialog", "PROXY_TYPE", 0));
#endif // QT_NO_WHATSTHIS
        label_44->setText(QApplication::translate("SettingsDialog", "PROXY_PORT", 0));
        useRandomPortCheckBox->setText(QApplication::translate("SettingsDialog", "USE_RANDOM_PORT", 0));
        portMappingsCheckBox->setText(QApplication::translate("SettingsDialog", "USE_PORT_MAPPINGS", 0));
#ifndef QT_NO_WHATSTHIS
        useDiskReadAheadCheckBox->setWhatsThis(QApplication::translate("SettingsDialog", "USE_DISK_READ_AHEAD_WIT", 0));
#endif // QT_NO_WHATSTHIS
        useDiskReadAheadCheckBox->setText(QApplication::translate("SettingsDialog", "USE_DISK_READ_AHEAD", 0));
#ifndef QT_NO_WHATSTHIS
        alowReorderedOpsCheckBox->setWhatsThis(QApplication::translate("SettingsDialog", "ALOW_REORDERED_OPERATIONS_WIT", 0));
#endif // QT_NO_WHATSTHIS
        alowReorderedOpsCheckBox->setText(QApplication::translate("SettingsDialog", "ALOW_REORDERED_OPERATIONS", 0));
        label_42->setText(QApplication::translate("SettingsDialog", "DISK_ALLOCATION_MODE", 0));
        casheSizeLineEdit->setSpecialValueText(QApplication::translate("SettingsDialog", "None", 0));
        casheSizeLineEdit->setSuffix(QApplication::translate("SettingsDialog", " Kb", 0));
        casheSizeLineEdit->setPrefix(QString());
        diskAllocationComboBox->clear();
        diskAllocationComboBox->insertItems(0, QStringList()
         << QApplication::translate("SettingsDialog", "FULL_ALLOCATION", 0)
         << QApplication::translate("SettingsDialog", "PIECE_ALLOCATION", 0)
        );
#ifndef QT_NO_WHATSTHIS
        lockFilesCheckBox->setWhatsThis(QApplication::translate("SettingsDialog", "LOCK_FILES_WIT", 0));
#endif // QT_NO_WHATSTHIS
        lockFilesCheckBox->setText(QApplication::translate("SettingsDialog", "LOCK_FILES", 0));
#ifndef QT_NO_WHATSTHIS
        useReadCasheCheckBox->setWhatsThis(QApplication::translate("SettingsDialog", "USE_READ_CASHE_WIT", 0));
#endif // QT_NO_WHATSTHIS
        useReadCasheCheckBox->setText(QApplication::translate("SettingsDialog", "USE_READ_CASHE", 0));
#ifndef QT_NO_WHATSTHIS
        lowPrioDiskCheckBox->setWhatsThis(QApplication::translate("SettingsDialog", "LOW_PORIO_DISK_WIT", 0));
#endif // QT_NO_WHATSTHIS
        lowPrioDiskCheckBox->setText(QApplication::translate("SettingsDialog", "LOW_PORIO_DISK", 0));
#ifndef QT_NO_WHATSTHIS
        label_19->setWhatsThis(QApplication::translate("SettingsDialog", "DISK_IO_CASH_MODE_WIT", 0));
#endif // QT_NO_WHATSTHIS
        label_19->setText(QApplication::translate("SettingsDialog", "DISK_IO_CASH_MODE", 0));
        diskIOCasheModeComboBox->clear();
        diskIOCasheModeComboBox->insertItems(0, QStringList()
         << QApplication::translate("SettingsDialog", "ENABLE_OS_CACHE", 0)
         << QApplication::translate("SettingsDialog", "DISABLE_OS_CACHE_FOR_ALIGNED_FILES", 0)
         << QApplication::translate("SettingsDialog", "DISABLE_OS_CACHE", 0)
        );
#ifndef QT_NO_WHATSTHIS
        label_20->setWhatsThis(QApplication::translate("SettingsDialog", "CASH_SIZE_WIT", 0));
#endif // QT_NO_WHATSTHIS
        label_20->setText(QApplication::translate("SettingsDialog", "CASH_SIZE", 0));
        groupBox_9->setTitle(QApplication::translate("SettingsDialog", "ADD_CHANGE_GROUP", 0));
#ifndef QT_NO_WHATSTHIS
        groupSavePathEdit->setWhatsThis(QApplication::translate("SettingsDialog", "groupSavePathEdit_WIT", 0));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_WHATSTHIS
        BrowseGroupSavePathButton->setWhatsThis(QApplication::translate("SettingsDialog", "GROUP_SAVE_PATH_CHOISE_BUTTON_WIT", 0));
#endif // QT_NO_WHATSTHIS
        BrowseGroupSavePathButton->setText(QApplication::translate("SettingsDialog", "GROUP_SAVE_PATH_CHOISE_BUTTON", 0));
        label_47->setText(QApplication::translate("SettingsDialog", "GROUP_EXTENTIONS", 0));
        label_48->setText(QApplication::translate("SettingsDialog", "GROUP_SAVE_PATH", 0));
#ifndef QT_NO_WHATSTHIS
        extensionsEdit->setWhatsThis(QApplication::translate("SettingsDialog", "extensionsEdit_WIT", 0));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_WHATSTHIS
        newGroupNameEdit->setWhatsThis(QApplication::translate("SettingsDialog", "newGroupNameEdit_WIT", 0));
#endif // QT_NO_WHATSTHIS
        label_46->setText(QApplication::translate("SettingsDialog", "GROUP_NAME", 0));
#ifndef QT_NO_WHATSTHIS
        addNewGroupButton->setWhatsThis(QApplication::translate("SettingsDialog", "ADD_GROUP_BUTON_WIT", 0));
#endif // QT_NO_WHATSTHIS
        addNewGroupButton->setText(QApplication::translate("SettingsDialog", "ADD_GROUP_BUTON", 0));
        updateGroupButton->setText(QApplication::translate("SettingsDialog", "UPDATE_GROUP", 0));
#ifndef QT_NO_WHATSTHIS
        removeGroupButton->setWhatsThis(QApplication::translate("SettingsDialog", "FILTERING_GROUP_DELETE_WIT", 0));
#endif // QT_NO_WHATSTHIS
        removeGroupButton->setText(QApplication::translate("SettingsDialog", "FILTERING_GROUP_DELETE", 0));
#ifndef QT_NO_WHATSTHIS
        GroupsTreeWidget->setWhatsThis(QApplication::translate("SettingsDialog", "GROUP_LISTBOX_WIT", 0));
#endif // QT_NO_WHATSTHIS
        groupBox_6->setTitle(QApplication::translate("SettingsDialog", "DAEMONTOOLS_PATH", 0));
#ifndef QT_NO_WHATSTHIS
        DTPathEdit->setWhatsThis(QApplication::translate("SettingsDialog", "DTPathEdit_WIT", 0));
#endif // QT_NO_WHATSTHIS
        browseDTPathButton->setText(QApplication::translate("SettingsDialog", "DAEMONTOOLS_PATH_CHOISE_BUUTON", 0));
        groupBox_8->setTitle(QApplication::translate("SettingsDialog", "DRIVE_NUMBER", 0));
#ifndef QT_NO_WHATSTHIS
        driveNumberComboBox->setWhatsThis(QApplication::translate("SettingsDialog", "driveNumberComboBox_WIT", 0));
#endif // QT_NO_WHATSTHIS
        customMoutGroupBox->setTitle(QApplication::translate("SettingsDialog", "DAEMONTOOLS_CUSTOM_COMMAND", 0));
#ifndef QT_NO_WHATSTHIS
        customCommandEdit->setWhatsThis(QApplication::translate("SettingsDialog", "DAEMONTOOLS_COMMAND_WIT", 0));
#endif // QT_NO_WHATSTHIS
        groupBox_11->setTitle(QApplication::translate("SettingsDialog", "GroupBox", 0));
        groupBox->setTitle(QApplication::translate("SettingsDialog", "ACTIONS", 0));
        label_11->setText(QApplication::translate("SettingsDialog", "LIMIT_DOWNLOAD", 0));
        label_12->setText(QApplication::translate("SettingsDialog", "LIMIT_UPLOAD", 0));
        startAllRadioButton->setText(QApplication::translate("SettingsDialog", "START_ALL", 0));
        limitUlRadioButton->setText(QApplication::translate("SettingsDialog", "LIMIT_UPLOAD", 0));
        limitDlRadioButton->setText(QApplication::translate("SettingsDialog", "LIMIT_DOWNLOAD", 0));
        pauseAllRadioButton->setText(QApplication::translate("SettingsDialog", "PUASE_ALL", 0));
        label_9->setText(QApplication::translate("SettingsDialog", "ACTION_BEGIN", 0));
        label_13->setText(QApplication::translate("SettingsDialog", "ACTION_NAME", 0));
        label_14->setText(QApplication::translate("SettingsDialog", "ACTIONS", 0));
        addTaskButton->setText(QApplication::translate("SettingsDialog", "ADD_BUTTON", 0));
        deleteTaskButton->setText(QApplication::translate("SettingsDialog", "DELETE_BUTTON", 0));
        webUIGroupBox->setTitle(QApplication::translate("SettingsDialog", "WEB_CONTROLL_ENABLED", 0));
        IPFilterGroupBox->setTitle(QApplication::translate("SettingsDialog", "WEB_UI_USE_IP_FILTER", 0));
        webUILogginGroupBox->setTitle(QApplication::translate("SettingsDialog", "WEB_UI_LOGGING_ENABLED", 0));
        label_22->setText(QApplication::translate("SettingsDialog", "WEB_UI_LOG_NAME", 0));
        label_10->setText(QApplication::translate("SettingsDialog", "WEB_UI_LOGIN", 0));
        label_18->setText(QApplication::translate("SettingsDialog", "WEB_UI_PASSWORD", 0));
        upnpCheckBox->setText(QApplication::translate("SettingsDialog", "WEB_UI_UPNP_ENABLED", 0));
        label_21->setText(QApplication::translate("SettingsDialog", "WEB_UI_PORT", 0));
        startRconButton->setText(QApplication::translate("SettingsDialog", "WEBUI_START", 0));
        stopRconButton->setText(QApplication::translate("SettingsDialog", "WEBUI_STOP", 0));
        RunningLabel->setText(QString());
        openWebUiButton->setText(QApplication::translate("SettingsDialog", "OPEN_WEBUI", 0));
        autosrtEmailNotificationCheckBox->setTitle(QApplication::translate("SettingsDialog", "RSS_AUTO_START_EMAIL_NOTIFICATION", 0));
        label_36->setText(QApplication::translate("SettingsDialog", "RSS_SMTP_AUTH_TYPE", 0));
        label_38->setText(QApplication::translate("SettingsDialog", "RSS_SMTP_PASSWORD", 0));
        label_25->setText(QApplication::translate("SettingsDialog", "RSS_SMTP_SERVER", 0));
        label_35->setText(QApplication::translate("SettingsDialog", "RSS_SMTP_PORT", 0));
        label_37->setText(QApplication::translate("SettingsDialog", "RSS_SMTP_LOGIN", 0));
        label_39->setText(QApplication::translate("SettingsDialog", "RSS_NOTIFICATION_EMEAIL", 0));
        label_40->setText(QApplication::translate("SettingsDialog", "RSS_SMTP_CONN_TYPE", 0));
        label_24->setText(QApplication::translate("SettingsDialog", "RSS_DEFAULT_REFRESH_RATE", 0));
        addRssDLRuleButton->setText(QApplication::translate("SettingsDialog", "+", 0));
        removeRssDLRuleButton->setText(QApplication::translate("SettingsDialog", "-", 0));
        importRssRulesButton->setText(QApplication::translate("SettingsDialog", "RSS_RULES_IMPORT", 0));
        exportRssRulesButton->setText(QApplication::translate("SettingsDialog", "RSS_RULES_EXPORT", 0));
    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSDIALOG_H
