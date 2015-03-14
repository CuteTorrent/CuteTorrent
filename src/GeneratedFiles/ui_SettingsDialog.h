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
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCalendarWidget>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDateTimeEdit>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QScrollArea>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStackedWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

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
    QDialogButtonBox *buttonBox;
    QStackedWidget *stackedWidget;
    QWidget *torrentTab;
    QGridLayout *gridLayout_22;
    QComboBox *styleComboBox;
    QCheckBox *runOnbootCheckBox;
    QLabel *label_26;
    QCheckBox *magnetAssociationCheckBox;
    QCheckBox *startMinimizedCheckBox;
    QCheckBox *asociationCheckBox;
    QLabel *label_8;
    QComboBox *localeComboBox;
    QGroupBox *groupBox_12;
    QGridLayout *gridLayout_29;
    QGroupBox *useDHTCheckBox;
    QGridLayout *gridLayout_30;
    QGroupBox *useDifDhtPorrtGroupBox;
    QGridLayout *gridLayout_31;
    QLineEdit *specDhtPortEdit;
    QLabel *label_30;
    QCheckBox *usePExCheckBox;
    QCheckBox *useLSDCheckBox;
    QGroupBox *trackerGroupBox;
    QGridLayout *gridLayout_2;
    QLabel *label_23;
    QLineEdit *trackerPortEdit;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_3;
    QLabel *label_6;
    QLabel *label_15;
    QLineEdit *activeLimitEdit;
    QLabel *label_7;
    QLabel *label_16;
    QLineEdit *activeSeedLimitEdit;
    QLabel *label_17;
    QLineEdit *activeDownloadLimitEdit;
    QSpinBox *uploadLimitEdit;
    QSpinBox *downloadLimitEdit;
    QWidget *page;
    QGridLayout *gridLayout_20;
    QLineEdit *portEdit;
    QLabel *label;
    QGroupBox *proxyGroupBox;
    QGridLayout *gridLayout;
    QLabel *label_5;
    QLineEdit *proxyUsernameEdit;
    QLineEdit *proxyHostEdit;
    QLabel *label_3;
    QLabel *label_2;
    QLineEdit *proxyPwdEdit;
    QComboBox *proxyTypeComboBox;
    QLabel *label_4;
    QGroupBox *groupBox_10;
    QGridLayout *gridLayout_28;
    QLabel *label_27;
    QComboBox *inEncPolicyComboBox;
    QLabel *label_28;
    QLabel *label_29;
    QComboBox *encLevelComboBox;
    QCheckBox *preferFullEncCheckBox;
    QComboBox *outEncPolicyComboBox;
    QSpacerItem *verticalSpacer_4;
    QWidget *hddTab;
    QGridLayout *gridLayout_23;
    QCheckBox *lockFilesCheckBox;
    QCheckBox *useReadCasheCheckBox;
    QCheckBox *lowPrioDiskCheckBox;
    QSpinBox *casheSizeLineEdit;
    QComboBox *diskIOCasheModeComboBox;
    QLabel *label_19;
    QCheckBox *useDiskReadAheadCheckBox;
    QLabel *label_20;
    QCheckBox *alowReorderedOpsCheckBox;
    QSpacerItem *verticalSpacer_3;
    QWidget *filteringTab;
    QGridLayout *gridLayout_24;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_5;
    QListWidget *GroupsListWidget;
    QPushButton *removeGroupButton;
    QGroupBox *groupBox_9;
    QGridLayout *gridLayout_8;
    QLineEdit *newGroupNameEdit;
    QPushButton *addNewGroupButton;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_6;
    QTextEdit *extensionsEdit;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_7;
    QLineEdit *groupSavePathEdit;
    QPushButton *BrowseGroupSavePathButton;
    QWidget *dtTab;
    QGridLayout *gridLayout_25;
    QGroupBox *groupBox_8;
    QGridLayout *gridLayout_11;
    QComboBox *driveNumberComboBox;
    QGroupBox *groupBox_6;
    QGridLayout *gridLayout_9;
    QLineEdit *DTPathEdit;
    QPushButton *browseDTPathButton;
    QGroupBox *customMoutGroupBox;
    QGridLayout *gridLayout_10;
    QLineEdit *customCommandEdit;
    QSpacerItem *verticalSpacer_2;
    QWidget *schedulingTab;
    QGridLayout *gridLayout_26;
    QLabel *label_14;
    QComboBox *tasksComboBox;
    QPushButton *addTaskButton;
    QPushButton *deleteTaskButton;
    QGroupBox *groupBox_11;
    QGridLayout *gridLayout_14;
    QDateTimeEdit *beginDateTimeEdit;
    QLabel *label_13;
    QLabel *label_9;
    QLineEdit *taskNameLineEdit;
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
    QCalendarWidget *calendarWidget;
    QWidget *webControllTab;
    QGridLayout *gridLayout_27;
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
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QListWidget *listWidget_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;
    QWidget *serchTab;
    QGridLayout *gridLayout_12;
    QGroupBox *groupBox_7;
    QGridLayout *gridLayout_13;
    QComboBox *sourcesComboBox;
    QLabel *label_24;
    QLineEdit *searchItemNameLineEdit;
    QLabel *label_25;
    QLineEdit *searchItemPatternLineEdit;
    QPushButton *addSearchButton;
    QPushButton *removeSearchButton;
    QSpacerItem *verticalSpacer;
    QWidget *keyMappingTab;
    QGridLayout *gridLayout_17;
    QScrollArea *hotKeyScrollArea;
    QWidget *keyMapContainer;
    QGridLayout *gridLayout_19;
    QListWidget *listWidget;

    void setupUi(QDialog *SettingsDialog)
    {
        if (SettingsDialog->objectName().isEmpty())
            SettingsDialog->setObjectName(QString::fromUtf8("SettingsDialog"));
        SettingsDialog->resize(793, 341);
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
        gridLayout_4->setSpacing(0);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setContentsMargins(6, 0, 6, 6);
        buttonBox = new QDialogButtonBox(m_centralWidget);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setMinimumSize(QSize(0, 23));
        buttonBox->setLayoutDirection(Qt::LeftToRight);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(false);

        gridLayout_4->addWidget(buttonBox, 2, 3, 1, 1);

        stackedWidget = new QStackedWidget(m_centralWidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setMaximumSize(QSize(16777215, 16777215));
        stackedWidget->setStyleSheet(QString::fromUtf8(""));
        torrentTab = new QWidget();
        torrentTab->setObjectName(QString::fromUtf8("torrentTab"));
        gridLayout_22 = new QGridLayout(torrentTab);
        gridLayout_22->setObjectName(QString::fromUtf8("gridLayout_22"));
        styleComboBox = new QComboBox(torrentTab);
        styleComboBox->setObjectName(QString::fromUtf8("styleComboBox"));

        gridLayout_22->addWidget(styleComboBox, 1, 1, 1, 1);

        runOnbootCheckBox = new QCheckBox(torrentTab);
        runOnbootCheckBox->setObjectName(QString::fromUtf8("runOnbootCheckBox"));

        gridLayout_22->addWidget(runOnbootCheckBox, 7, 0, 1, 2);

        label_26 = new QLabel(torrentTab);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        gridLayout_22->addWidget(label_26, 1, 0, 1, 1);

        magnetAssociationCheckBox = new QCheckBox(torrentTab);
        magnetAssociationCheckBox->setObjectName(QString::fromUtf8("magnetAssociationCheckBox"));

        gridLayout_22->addWidget(magnetAssociationCheckBox, 8, 2, 1, 1);

        startMinimizedCheckBox = new QCheckBox(torrentTab);
        startMinimizedCheckBox->setObjectName(QString::fromUtf8("startMinimizedCheckBox"));

        gridLayout_22->addWidget(startMinimizedCheckBox, 8, 0, 1, 2);

        asociationCheckBox = new QCheckBox(torrentTab);
        asociationCheckBox->setObjectName(QString::fromUtf8("asociationCheckBox"));

        gridLayout_22->addWidget(asociationCheckBox, 7, 2, 1, 1);

        label_8 = new QLabel(torrentTab);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_22->addWidget(label_8, 0, 0, 1, 1);

        localeComboBox = new QComboBox(torrentTab);
        localeComboBox->setObjectName(QString::fromUtf8("localeComboBox"));
        localeComboBox->setStyleSheet(QString::fromUtf8(""));

        gridLayout_22->addWidget(localeComboBox, 0, 1, 1, 1);

        groupBox_12 = new QGroupBox(torrentTab);
        groupBox_12->setObjectName(QString::fromUtf8("groupBox_12"));
        gridLayout_29 = new QGridLayout(groupBox_12);
        gridLayout_29->setObjectName(QString::fromUtf8("gridLayout_29"));
        useDHTCheckBox = new QGroupBox(groupBox_12);
        useDHTCheckBox->setObjectName(QString::fromUtf8("useDHTCheckBox"));
        useDHTCheckBox->setCheckable(true);
        useDHTCheckBox->setChecked(false);
        gridLayout_30 = new QGridLayout(useDHTCheckBox);
        gridLayout_30->setObjectName(QString::fromUtf8("gridLayout_30"));
        useDifDhtPorrtGroupBox = new QGroupBox(useDHTCheckBox);
        useDifDhtPorrtGroupBox->setObjectName(QString::fromUtf8("useDifDhtPorrtGroupBox"));
        useDifDhtPorrtGroupBox->setCheckable(true);
        useDifDhtPorrtGroupBox->setChecked(false);
        gridLayout_31 = new QGridLayout(useDifDhtPorrtGroupBox);
        gridLayout_31->setObjectName(QString::fromUtf8("gridLayout_31"));
        specDhtPortEdit = new QLineEdit(useDifDhtPorrtGroupBox);
        specDhtPortEdit->setObjectName(QString::fromUtf8("specDhtPortEdit"));

        gridLayout_31->addWidget(specDhtPortEdit, 0, 1, 1, 1);

        label_30 = new QLabel(useDifDhtPorrtGroupBox);
        label_30->setObjectName(QString::fromUtf8("label_30"));

        gridLayout_31->addWidget(label_30, 0, 0, 1, 1);


        gridLayout_30->addWidget(useDifDhtPorrtGroupBox, 1, 0, 1, 2);


        gridLayout_29->addWidget(useDHTCheckBox, 0, 0, 1, 1);

        usePExCheckBox = new QCheckBox(groupBox_12);
        usePExCheckBox->setObjectName(QString::fromUtf8("usePExCheckBox"));

        gridLayout_29->addWidget(usePExCheckBox, 1, 0, 1, 1);

        useLSDCheckBox = new QCheckBox(groupBox_12);
        useLSDCheckBox->setObjectName(QString::fromUtf8("useLSDCheckBox"));

        gridLayout_29->addWidget(useLSDCheckBox, 2, 0, 1, 1);


        gridLayout_22->addWidget(groupBox_12, 5, 2, 1, 1);

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


        gridLayout_22->addWidget(trackerGroupBox, 0, 2, 2, 1);

        groupBox_2 = new QGroupBox(torrentTab);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_3 = new QGridLayout(groupBox_2);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_3->addWidget(label_6, 0, 0, 1, 1);

        label_15 = new QLabel(groupBox_2);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout_3->addWidget(label_15, 3, 0, 1, 1);

        activeLimitEdit = new QLineEdit(groupBox_2);
        activeLimitEdit->setObjectName(QString::fromUtf8("activeLimitEdit"));

        gridLayout_3->addWidget(activeLimitEdit, 3, 1, 1, 1);

        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_3->addWidget(label_7, 2, 0, 1, 1);

        label_16 = new QLabel(groupBox_2);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        gridLayout_3->addWidget(label_16, 4, 0, 1, 1);

        activeSeedLimitEdit = new QLineEdit(groupBox_2);
        activeSeedLimitEdit->setObjectName(QString::fromUtf8("activeSeedLimitEdit"));

        gridLayout_3->addWidget(activeSeedLimitEdit, 4, 1, 1, 1);

        label_17 = new QLabel(groupBox_2);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        gridLayout_3->addWidget(label_17, 5, 0, 1, 1);

        activeDownloadLimitEdit = new QLineEdit(groupBox_2);
        activeDownloadLimitEdit->setObjectName(QString::fromUtf8("activeDownloadLimitEdit"));

        gridLayout_3->addWidget(activeDownloadLimitEdit, 5, 1, 1, 1);

        uploadLimitEdit = new QSpinBox(groupBox_2);
        uploadLimitEdit->setObjectName(QString::fromUtf8("uploadLimitEdit"));
        uploadLimitEdit->setMaximum(99999);

        gridLayout_3->addWidget(uploadLimitEdit, 0, 1, 1, 1);

        downloadLimitEdit = new QSpinBox(groupBox_2);
        downloadLimitEdit->setObjectName(QString::fromUtf8("downloadLimitEdit"));
        downloadLimitEdit->setMaximum(99999);

        gridLayout_3->addWidget(downloadLimitEdit, 2, 1, 1, 1);


        gridLayout_22->addWidget(groupBox_2, 5, 0, 1, 2);

        stackedWidget->addWidget(torrentTab);
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        gridLayout_20 = new QGridLayout(page);
        gridLayout_20->setObjectName(QString::fromUtf8("gridLayout_20"));
        portEdit = new QLineEdit(page);
        portEdit->setObjectName(QString::fromUtf8("portEdit"));

        gridLayout_20->addWidget(portEdit, 0, 2, 1, 1);

        label = new QLabel(page);
        label->setObjectName(QString::fromUtf8("label"));
        label->setLayoutDirection(Qt::LeftToRight);
        label->setAutoFillBackground(false);
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_20->addWidget(label, 0, 0, 1, 2);

        proxyGroupBox = new QGroupBox(page);
        proxyGroupBox->setObjectName(QString::fromUtf8("proxyGroupBox"));
        proxyGroupBox->setEnabled(true);
        proxyGroupBox->setCheckable(true);
        proxyGroupBox->setChecked(false);
        gridLayout = new QGridLayout(proxyGroupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_5 = new QLabel(proxyGroupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 3, 0, 1, 1);

        proxyUsernameEdit = new QLineEdit(proxyGroupBox);
        proxyUsernameEdit->setObjectName(QString::fromUtf8("proxyUsernameEdit"));

        gridLayout->addWidget(proxyUsernameEdit, 1, 1, 1, 1);

        proxyHostEdit = new QLineEdit(proxyGroupBox);
        proxyHostEdit->setObjectName(QString::fromUtf8("proxyHostEdit"));

        gridLayout->addWidget(proxyHostEdit, 0, 1, 1, 1);

        label_3 = new QLabel(proxyGroupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        label_2 = new QLabel(proxyGroupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        proxyPwdEdit = new QLineEdit(proxyGroupBox);
        proxyPwdEdit->setObjectName(QString::fromUtf8("proxyPwdEdit"));

        gridLayout->addWidget(proxyPwdEdit, 2, 1, 1, 1);

        proxyTypeComboBox = new QComboBox(proxyGroupBox);
        proxyTypeComboBox->setObjectName(QString::fromUtf8("proxyTypeComboBox"));

        gridLayout->addWidget(proxyTypeComboBox, 3, 1, 1, 1);

        label_4 = new QLabel(proxyGroupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 2, 0, 1, 1);


        gridLayout_20->addWidget(proxyGroupBox, 1, 0, 1, 3);

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

        gridLayout_28->addWidget(inEncPolicyComboBox, 0, 1, 1, 2);

        label_28 = new QLabel(groupBox_10);
        label_28->setObjectName(QString::fromUtf8("label_28"));

        gridLayout_28->addWidget(label_28, 1, 0, 1, 1);

        label_29 = new QLabel(groupBox_10);
        label_29->setObjectName(QString::fromUtf8("label_29"));

        gridLayout_28->addWidget(label_29, 2, 0, 1, 2);

        encLevelComboBox = new QComboBox(groupBox_10);
        encLevelComboBox->setObjectName(QString::fromUtf8("encLevelComboBox"));

        gridLayout_28->addWidget(encLevelComboBox, 2, 2, 1, 1);

        preferFullEncCheckBox = new QCheckBox(groupBox_10);
        preferFullEncCheckBox->setObjectName(QString::fromUtf8("preferFullEncCheckBox"));

        gridLayout_28->addWidget(preferFullEncCheckBox, 3, 0, 1, 3);

        outEncPolicyComboBox = new QComboBox(groupBox_10);
        outEncPolicyComboBox->setObjectName(QString::fromUtf8("outEncPolicyComboBox"));

        gridLayout_28->addWidget(outEncPolicyComboBox, 1, 1, 1, 2);


        gridLayout_20->addWidget(groupBox_10, 0, 3, 2, 1);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_20->addItem(verticalSpacer_4, 2, 0, 1, 1);

        stackedWidget->addWidget(page);
        hddTab = new QWidget();
        hddTab->setObjectName(QString::fromUtf8("hddTab"));
        gridLayout_23 = new QGridLayout(hddTab);
        gridLayout_23->setObjectName(QString::fromUtf8("gridLayout_23"));
        lockFilesCheckBox = new QCheckBox(hddTab);
        lockFilesCheckBox->setObjectName(QString::fromUtf8("lockFilesCheckBox"));

        gridLayout_23->addWidget(lockFilesCheckBox, 4, 0, 1, 1);

        useReadCasheCheckBox = new QCheckBox(hddTab);
        useReadCasheCheckBox->setObjectName(QString::fromUtf8("useReadCasheCheckBox"));

        gridLayout_23->addWidget(useReadCasheCheckBox, 3, 1, 1, 1);

        lowPrioDiskCheckBox = new QCheckBox(hddTab);
        lowPrioDiskCheckBox->setObjectName(QString::fromUtf8("lowPrioDiskCheckBox"));

        gridLayout_23->addWidget(lowPrioDiskCheckBox, 3, 0, 1, 1);

        casheSizeLineEdit = new QSpinBox(hddTab);
        casheSizeLineEdit->setObjectName(QString::fromUtf8("casheSizeLineEdit"));
        casheSizeLineEdit->setMaximum(512000);

        gridLayout_23->addWidget(casheSizeLineEdit, 1, 1, 1, 1);

        diskIOCasheModeComboBox = new QComboBox(hddTab);
        diskIOCasheModeComboBox->setObjectName(QString::fromUtf8("diskIOCasheModeComboBox"));

        gridLayout_23->addWidget(diskIOCasheModeComboBox, 0, 1, 1, 1);

        label_19 = new QLabel(hddTab);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        gridLayout_23->addWidget(label_19, 0, 0, 1, 1);

        useDiskReadAheadCheckBox = new QCheckBox(hddTab);
        useDiskReadAheadCheckBox->setObjectName(QString::fromUtf8("useDiskReadAheadCheckBox"));

        gridLayout_23->addWidget(useDiskReadAheadCheckBox, 2, 0, 1, 1);

        label_20 = new QLabel(hddTab);
        label_20->setObjectName(QString::fromUtf8("label_20"));

        gridLayout_23->addWidget(label_20, 1, 0, 1, 1);

        alowReorderedOpsCheckBox = new QCheckBox(hddTab);
        alowReorderedOpsCheckBox->setObjectName(QString::fromUtf8("alowReorderedOpsCheckBox"));

        gridLayout_23->addWidget(alowReorderedOpsCheckBox, 2, 1, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_23->addItem(verticalSpacer_3, 5, 0, 1, 3);

        stackedWidget->addWidget(hddTab);
        filteringTab = new QWidget();
        filteringTab->setObjectName(QString::fromUtf8("filteringTab"));
        gridLayout_24 = new QGridLayout(filteringTab);
        gridLayout_24->setObjectName(QString::fromUtf8("gridLayout_24"));
        groupBox_3 = new QGroupBox(filteringTab);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setMaximumSize(QSize(150, 16777215));
        gridLayout_5 = new QGridLayout(groupBox_3);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        GroupsListWidget = new QListWidget(groupBox_3);
        GroupsListWidget->setObjectName(QString::fromUtf8("GroupsListWidget"));

        gridLayout_5->addWidget(GroupsListWidget, 0, 1, 1, 1);

        removeGroupButton = new QPushButton(groupBox_3);
        removeGroupButton->setObjectName(QString::fromUtf8("removeGroupButton"));

        gridLayout_5->addWidget(removeGroupButton, 1, 1, 1, 1);


        gridLayout_24->addWidget(groupBox_3, 0, 0, 3, 1);

        groupBox_9 = new QGroupBox(filteringTab);
        groupBox_9->setObjectName(QString::fromUtf8("groupBox_9"));
        gridLayout_8 = new QGridLayout(groupBox_9);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        newGroupNameEdit = new QLineEdit(groupBox_9);
        newGroupNameEdit->setObjectName(QString::fromUtf8("newGroupNameEdit"));

        gridLayout_8->addWidget(newGroupNameEdit, 0, 0, 1, 1);

        addNewGroupButton = new QPushButton(groupBox_9);
        addNewGroupButton->setObjectName(QString::fromUtf8("addNewGroupButton"));

        gridLayout_8->addWidget(addNewGroupButton, 0, 1, 1, 1);


        gridLayout_24->addWidget(groupBox_9, 0, 1, 1, 1);

        groupBox_4 = new QGroupBox(filteringTab);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        gridLayout_6 = new QGridLayout(groupBox_4);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        extensionsEdit = new QTextEdit(groupBox_4);
        extensionsEdit->setObjectName(QString::fromUtf8("extensionsEdit"));

        gridLayout_6->addWidget(extensionsEdit, 0, 0, 1, 1);


        gridLayout_24->addWidget(groupBox_4, 1, 1, 1, 1);

        groupBox_5 = new QGroupBox(filteringTab);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        gridLayout_7 = new QGridLayout(groupBox_5);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        groupSavePathEdit = new QLineEdit(groupBox_5);
        groupSavePathEdit->setObjectName(QString::fromUtf8("groupSavePathEdit"));

        gridLayout_7->addWidget(groupSavePathEdit, 0, 0, 1, 1);

        BrowseGroupSavePathButton = new QPushButton(groupBox_5);
        BrowseGroupSavePathButton->setObjectName(QString::fromUtf8("BrowseGroupSavePathButton"));

        gridLayout_7->addWidget(BrowseGroupSavePathButton, 0, 1, 1, 1);


        gridLayout_24->addWidget(groupBox_5, 2, 1, 1, 1);

        stackedWidget->addWidget(filteringTab);
        dtTab = new QWidget();
        dtTab->setObjectName(QString::fromUtf8("dtTab"));
        gridLayout_25 = new QGridLayout(dtTab);
        gridLayout_25->setObjectName(QString::fromUtf8("gridLayout_25"));
        groupBox_8 = new QGroupBox(dtTab);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        gridLayout_11 = new QGridLayout(groupBox_8);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        driveNumberComboBox = new QComboBox(groupBox_8);
        driveNumberComboBox->setObjectName(QString::fromUtf8("driveNumberComboBox"));

        gridLayout_11->addWidget(driveNumberComboBox, 0, 0, 1, 1);


        gridLayout_25->addWidget(groupBox_8, 0, 1, 1, 1);

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
        label_14 = new QLabel(schedulingTab);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setMaximumSize(QSize(150, 16777215));

        gridLayout_26->addWidget(label_14, 0, 0, 1, 1);

        tasksComboBox = new QComboBox(schedulingTab);
        tasksComboBox->setObjectName(QString::fromUtf8("tasksComboBox"));
        tasksComboBox->setMaximumSize(QSize(150, 16777215));

        gridLayout_26->addWidget(tasksComboBox, 0, 1, 1, 1);

        addTaskButton = new QPushButton(schedulingTab);
        addTaskButton->setObjectName(QString::fromUtf8("addTaskButton"));

        gridLayout_26->addWidget(addTaskButton, 0, 2, 1, 1);

        deleteTaskButton = new QPushButton(schedulingTab);
        deleteTaskButton->setObjectName(QString::fromUtf8("deleteTaskButton"));

        gridLayout_26->addWidget(deleteTaskButton, 0, 3, 1, 1);

        groupBox_11 = new QGroupBox(schedulingTab);
        groupBox_11->setObjectName(QString::fromUtf8("groupBox_11"));
        gridLayout_14 = new QGridLayout(groupBox_11);
        gridLayout_14->setObjectName(QString::fromUtf8("gridLayout_14"));
        beginDateTimeEdit = new QDateTimeEdit(groupBox_11);
        beginDateTimeEdit->setObjectName(QString::fromUtf8("beginDateTimeEdit"));

        gridLayout_14->addWidget(beginDateTimeEdit, 1, 1, 1, 1);

        label_13 = new QLabel(groupBox_11);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_14->addWidget(label_13, 0, 0, 1, 1);

        label_9 = new QLabel(groupBox_11);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_14->addWidget(label_9, 1, 0, 1, 1);

        taskNameLineEdit = new QLineEdit(groupBox_11);
        taskNameLineEdit->setObjectName(QString::fromUtf8("taskNameLineEdit"));
        taskNameLineEdit->setMaximumSize(QSize(16777215, 16777215));

        gridLayout_14->addWidget(taskNameLineEdit, 0, 1, 1, 1);

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

        calendarWidget = new QCalendarWidget(groupBox_11);
        calendarWidget->setObjectName(QString::fromUtf8("calendarWidget"));

        gridLayout_14->addWidget(calendarWidget, 0, 2, 3, 1);

        label_9->raise();
        beginDateTimeEdit->raise();
        groupBox->raise();
        label_13->raise();
        taskNameLineEdit->raise();
        calendarWidget->raise();

        gridLayout_26->addWidget(groupBox_11, 1, 0, 1, 4);

        stackedWidget->addWidget(schedulingTab);
        webControllTab = new QWidget();
        webControllTab->setObjectName(QString::fromUtf8("webControllTab"));
        gridLayout_27 = new QGridLayout(webControllTab);
        gridLayout_27->setObjectName(QString::fromUtf8("gridLayout_27"));
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
        ipFilterTextEdit->setLineWrapColumnOrWidth(0);

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
        pushButton_2 = new QPushButton(page_2);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        gridLayout_33->addWidget(pushButton_2, 4, 0, 1, 1);

        pushButton_3 = new QPushButton(page_2);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));

        gridLayout_33->addWidget(pushButton_3, 4, 1, 1, 1);

        listWidget_2 = new QListWidget(page_2);
        listWidget_2->setObjectName(QString::fromUtf8("listWidget_2"));

        gridLayout_33->addWidget(listWidget_2, 1, 0, 3, 2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_33->addItem(horizontalSpacer, 0, 2, 5, 1);

        pushButton = new QPushButton(page_2);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout_33->addWidget(pushButton, 0, 0, 1, 2);

        stackedWidget->addWidget(page_2);
        serchTab = new QWidget();
        serchTab->setObjectName(QString::fromUtf8("serchTab"));
        gridLayout_12 = new QGridLayout(serchTab);
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        groupBox_7 = new QGroupBox(serchTab);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        gridLayout_13 = new QGridLayout(groupBox_7);
        gridLayout_13->setObjectName(QString::fromUtf8("gridLayout_13"));
        sourcesComboBox = new QComboBox(groupBox_7);
        sourcesComboBox->setObjectName(QString::fromUtf8("sourcesComboBox"));

        gridLayout_13->addWidget(sourcesComboBox, 0, 0, 1, 1);

        label_24 = new QLabel(groupBox_7);
        label_24->setObjectName(QString::fromUtf8("label_24"));

        gridLayout_13->addWidget(label_24, 1, 0, 1, 1);

        searchItemNameLineEdit = new QLineEdit(groupBox_7);
        searchItemNameLineEdit->setObjectName(QString::fromUtf8("searchItemNameLineEdit"));

        gridLayout_13->addWidget(searchItemNameLineEdit, 1, 1, 1, 1);

        label_25 = new QLabel(groupBox_7);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        gridLayout_13->addWidget(label_25, 2, 0, 1, 1);

        searchItemPatternLineEdit = new QLineEdit(groupBox_7);
        searchItemPatternLineEdit->setObjectName(QString::fromUtf8("searchItemPatternLineEdit"));

        gridLayout_13->addWidget(searchItemPatternLineEdit, 2, 1, 1, 1);

        addSearchButton = new QPushButton(groupBox_7);
        addSearchButton->setObjectName(QString::fromUtf8("addSearchButton"));

        gridLayout_13->addWidget(addSearchButton, 3, 0, 1, 1);

        removeSearchButton = new QPushButton(groupBox_7);
        removeSearchButton->setObjectName(QString::fromUtf8("removeSearchButton"));

        gridLayout_13->addWidget(removeSearchButton, 3, 1, 1, 1);


        gridLayout_12->addWidget(groupBox_7, 0, 0, 1, 2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_12->addItem(verticalSpacer, 1, 0, 1, 3);

        stackedWidget->addWidget(serchTab);
        keyMappingTab = new QWidget();
        keyMappingTab->setObjectName(QString::fromUtf8("keyMappingTab"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
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
        keyMapContainer->setGeometry(QRect(0, 0, 617, 286));
        gridLayout_19 = new QGridLayout(keyMapContainer);
        gridLayout_19->setObjectName(QString::fromUtf8("gridLayout_19"));
        hotKeyScrollArea->setWidget(keyMapContainer);

        gridLayout_17->addWidget(hotKeyScrollArea, 0, 0, 1, 1);

        stackedWidget->addWidget(keyMappingTab);

        gridLayout_4->addWidget(stackedWidget, 0, 2, 1, 2);

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


        verticalLayout->addWidget(m_centralWidget);

        QWidget::setTabOrder(activeLimitEdit, activeSeedLimitEdit);
        QWidget::setTabOrder(activeSeedLimitEdit, activeDownloadLimitEdit);
        QWidget::setTabOrder(activeDownloadLimitEdit, GroupsListWidget);
        QWidget::setTabOrder(GroupsListWidget, removeGroupButton);
        QWidget::setTabOrder(removeGroupButton, newGroupNameEdit);
        QWidget::setTabOrder(newGroupNameEdit, addNewGroupButton);
        QWidget::setTabOrder(addNewGroupButton, extensionsEdit);
        QWidget::setTabOrder(extensionsEdit, groupSavePathEdit);
        QWidget::setTabOrder(groupSavePathEdit, BrowseGroupSavePathButton);
        QWidget::setTabOrder(BrowseGroupSavePathButton, DTPathEdit);
        QWidget::setTabOrder(DTPathEdit, browseDTPathButton);
        QWidget::setTabOrder(browseDTPathButton, driveNumberComboBox);
        QWidget::setTabOrder(driveNumberComboBox, customCommandEdit);

        retranslateUi(SettingsDialog);
        QObject::connect(GroupsListWidget, SIGNAL(currentRowChanged(int)), SettingsDialog, SLOT(showSelectedGroup(int)));
        QObject::connect(addTaskButton, SIGNAL(clicked()), SettingsDialog, SLOT(AddTask()));
        QObject::connect(BrowseGroupSavePathButton, SIGNAL(clicked()), SettingsDialog, SLOT(browseSavepathGroup()));
        QObject::connect(deleteTaskButton, SIGNAL(clicked()), SettingsDialog, SLOT(DeleteTask()));
        QObject::connect(listWidget, SIGNAL(currentRowChanged(int)), stackedWidget, SLOT(setCurrentIndex(int)));
        QObject::connect(startRconButton, SIGNAL(clicked()), SettingsDialog, SLOT(StartRcon()));
        QObject::connect(stopRconButton, SIGNAL(clicked()), SettingsDialog, SLOT(StopRcon()));
        QObject::connect(addSearchButton, SIGNAL(clicked()), SettingsDialog, SLOT(addSearchitem()));
        QObject::connect(sourcesComboBox, SIGNAL(currentIndexChanged(int)), SettingsDialog, SLOT(searchItemChanged(int)));
        QObject::connect(removeSearchButton, SIGNAL(clicked()), SettingsDialog, SLOT(removeSearchItem()));
        QObject::connect(limitUlRadioButton, SIGNAL(clicked()), ulLimitEdit, SLOT(setFocus()));
        QObject::connect(removeGroupButton, SIGNAL(clicked()), SettingsDialog, SLOT(removeGroup()));
        QObject::connect(addNewGroupButton, SIGNAL(clicked()), SettingsDialog, SLOT(addGroup()));
        QObject::connect(browseDTPathButton, SIGNAL(clicked()), SettingsDialog, SLOT(browseDTPath()));
        QObject::connect(limitDlRadioButton, SIGNAL(clicked()), dlLimitEdit, SLOT(setFocus()));
        QObject::connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), SettingsDialog, SLOT(chooseAction(QAbstractButton*)));
        QObject::connect(openWebUiButton, SIGNAL(clicked()), SettingsDialog, SLOT(OpenWebUI()));

        stackedWidget->setCurrentIndex(0);
        inEncPolicyComboBox->setCurrentIndex(0);
        encLevelComboBox->setCurrentIndex(2);
        outEncPolicyComboBox->setCurrentIndex(0);
        listWidget->setCurrentRow(-1);


        QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
        SettingsDialog->setWindowTitle(QApplication::translate("SettingsDialog", "SETTINGS_DIALOG", 0, QApplication::UnicodeUTF8));
        tbMenu->setText(QString());
        LTitle->setText(QApplication::translate("SettingsDialog", "SETTINGS_DIALOG", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        runOnbootCheckBox->setWhatsThis(QApplication::translate("SettingsDialog", "START_ON_BOOT_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        runOnbootCheckBox->setText(QApplication::translate("SettingsDialog", "START_ON_BOOT", 0, QApplication::UnicodeUTF8));
        label_26->setText(QApplication::translate("SettingsDialog", "STYLE", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        magnetAssociationCheckBox->setWhatsThis(QApplication::translate("SettingsDialog", "MAGNET_ASSOCIATION_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        magnetAssociationCheckBox->setText(QApplication::translate("SettingsDialog", "MAGNET_ASSOCIATION", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        startMinimizedCheckBox->setWhatsThis(QApplication::translate("SettingsDialog", "START_MINIMIZED_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        startMinimizedCheckBox->setText(QApplication::translate("SettingsDialog", "START_MINIMIZED", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        asociationCheckBox->setWhatsThis(QApplication::translate("SettingsDialog", "TORRENT_FILES_ASSOCIATION_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        asociationCheckBox->setText(QApplication::translate("SettingsDialog", "TORRENT_FILES_ASSOCIATION", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("SettingsDialog", "LANGUAGE", 0, QApplication::UnicodeUTF8));
        groupBox_12->setTitle(QApplication::translate("SettingsDialog", "ADDITINAL_PEER_SOUECES", 0, QApplication::UnicodeUTF8));
        useDHTCheckBox->setTitle(QApplication::translate("SettingsDialog", "USE_DHT", 0, QApplication::UnicodeUTF8));
        useDifDhtPorrtGroupBox->setTitle(QApplication::translate("SettingsDialog", "USE_DIF_DHT_PORT", 0, QApplication::UnicodeUTF8));
        label_30->setText(QApplication::translate("SettingsDialog", "DHT_PORT", 0, QApplication::UnicodeUTF8));
        usePExCheckBox->setText(QApplication::translate("SettingsDialog", "USE_PEX", 0, QApplication::UnicodeUTF8));
        useLSDCheckBox->setText(QApplication::translate("SettingsDialog", "USE_LSD", 0, QApplication::UnicodeUTF8));
        trackerGroupBox->setTitle(QApplication::translate("SettingsDialog", "START_TRACKER", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("SettingsDialog", "TRACKER_PORT", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("SettingsDialog", "RESTRICTIONS", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        label_6->setWhatsThis(QApplication::translate("SettingsDialog", "UPLOAD_SPEED_LIMIT_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        label_6->setText(QApplication::translate("SettingsDialog", "UPLOAD_SPEED_LIMIT(Kb/s)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        label_15->setWhatsThis(QApplication::translate("SettingsDialog", "ACTIVE_TORRENTS_LIMIT_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        label_15->setText(QApplication::translate("SettingsDialog", "ACTIVE_TORRENTS_LIMIT", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        activeLimitEdit->setWhatsThis(QApplication::translate("SettingsDialog", "ACTIVE_TORRENTS_LIMIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_WHATSTHIS
        label_7->setWhatsThis(QApplication::translate("SettingsDialog", "DOWNLOAD_SPEED_LIMIT_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        label_7->setText(QApplication::translate("SettingsDialog", "DOWNLOAD_SPEED_LIMIT(Kb/s)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        label_16->setWhatsThis(QApplication::translate("SettingsDialog", "ACTIVE_SEEDS_LIMIT_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        label_16->setText(QApplication::translate("SettingsDialog", "ACTIVE_SEEDS_LIMIT", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        activeSeedLimitEdit->setWhatsThis(QApplication::translate("SettingsDialog", "ACTIVE_SEEDS_LIMIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_WHATSTHIS
        label_17->setWhatsThis(QApplication::translate("SettingsDialog", "ACTIVE_DOWNLOADS_LIMIT_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        label_17->setText(QApplication::translate("SettingsDialog", "ACTIVE_DOWNLOADS_LIMIT", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        activeDownloadLimitEdit->setWhatsThis(QApplication::translate("SettingsDialog", "ACTIVE_DOWNLOADS_LIMIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        uploadLimitEdit->setSpecialValueText(QApplication::translate("SettingsDialog", "None", 0, QApplication::UnicodeUTF8));
        uploadLimitEdit->setSuffix(QApplication::translate("SettingsDialog", " Kb\\s", 0, QApplication::UnicodeUTF8));
        uploadLimitEdit->setPrefix(QString());
        downloadLimitEdit->setSpecialValueText(QApplication::translate("SettingsDialog", "None", 0, QApplication::UnicodeUTF8));
        downloadLimitEdit->setSuffix(QApplication::translate("SettingsDialog", " Kb\\s", 0, QApplication::UnicodeUTF8));
        downloadLimitEdit->setPrefix(QString());
        label->setText(QApplication::translate("SettingsDialog", "LISTEN_PORT", 0, QApplication::UnicodeUTF8));
        proxyGroupBox->setTitle(QApplication::translate("SettingsDialog", "USE_PROXY", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        label_5->setWhatsThis(QApplication::translate("SettingsDialog", "PROXY_TYPE_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        label_5->setText(QApplication::translate("SettingsDialog", "PROXY_TYPE", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        proxyUsernameEdit->setWhatsThis(QApplication::translate("SettingsDialog", "PROXY_USER_NAME_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_WHATSTHIS
        proxyHostEdit->setWhatsThis(QApplication::translate("SettingsDialog", "PROXY_HOST_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_WHATSTHIS
        label_3->setWhatsThis(QApplication::translate("SettingsDialog", "PROXY_USER_NAME_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        label_3->setText(QApplication::translate("SettingsDialog", "PROXY_USER_NAME", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        label_2->setWhatsThis(QApplication::translate("SettingsDialog", "PROXY_HOST_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        label_2->setText(QApplication::translate("SettingsDialog", "PROXY_HOST (IP:port)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        proxyPwdEdit->setWhatsThis(QApplication::translate("SettingsDialog", "PROXY_PASSWORD", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        proxyTypeComboBox->clear();
        proxyTypeComboBox->insertItems(0, QStringList()
         << QApplication::translate("SettingsDialog", "Socks4", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingsDialog", "Socks5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingsDialog", "Socks5 \321\201 \320\277\320\260\321\200\320\276\320\273\320\265\320\274", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingsDialog", "http", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingsDialog", "http \321\201 \320\277\320\260\321\200\320\276\320\273\320\265\320\274", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingsDialog", "i2p", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_WHATSTHIS
        proxyTypeComboBox->setWhatsThis(QApplication::translate("SettingsDialog", "PROXY_TYPE", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_WHATSTHIS
        label_4->setWhatsThis(QApplication::translate("SettingsDialog", "PROXY_PASSWORD_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        label_4->setText(QApplication::translate("SettingsDialog", "PROXY_PASSWORD", 0, QApplication::UnicodeUTF8));
        groupBox_10->setTitle(QApplication::translate("SettingsDialog", "NET_ENC", 0, QApplication::UnicodeUTF8));
        label_27->setText(QApplication::translate("SettingsDialog", "IN_ENC_POLICITY", 0, QApplication::UnicodeUTF8));
        inEncPolicyComboBox->clear();
        inEncPolicyComboBox->insertItems(0, QStringList()
         << QApplication::translate("SettingsDialog", "FORCED", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingsDialog", "ENABLED", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingsDialog", "DISABLED", 0, QApplication::UnicodeUTF8)
        );
        label_28->setText(QApplication::translate("SettingsDialog", "OUT_ENC_POLICITY", 0, QApplication::UnicodeUTF8));
        label_29->setText(QApplication::translate("SettingsDialog", "ENCRYPTION_LEVEL", 0, QApplication::UnicodeUTF8));
        encLevelComboBox->clear();
        encLevelComboBox->insertItems(0, QStringList()
         << QApplication::translate("SettingsDialog", "PROTOCOL_ENC_ONLY", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingsDialog", "FULL_ENC", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingsDialog", "BOTH", 0, QApplication::UnicodeUTF8)
        );
        preferFullEncCheckBox->setText(QApplication::translate("SettingsDialog", "PREFER_FULL_ENCRYPTION", 0, QApplication::UnicodeUTF8));
        outEncPolicyComboBox->clear();
        outEncPolicyComboBox->insertItems(0, QStringList()
         << QApplication::translate("SettingsDialog", "FORCED", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingsDialog", "ENABLED", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingsDialog", "DISABLED", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_WHATSTHIS
        lockFilesCheckBox->setWhatsThis(QApplication::translate("SettingsDialog", "LOCK_FILES_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        lockFilesCheckBox->setText(QApplication::translate("SettingsDialog", "LOCK_FILES", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        useReadCasheCheckBox->setWhatsThis(QApplication::translate("SettingsDialog", "USE_READ_CASHE_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        useReadCasheCheckBox->setText(QApplication::translate("SettingsDialog", "USE_READ_CASHE", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        lowPrioDiskCheckBox->setWhatsThis(QApplication::translate("SettingsDialog", "LOW_PORIO_DISK_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        lowPrioDiskCheckBox->setText(QApplication::translate("SettingsDialog", "LOW_PORIO_DISK", 0, QApplication::UnicodeUTF8));
        casheSizeLineEdit->setSpecialValueText(QApplication::translate("SettingsDialog", "None", 0, QApplication::UnicodeUTF8));
        casheSizeLineEdit->setSuffix(QApplication::translate("SettingsDialog", " Kb", 0, QApplication::UnicodeUTF8));
        casheSizeLineEdit->setPrefix(QString());
        diskIOCasheModeComboBox->clear();
        diskIOCasheModeComboBox->insertItems(0, QStringList()
         << QApplication::translate("SettingsDialog", "ENABLE_OS_CACHE", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingsDialog", "DISABLE_OS_CACHE_FOR_ALIGNED_FILES", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingsDialog", "DISABLE_OS_CACHE", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_WHATSTHIS
        label_19->setWhatsThis(QApplication::translate("SettingsDialog", "DISK_IO_CASH_MODE_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        label_19->setText(QApplication::translate("SettingsDialog", "DISK_IO_CASH_MODE", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        useDiskReadAheadCheckBox->setWhatsThis(QApplication::translate("SettingsDialog", "USE_DISK_READ_AHEAD_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        useDiskReadAheadCheckBox->setText(QApplication::translate("SettingsDialog", "USE_DISK_READ_AHEAD", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        label_20->setWhatsThis(QApplication::translate("SettingsDialog", "CASH_SIZE_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        label_20->setText(QApplication::translate("SettingsDialog", "CASH_SIZE", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        alowReorderedOpsCheckBox->setWhatsThis(QApplication::translate("SettingsDialog", "ALOW_REORDERED_OPERATIONS_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        alowReorderedOpsCheckBox->setText(QApplication::translate("SettingsDialog", "ALOW_REORDERED_OPERATIONS", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("SettingsDialog", "FILTERING_GROUP", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        GroupsListWidget->setWhatsThis(QApplication::translate("SettingsDialog", "GROUP_LISTBOX_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_WHATSTHIS
        removeGroupButton->setWhatsThis(QApplication::translate("SettingsDialog", "FILTERING_GROUP_DELETE_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        removeGroupButton->setText(QApplication::translate("SettingsDialog", "FILTERING_GROUP_DELETE", 0, QApplication::UnicodeUTF8));
        groupBox_9->setTitle(QApplication::translate("SettingsDialog", "ADD_CHANGE_GROUP", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        newGroupNameEdit->setWhatsThis(QApplication::translate("SettingsDialog", "newGroupNameEdit_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_WHATSTHIS
        addNewGroupButton->setWhatsThis(QApplication::translate("SettingsDialog", "ADD_GROUP_BUTON_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        addNewGroupButton->setText(QApplication::translate("SettingsDialog", "ADD_GROUP_BUTON", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("SettingsDialog", "FILE_EXTENSIONS(FORMAT: jpg|png|bmp)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        extensionsEdit->setWhatsThis(QApplication::translate("SettingsDialog", "extensionsEdit_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        groupBox_5->setTitle(QApplication::translate("SettingsDialog", "GROUP_SAVE_PATH", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        groupSavePathEdit->setWhatsThis(QApplication::translate("SettingsDialog", "groupSavePathEdit_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_WHATSTHIS
        BrowseGroupSavePathButton->setWhatsThis(QApplication::translate("SettingsDialog", "GROUP_SAVE_PATH_CHOISE_BUTTON_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        BrowseGroupSavePathButton->setText(QApplication::translate("SettingsDialog", "GROUP_SAVE_PATH_CHOISE_BUTTON", 0, QApplication::UnicodeUTF8));
        groupBox_8->setTitle(QApplication::translate("SettingsDialog", "DRIVE_NUMBER", 0, QApplication::UnicodeUTF8));
        driveNumberComboBox->clear();
        driveNumberComboBox->insertItems(0, QStringList()
         << QApplication::translate("SettingsDialog", "1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingsDialog", "2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingsDialog", "3", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingsDialog", "4", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingsDialog", "5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingsDialog", "6", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingsDialog", "7", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingsDialog", "8", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingsDialog", "9", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingsDialog", "10", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_WHATSTHIS
        driveNumberComboBox->setWhatsThis(QApplication::translate("SettingsDialog", "driveNumberComboBox_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        groupBox_6->setTitle(QApplication::translate("SettingsDialog", "DAEMONTOOLS_PATH", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        DTPathEdit->setWhatsThis(QApplication::translate("SettingsDialog", "DTPathEdit_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_ACCESSIBILITY
        browseDTPathButton->setAccessibleName(QApplication::translate("SettingsDialog", "DAEMONTOOLS_PATH_CHOISE_BUUTON_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        browseDTPathButton->setText(QApplication::translate("SettingsDialog", "DAEMONTOOLS_PATH_CHOISE_BUUTON", 0, QApplication::UnicodeUTF8));
        customMoutGroupBox->setTitle(QApplication::translate("SettingsDialog", "DAEMONTOOLS_CUSTOM_COMMAND", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        customCommandEdit->setWhatsThis(QApplication::translate("SettingsDialog", "DAEMONTOOLS_COMMAND_WIT", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        label_14->setText(QApplication::translate("SettingsDialog", "ACTIONS", 0, QApplication::UnicodeUTF8));
        addTaskButton->setText(QApplication::translate("SettingsDialog", "ADD_BUTTON", 0, QApplication::UnicodeUTF8));
        deleteTaskButton->setText(QApplication::translate("SettingsDialog", "DELETE_BUTTON", 0, QApplication::UnicodeUTF8));
        groupBox_11->setTitle(QApplication::translate("SettingsDialog", "GroupBox", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("SettingsDialog", "ACTION_NAME", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("SettingsDialog", "ACTION_BEGIN", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("SettingsDialog", "ACTIONS", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("SettingsDialog", "LIMIT_DOWNLOAD", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("SettingsDialog", "LIMIT_UPLOAD", 0, QApplication::UnicodeUTF8));
        startAllRadioButton->setText(QApplication::translate("SettingsDialog", "START_ALL", 0, QApplication::UnicodeUTF8));
        limitUlRadioButton->setText(QApplication::translate("SettingsDialog", "LIMIT_UPLOAD", 0, QApplication::UnicodeUTF8));
        limitDlRadioButton->setText(QApplication::translate("SettingsDialog", "LIMIT_DOWNLOAD", 0, QApplication::UnicodeUTF8));
        pauseAllRadioButton->setText(QApplication::translate("SettingsDialog", "PUASE_ALL", 0, QApplication::UnicodeUTF8));
        webUIGroupBox->setTitle(QApplication::translate("SettingsDialog", "WEB_CONTROLL_ENABLED", 0, QApplication::UnicodeUTF8));
        IPFilterGroupBox->setTitle(QApplication::translate("SettingsDialog", "WEB_UI_USE_IP_FILTER", 0, QApplication::UnicodeUTF8));
        ipFilterTextEdit->setHtml(QApplication::translate("SettingsDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"><br /></p></body></html>", 0, QApplication::UnicodeUTF8));
        webUILogginGroupBox->setTitle(QApplication::translate("SettingsDialog", "WEB_UI_LOGGING_ENABLED", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("SettingsDialog", "WEB_UI_LOG_NAME", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("SettingsDialog", "WEB_UI_LOGIN", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("SettingsDialog", "WEB_UI_PASSWORD", 0, QApplication::UnicodeUTF8));
        upnpCheckBox->setText(QApplication::translate("SettingsDialog", "WEB_UI_UPNP_ENABLED", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("SettingsDialog", "WEB_UI_PORT", 0, QApplication::UnicodeUTF8));
        startRconButton->setText(QApplication::translate("SettingsDialog", "WEBUI_START", 0, QApplication::UnicodeUTF8));
        stopRconButton->setText(QApplication::translate("SettingsDialog", "WEBUI_STOP", 0, QApplication::UnicodeUTF8));
        RunningLabel->setText(QString());
        openWebUiButton->setText(QApplication::translate("SettingsDialog", "OPEN_WEBUI", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("SettingsDialog", "RSS_RULES_IMPORT", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("SettingsDialog", "RSS_RULES_EXPORT", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("SettingsDialog", "ADD_RSS_DOWNLOAD_RULE", 0, QApplication::UnicodeUTF8));
        groupBox_7->setTitle(QString());
        label_24->setText(QApplication::translate("SettingsDialog", "SEARCH_NAME", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("SettingsDialog", "SEARCH_PATERN", 0, QApplication::UnicodeUTF8));
        addSearchButton->setText(QApplication::translate("SettingsDialog", "ADD_SEARCH", 0, QApplication::UnicodeUTF8));
        removeSearchButton->setText(QApplication::translate("SettingsDialog", "REMOVE_SEARCH", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = listWidget->isSortingEnabled();
        listWidget->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = listWidget->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("SettingsDialog", "TAB_TORRENT", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem1 = listWidget->item(1);
        ___qlistwidgetitem1->setText(QApplication::translate("SettingsDialog", "TAB_NETWORK", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem2 = listWidget->item(2);
        ___qlistwidgetitem2->setText(QApplication::translate("SettingsDialog", "TAB_HDD", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem3 = listWidget->item(3);
        ___qlistwidgetitem3->setText(QApplication::translate("SettingsDialog", "TAB_TORRENT_FILTERING", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem4 = listWidget->item(4);
        ___qlistwidgetitem4->setText(QApplication::translate("SettingsDialog", "TAB_DAEMONTOOLS", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem5 = listWidget->item(5);
        ___qlistwidgetitem5->setText(QApplication::translate("SettingsDialog", "TAB_SCEDULER", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem6 = listWidget->item(6);
        ___qlistwidgetitem6->setText(QApplication::translate("SettingsDialog", "TAB_WEB_CONTROL", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem7 = listWidget->item(7);
        ___qlistwidgetitem7->setText(QApplication::translate("SettingsDialog", "TAB_RSS", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem8 = listWidget->item(8);
        ___qlistwidgetitem8->setText(QApplication::translate("SettingsDialog", "TAB_SEARCH", 0, QApplication::UnicodeUTF8));
        QListWidgetItem *___qlistwidgetitem9 = listWidget->item(9);
        ___qlistwidgetitem9->setText(QApplication::translate("SettingsDialog", "TAB_KEY_MAPPINGS", 0, QApplication::UnicodeUTF8));
        listWidget->setSortingEnabled(__sortingEnabled);

    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSDIALOG_H
