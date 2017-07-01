/********************************************************************************
** Form generated from reading UI file 'AddDownloadRuleDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDDOWNLOADRULEDIALOG_H
#define UI_ADDDOWNLOADRULEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AddRssDownloadRuleDialog
{
public:
    QVBoxLayout *m_windowLayout;
    QWidget *titleBar;
    QHBoxLayout *horizontalLayout;
    QLabel *tbMenu;
    QLabel *LTitle;
    QPushButton *pbClose;
    QWidget *m_centralWidget;
    QGridLayout *gridLayout_5;
    QSplitter *splitter;
    QWidget *m_ruleSettingsContainer;
    QGridLayout *gridLayout_4;
    QLabel *label_5;
    QComboBox *m_pSearchTypeComboBox;
    QLineEdit *m_pRuleNameEdit;
    QComboBox *m_pRuleTypeCombobx;
    QLineEdit *m_pSearchStringEdit;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QGroupBox *m_pUseSavePathGroupBox;
    QGridLayout *gridLayout_3;
    QLabel *label_6;
    QLineEdit *m_pSavePathEdit;
    QPushButton *m_pBrowseButton;
    QCheckBox *m_pUseGroupsCheckBox;
    QLabel *m_pRuleTypeHintLabel;
    QWidget *widget_2;
    QGridLayout *gridLayout;
    QListWidget *m_pFeedsListWidget;
    QLabel *label_4;
    QWidget *widget;
    QGridLayout *gridLayout_2;
    QLabel *label_7;
    QTreeView *m_pFilteredFeedsView;
    QDialogButtonBox *buttonBox;

    void setupUi(QWidget *AddRssDownloadRuleDialog)
    {
        if (AddRssDownloadRuleDialog->objectName().isEmpty())
            AddRssDownloadRuleDialog->setObjectName(QString::fromUtf8("AddRssDownloadRuleDialog"));
        AddRssDownloadRuleDialog->resize(767, 300);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/app.ico"), QSize(), QIcon::Normal, QIcon::Off);
        AddRssDownloadRuleDialog->setWindowIcon(icon);
        m_windowLayout = new QVBoxLayout(AddRssDownloadRuleDialog);
        m_windowLayout->setSpacing(0);
        m_windowLayout->setObjectName(QString::fromUtf8("m_windowLayout"));
        m_windowLayout->setContentsMargins(1, 1, 1, 0);
        titleBar = new QWidget(AddRssDownloadRuleDialog);
        titleBar->setObjectName(QString::fromUtf8("titleBar"));
        titleBar->setMinimumSize(QSize(0, 23));
        titleBar->setMaximumSize(QSize(16777215, 23));
        titleBar->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout = new QHBoxLayout(titleBar);
        horizontalLayout->setSpacing(4);
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
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(8);
        font.setBold(false);
        font.setWeight(50);
        font.setStyleStrategy(QFont::PreferDefault);
        LTitle->setFont(font);
        LTitle->setStyleSheet(QString::fromUtf8(""));
        LTitle->setTextFormat(Qt::AutoText);

        horizontalLayout->addWidget(LTitle);

        pbClose = new QPushButton(titleBar);
        pbClose->setObjectName(QString::fromUtf8("pbClose"));
        pbClose->setMinimumSize(QSize(22, 22));
        pbClose->setMaximumSize(QSize(22, 22));
        pbClose->setIconSize(QSize(22, 22));
        pbClose->setFlat(true);

        horizontalLayout->addWidget(pbClose);


        m_windowLayout->addWidget(titleBar);

        m_centralWidget = new QWidget(AddRssDownloadRuleDialog);
        m_centralWidget->setObjectName(QString::fromUtf8("m_centralWidget"));
        gridLayout_5 = new QGridLayout(m_centralWidget);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        splitter = new QSplitter(m_centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        m_ruleSettingsContainer = new QWidget(splitter);
        m_ruleSettingsContainer->setObjectName(QString::fromUtf8("m_ruleSettingsContainer"));
        gridLayout_4 = new QGridLayout(m_ruleSettingsContainer);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        label_5 = new QLabel(m_ruleSettingsContainer);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_4->addWidget(label_5, 0, 0, 1, 1);

        m_pSearchTypeComboBox = new QComboBox(m_ruleSettingsContainer);
        m_pSearchTypeComboBox->setObjectName(QString::fromUtf8("m_pSearchTypeComboBox"));

        gridLayout_4->addWidget(m_pSearchTypeComboBox, 3, 1, 1, 1);

        m_pRuleNameEdit = new QLineEdit(m_ruleSettingsContainer);
        m_pRuleNameEdit->setObjectName(QString::fromUtf8("m_pRuleNameEdit"));

        gridLayout_4->addWidget(m_pRuleNameEdit, 0, 1, 1, 1);

        m_pRuleTypeCombobx = new QComboBox(m_ruleSettingsContainer);
        m_pRuleTypeCombobx->setObjectName(QString::fromUtf8("m_pRuleTypeCombobx"));

        gridLayout_4->addWidget(m_pRuleTypeCombobx, 1, 1, 1, 1);

        m_pSearchStringEdit = new QLineEdit(m_ruleSettingsContainer);
        m_pSearchStringEdit->setObjectName(QString::fromUtf8("m_pSearchStringEdit"));

        gridLayout_4->addWidget(m_pSearchStringEdit, 4, 1, 1, 1);

        label = new QLabel(m_ruleSettingsContainer);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_4->addWidget(label, 1, 0, 1, 1);

        label_2 = new QLabel(m_ruleSettingsContainer);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_4->addWidget(label_2, 4, 0, 1, 1);

        label_3 = new QLabel(m_ruleSettingsContainer);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_4->addWidget(label_3, 3, 0, 1, 1);

        m_pUseSavePathGroupBox = new QGroupBox(m_ruleSettingsContainer);
        m_pUseSavePathGroupBox->setObjectName(QString::fromUtf8("m_pUseSavePathGroupBox"));
        m_pUseSavePathGroupBox->setCheckable(true);
        m_pUseSavePathGroupBox->setChecked(false);
        gridLayout_3 = new QGridLayout(m_pUseSavePathGroupBox);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_6 = new QLabel(m_pUseSavePathGroupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_3->addWidget(label_6, 0, 0, 1, 1);

        m_pSavePathEdit = new QLineEdit(m_pUseSavePathGroupBox);
        m_pSavePathEdit->setObjectName(QString::fromUtf8("m_pSavePathEdit"));

        gridLayout_3->addWidget(m_pSavePathEdit, 0, 1, 1, 1);

        m_pBrowseButton = new QPushButton(m_pUseSavePathGroupBox);
        m_pBrowseButton->setObjectName(QString::fromUtf8("m_pBrowseButton"));

        gridLayout_3->addWidget(m_pBrowseButton, 0, 2, 1, 1);


        gridLayout_4->addWidget(m_pUseSavePathGroupBox, 5, 0, 1, 2);

        m_pUseGroupsCheckBox = new QCheckBox(m_ruleSettingsContainer);
        m_pUseGroupsCheckBox->setObjectName(QString::fromUtf8("m_pUseGroupsCheckBox"));

        gridLayout_4->addWidget(m_pUseGroupsCheckBox, 6, 0, 1, 2);

        m_pRuleTypeHintLabel = new QLabel(m_ruleSettingsContainer);
        m_pRuleTypeHintLabel->setObjectName(QString::fromUtf8("m_pRuleTypeHintLabel"));
        m_pRuleTypeHintLabel->setWordWrap(true);

        gridLayout_4->addWidget(m_pRuleTypeHintLabel, 2, 0, 1, 2);

        splitter->addWidget(m_ruleSettingsContainer);
        widget_2 = new QWidget(splitter);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        gridLayout = new QGridLayout(widget_2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        m_pFeedsListWidget = new QListWidget(widget_2);
        m_pFeedsListWidget->setObjectName(QString::fromUtf8("m_pFeedsListWidget"));

        gridLayout->addWidget(m_pFeedsListWidget, 1, 0, 1, 1);

        label_4 = new QLabel(widget_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 0, 0, 1, 1);

        splitter->addWidget(widget_2);
        widget = new QWidget(splitter);
        widget->setObjectName(QString::fromUtf8("widget"));
        gridLayout_2 = new QGridLayout(widget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_7 = new QLabel(widget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_2->addWidget(label_7, 0, 0, 1, 1);

        m_pFilteredFeedsView = new QTreeView(widget);
        m_pFilteredFeedsView->setObjectName(QString::fromUtf8("m_pFilteredFeedsView"));
        m_pFilteredFeedsView->setMouseTracking(true);
        m_pFilteredFeedsView->setContextMenuPolicy(Qt::CustomContextMenu);
        m_pFilteredFeedsView->setAnimated(true);
        m_pFilteredFeedsView->setHeaderHidden(true);

        gridLayout_2->addWidget(m_pFilteredFeedsView, 1, 0, 1, 1);

        splitter->addWidget(widget);

        gridLayout_5->addWidget(splitter, 0, 0, 1, 1);

        buttonBox = new QDialogButtonBox(m_centralWidget);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout_5->addWidget(buttonBox, 1, 0, 1, 1);


        m_windowLayout->addWidget(m_centralWidget);

        QWidget::setTabOrder(m_pRuleNameEdit, m_pRuleTypeCombobx);
        QWidget::setTabOrder(m_pRuleTypeCombobx, m_pSearchTypeComboBox);
        QWidget::setTabOrder(m_pSearchTypeComboBox, m_pSearchStringEdit);
        QWidget::setTabOrder(m_pSearchStringEdit, m_pUseSavePathGroupBox);
        QWidget::setTabOrder(m_pUseSavePathGroupBox, m_pSavePathEdit);
        QWidget::setTabOrder(m_pSavePathEdit, m_pBrowseButton);
        QWidget::setTabOrder(m_pBrowseButton, m_pUseGroupsCheckBox);
        QWidget::setTabOrder(m_pUseGroupsCheckBox, m_pFeedsListWidget);
        QWidget::setTabOrder(m_pFeedsListWidget, m_pFilteredFeedsView);
        QWidget::setTabOrder(m_pFilteredFeedsView, buttonBox);
        QWidget::setTabOrder(buttonBox, pbClose);

        retranslateUi(AddRssDownloadRuleDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), AddRssDownloadRuleDialog, SLOT(onApplyRule()));
        QObject::connect(buttonBox, SIGNAL(rejected()), AddRssDownloadRuleDialog, SLOT(onCancel()));
        QObject::connect(m_pRuleNameEdit, SIGNAL(textEdited(QString)), AddRssDownloadRuleDialog, SLOT(onChange()));
        QObject::connect(m_pRuleTypeCombobx, SIGNAL(currentIndexChanged(int)), AddRssDownloadRuleDialog, SLOT(onChange()));
        QObject::connect(m_pSearchTypeComboBox, SIGNAL(currentIndexChanged(int)), AddRssDownloadRuleDialog, SLOT(onChange()));
        QObject::connect(m_pSearchStringEdit, SIGNAL(textEdited(QString)), AddRssDownloadRuleDialog, SLOT(onChange()));
        QObject::connect(m_pUseSavePathGroupBox, SIGNAL(clicked()), AddRssDownloadRuleDialog, SLOT(onChange()));
        QObject::connect(m_pSavePathEdit, SIGNAL(textEdited(QString)), AddRssDownloadRuleDialog, SLOT(onChange()));
        QObject::connect(m_pUseGroupsCheckBox, SIGNAL(stateChanged(int)), AddRssDownloadRuleDialog, SLOT(onChange()));
        QObject::connect(m_pRuleTypeCombobx, SIGNAL(currentIndexChanged(int)), AddRssDownloadRuleDialog, SLOT(onUpdateRuleTypeHint()));
        QObject::connect(m_pBrowseButton, SIGNAL(clicked()), AddRssDownloadRuleDialog, SLOT(onBrowseStaticPath()));

        QMetaObject::connectSlotsByName(AddRssDownloadRuleDialog);
    } // setupUi

    void retranslateUi(QWidget *AddRssDownloadRuleDialog)
    {
        AddRssDownloadRuleDialog->setWindowTitle(QApplication::translate("AddRssDownloadRuleDialog", "ADD_RSS_DWONLOAD_RULE_DIALOG", 0));
        tbMenu->setText(QString());
        LTitle->setText(QApplication::translate("AddRssDownloadRuleDialog", "ADD_RSS_DWONLOAD_RULE_DIALOG", 0));
        label_5->setText(QApplication::translate("AddRssDownloadRuleDialog", "RULE_NAME", 0));
        label->setText(QApplication::translate("AddRssDownloadRuleDialog", "RULE_TYPE", 0));
        label_2->setText(QApplication::translate("AddRssDownloadRuleDialog", "SEARCH_STRING", 0));
        label_3->setText(QApplication::translate("AddRssDownloadRuleDialog", "SEARCH_TYPE", 0));
        m_pUseSavePathGroupBox->setTitle(QApplication::translate("AddRssDownloadRuleDialog", "USE_SAVE_PATH", 0));
        label_6->setText(QApplication::translate("AddRssDownloadRuleDialog", "SAVE_PATH", 0));
        m_pBrowseButton->setText(QApplication::translate("AddRssDownloadRuleDialog", "BROWSE", 0));
        m_pUseGroupsCheckBox->setText(QApplication::translate("AddRssDownloadRuleDialog", "USE_GROUPS_FOR_FILTERING", 0));
        m_pRuleTypeHintLabel->setText(QApplication::translate("AddRssDownloadRuleDialog", "RULE_HINT", 0));
        label_4->setText(QApplication::translate("AddRssDownloadRuleDialog", "APPLY_TO_FEEDS:", 0));
        label_7->setText(QApplication::translate("AddRssDownloadRuleDialog", "FEED_ITEMS_MATCHED_BY_CURRENT_RULE:", 0));
    } // retranslateUi

};

namespace Ui {
    class AddRssDownloadRuleDialog: public Ui_AddRssDownloadRuleDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDDOWNLOADRULEDIALOG_H
