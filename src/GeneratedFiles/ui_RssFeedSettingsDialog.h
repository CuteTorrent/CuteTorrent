/********************************************************************************
** Form generated from reading UI file 'RssFeedSettingsDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RSSFEEDSETTINGSDIALOG_H
#define UI_RSSFEEDSETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RssSettings
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *titleBar;
    QHBoxLayout *horizontalLayout;
    QLabel *tbMenu;
    QLabel *LTitle;
    QPushButton *pbClose;
    QWidget *m_centralWidget;
    QGridLayout *gridLayout;
    QDialogButtonBox *buttonBox;
    QLineEdit *m_pRssUrlEdit;
    QGroupBox *m_pRssAutoDownloadGroupBox;
    QGridLayout *gridLayout_2;
    QCheckBox *m_pUseGroupsCheckBox;
    QGroupBox *m_pUseSavePathGroupBox;
    QGridLayout *gridLayout_3;
    QLabel *label_2;
    QLineEdit *m_pSavePathEdit;
    QPushButton *m_pBrowseButton;
    QLabel *label;
    QLabel *label_3;
    QSpinBox *m_pRefreshRateSpinBox;

    void setupUi(QWidget *RssSettings)
    {
        if (RssSettings->objectName().isEmpty())
            RssSettings->setObjectName(QString::fromUtf8("RssSettings"));
        RssSettings->resize(417, 235);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/app.ico"), QSize(), QIcon::Normal, QIcon::Off);
        RssSettings->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(RssSettings);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(1, 1, 1, 0);
        titleBar = new QWidget(RssSettings);
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


        verticalLayout->addWidget(titleBar);

        m_centralWidget = new QWidget(RssSettings);
        m_centralWidget->setObjectName(QString::fromUtf8("m_centralWidget"));
        gridLayout = new QGridLayout(m_centralWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        buttonBox = new QDialogButtonBox(m_centralWidget);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 4, 0, 1, 3);

        m_pRssUrlEdit = new QLineEdit(m_centralWidget);
        m_pRssUrlEdit->setObjectName(QString::fromUtf8("m_pRssUrlEdit"));
        m_pRssUrlEdit->setEnabled(false);

        gridLayout->addWidget(m_pRssUrlEdit, 0, 1, 1, 1);

        m_pRssAutoDownloadGroupBox = new QGroupBox(m_centralWidget);
        m_pRssAutoDownloadGroupBox->setObjectName(QString::fromUtf8("m_pRssAutoDownloadGroupBox"));
        m_pRssAutoDownloadGroupBox->setCheckable(true);
        m_pRssAutoDownloadGroupBox->setChecked(false);
        gridLayout_2 = new QGridLayout(m_pRssAutoDownloadGroupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        m_pUseGroupsCheckBox = new QCheckBox(m_pRssAutoDownloadGroupBox);
        m_pUseGroupsCheckBox->setObjectName(QString::fromUtf8("m_pUseGroupsCheckBox"));

        gridLayout_2->addWidget(m_pUseGroupsCheckBox, 1, 0, 1, 2);

        m_pUseSavePathGroupBox = new QGroupBox(m_pRssAutoDownloadGroupBox);
        m_pUseSavePathGroupBox->setObjectName(QString::fromUtf8("m_pUseSavePathGroupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(m_pUseSavePathGroupBox->sizePolicy().hasHeightForWidth());
        m_pUseSavePathGroupBox->setSizePolicy(sizePolicy);
        m_pUseSavePathGroupBox->setCheckable(true);
        m_pUseSavePathGroupBox->setChecked(false);
        gridLayout_3 = new QGridLayout(m_pUseSavePathGroupBox);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_2 = new QLabel(m_pUseSavePathGroupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_3->addWidget(label_2, 0, 0, 1, 1);

        m_pSavePathEdit = new QLineEdit(m_pUseSavePathGroupBox);
        m_pSavePathEdit->setObjectName(QString::fromUtf8("m_pSavePathEdit"));

        gridLayout_3->addWidget(m_pSavePathEdit, 0, 1, 1, 1);

        m_pBrowseButton = new QPushButton(m_pUseSavePathGroupBox);
        m_pBrowseButton->setObjectName(QString::fromUtf8("m_pBrowseButton"));

        gridLayout_3->addWidget(m_pBrowseButton, 0, 2, 1, 1);


        gridLayout_2->addWidget(m_pUseSavePathGroupBox, 0, 0, 1, 2);


        gridLayout->addWidget(m_pRssAutoDownloadGroupBox, 1, 0, 1, 2);

        label = new QLabel(m_centralWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_3 = new QLabel(m_centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        m_pRefreshRateSpinBox = new QSpinBox(m_centralWidget);
        m_pRefreshRateSpinBox->setObjectName(QString::fromUtf8("m_pRefreshRateSpinBox"));
        m_pRefreshRateSpinBox->setMaximum(300);

        gridLayout->addWidget(m_pRefreshRateSpinBox, 2, 1, 1, 1);


        verticalLayout->addWidget(m_centralWidget);


        retranslateUi(RssSettings);

        QMetaObject::connectSlotsByName(RssSettings);
    } // setupUi

    void retranslateUi(QWidget *RssSettings)
    {
        RssSettings->setWindowTitle(QApplication::translate("RssSettings", "RSS_FEED_SETTINGS", 0, QApplication::UnicodeUTF8));
        tbMenu->setText(QString());
        LTitle->setText(QApplication::translate("RssSettings", "RSS_SETTINGS", 0, QApplication::UnicodeUTF8));
        m_pRssAutoDownloadGroupBox->setTitle(QApplication::translate("RssSettings", "RSS_AUTO_DOWNLOAD", 0, QApplication::UnicodeUTF8));
        m_pUseGroupsCheckBox->setText(QApplication::translate("RssSettings", "USE_GROUPS_FOR_FILTERING", 0, QApplication::UnicodeUTF8));
        m_pUseSavePathGroupBox->setTitle(QApplication::translate("RssSettings", "USE_SAVE_PATH", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("RssSettings", "SAVE_PATH", 0, QApplication::UnicodeUTF8));
        m_pBrowseButton->setText(QApplication::translate("RssSettings", "BROWSE", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("RssSettings", "RSS_URL", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("RssSettings", "RSS_REFRASH_RATE", 0, QApplication::UnicodeUTF8));
        m_pRefreshRateSpinBox->setSuffix(QApplication::translate("RssSettings", " MINUTES", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RssSettings: public Ui_RssSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RSSFEEDSETTINGSDIALOG_H
