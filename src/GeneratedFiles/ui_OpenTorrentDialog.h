/********************************************************************************
** Form generated from reading UI file 'OpenTorrentDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPENTORRENTDIALOG_H
#define UI_OPENTORRENTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OpenTorrentDialog
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *titleBar;
    QHBoxLayout *horizontalLayout;
    QLabel *tbMenu;
    QLabel *LTitle;
    QPushButton *pbClose;
    QWidget *m_centralWidget;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLineEdit *pathEdit;
    QLabel *label;
    QPushButton *browseButton;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_4;
    QLabel *labelNameTitile;
    QLineEdit *labelNameData;
    QLabel *labelCommentTitle;
    QLabel *labelComentData;
    QLabel *labelSizeData;
    QLabel *labelSizeTitle;
    QLabel *loaderGifLabel;
    QLabel *loaderTextLabel;
    QGridLayout *gridLayout_2;
    QCheckBox *setPausedCheckBox;
    QCheckBox *setSuperSeedCheckBox;
    QCheckBox *setSeedModeCheckBox;
    QCheckBox *setSequntialCheckBox;
    QCheckBox *skipNameInPathCheckBox;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *markAllButton;
    QPushButton *unmarkAllButton;
    QTreeView *torrentFilesTreeView;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *yesButton;

    void setupUi(QDialog *OpenTorrentDialog)
    {
        if (OpenTorrentDialog->objectName().isEmpty())
            OpenTorrentDialog->setObjectName(QString::fromUtf8("OpenTorrentDialog"));
        OpenTorrentDialog->resize(429, 535);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/app.ico"), QSize(), QIcon::Normal, QIcon::Off);
        OpenTorrentDialog->setWindowIcon(icon);
        OpenTorrentDialog->setStyleSheet(QString::fromUtf8(""));
        verticalLayout = new QVBoxLayout(OpenTorrentDialog);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(1, 1, 1, 0);
        titleBar = new QWidget(OpenTorrentDialog);
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

        m_centralWidget = new QWidget(OpenTorrentDialog);
        m_centralWidget->setObjectName(QString::fromUtf8("m_centralWidget"));
        m_centralWidget->setStyleSheet(QString::fromUtf8(""));
        gridLayout_3 = new QGridLayout(m_centralWidget);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        groupBox = new QGroupBox(m_centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        pathEdit = new QLineEdit(groupBox);
        pathEdit->setObjectName(QString::fromUtf8("pathEdit"));

        gridLayout->addWidget(pathEdit, 0, 0, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 1, 1, 1);

        browseButton = new QPushButton(groupBox);
        browseButton->setObjectName(QString::fromUtf8("browseButton"));

        gridLayout->addWidget(browseButton, 0, 1, 1, 1);


        gridLayout_3->addWidget(groupBox, 0, 0, 1, 6);

        groupBox_2 = new QGroupBox(m_centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_4 = new QGridLayout(groupBox_2);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        labelNameTitile = new QLabel(groupBox_2);
        labelNameTitile->setObjectName(QString::fromUtf8("labelNameTitile"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(labelNameTitile->sizePolicy().hasHeightForWidth());
        labelNameTitile->setSizePolicy(sizePolicy);

        gridLayout_4->addWidget(labelNameTitile, 0, 0, 1, 1);

        labelNameData = new QLineEdit(groupBox_2);
        labelNameData->setObjectName(QString::fromUtf8("labelNameData"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(labelNameData->sizePolicy().hasHeightForWidth());
        labelNameData->setSizePolicy(sizePolicy1);

        gridLayout_4->addWidget(labelNameData, 0, 1, 1, 3);

        labelCommentTitle = new QLabel(groupBox_2);
        labelCommentTitle->setObjectName(QString::fromUtf8("labelCommentTitle"));
        sizePolicy.setHeightForWidth(labelCommentTitle->sizePolicy().hasHeightForWidth());
        labelCommentTitle->setSizePolicy(sizePolicy);

        gridLayout_4->addWidget(labelCommentTitle, 1, 0, 2, 1);

        labelComentData = new QLabel(groupBox_2);
        labelComentData->setObjectName(QString::fromUtf8("labelComentData"));
        sizePolicy1.setHeightForWidth(labelComentData->sizePolicy().hasHeightForWidth());
        labelComentData->setSizePolicy(sizePolicy1);
        labelComentData->setMaximumSize(QSize(16777215, 13));
        labelComentData->setScaledContents(true);
        labelComentData->setWordWrap(true);

        gridLayout_4->addWidget(labelComentData, 1, 1, 1, 1);

        labelSizeData = new QLabel(groupBox_2);
        labelSizeData->setObjectName(QString::fromUtf8("labelSizeData"));
        sizePolicy1.setHeightForWidth(labelSizeData->sizePolicy().hasHeightForWidth());
        labelSizeData->setSizePolicy(sizePolicy1);

        gridLayout_4->addWidget(labelSizeData, 2, 1, 2, 1);

        labelSizeTitle = new QLabel(groupBox_2);
        labelSizeTitle->setObjectName(QString::fromUtf8("labelSizeTitle"));
        sizePolicy.setHeightForWidth(labelSizeTitle->sizePolicy().hasHeightForWidth());
        labelSizeTitle->setSizePolicy(sizePolicy);

        gridLayout_4->addWidget(labelSizeTitle, 3, 0, 1, 1);

        loaderGifLabel = new QLabel(groupBox_2);
        loaderGifLabel->setObjectName(QString::fromUtf8("loaderGifLabel"));

        gridLayout_4->addWidget(loaderGifLabel, 3, 2, 1, 1);

        loaderTextLabel = new QLabel(groupBox_2);
        loaderTextLabel->setObjectName(QString::fromUtf8("loaderTextLabel"));

        gridLayout_4->addWidget(loaderTextLabel, 3, 3, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        setPausedCheckBox = new QCheckBox(groupBox_2);
        setPausedCheckBox->setObjectName(QString::fromUtf8("setPausedCheckBox"));

        gridLayout_2->addWidget(setPausedCheckBox, 2, 0, 1, 1);

        setSuperSeedCheckBox = new QCheckBox(groupBox_2);
        setSuperSeedCheckBox->setObjectName(QString::fromUtf8("setSuperSeedCheckBox"));

        gridLayout_2->addWidget(setSuperSeedCheckBox, 2, 1, 1, 1);

        setSeedModeCheckBox = new QCheckBox(groupBox_2);
        setSeedModeCheckBox->setObjectName(QString::fromUtf8("setSeedModeCheckBox"));

        gridLayout_2->addWidget(setSeedModeCheckBox, 1, 1, 1, 1);

        setSequntialCheckBox = new QCheckBox(groupBox_2);
        setSequntialCheckBox->setObjectName(QString::fromUtf8("setSequntialCheckBox"));

        gridLayout_2->addWidget(setSequntialCheckBox, 1, 0, 1, 1);

        skipNameInPathCheckBox = new QCheckBox(groupBox_2);
        skipNameInPathCheckBox->setObjectName(QString::fromUtf8("skipNameInPathCheckBox"));

        gridLayout_2->addWidget(skipNameInPathCheckBox, 0, 0, 1, 2);


        gridLayout_4->addLayout(gridLayout_2, 4, 0, 1, 4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        markAllButton = new QPushButton(groupBox_2);
        markAllButton->setObjectName(QString::fromUtf8("markAllButton"));

        horizontalLayout_2->addWidget(markAllButton);

        unmarkAllButton = new QPushButton(groupBox_2);
        unmarkAllButton->setObjectName(QString::fromUtf8("unmarkAllButton"));

        horizontalLayout_2->addWidget(unmarkAllButton);


        gridLayout_4->addLayout(horizontalLayout_2, 5, 0, 1, 4);

        torrentFilesTreeView = new QTreeView(groupBox_2);
        torrentFilesTreeView->setObjectName(QString::fromUtf8("torrentFilesTreeView"));
        torrentFilesTreeView->setStyleSheet(QString::fromUtf8(""));
        torrentFilesTreeView->setSortingEnabled(true);

        gridLayout_4->addWidget(torrentFilesTreeView, 6, 0, 1, 4);


        gridLayout_3->addWidget(groupBox_2, 1, 0, 1, 6);

        horizontalSpacer = new QSpacerItem(266, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 2, 3, 1, 1);

        cancelButton = new QPushButton(m_centralWidget);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(cancelButton->sizePolicy().hasHeightForWidth());
        cancelButton->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(cancelButton, 2, 5, 1, 1);

        yesButton = new QPushButton(m_centralWidget);
        yesButton->setObjectName(QString::fromUtf8("yesButton"));
        sizePolicy2.setHeightForWidth(yesButton->sizePolicy().hasHeightForWidth());
        yesButton->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(yesButton, 2, 4, 1, 1);


        verticalLayout->addWidget(m_centralWidget);


        retranslateUi(OpenTorrentDialog);
        QObject::connect(browseButton, SIGNAL(clicked()), OpenTorrentDialog, SLOT(BrowseButton()));
        QObject::connect(cancelButton, SIGNAL(clicked()), OpenTorrentDialog, SLOT(reject()));
        QObject::connect(yesButton, SIGNAL(clicked()), OpenTorrentDialog, SLOT(accept()));
        QObject::connect(pathEdit, SIGNAL(textChanged(QString)), OpenTorrentDialog, SLOT(OnPathChanged(QString)));
        QObject::connect(markAllButton, SIGNAL(clicked()), OpenTorrentDialog, SLOT(OnCheckAll()));
        QObject::connect(unmarkAllButton, SIGNAL(clicked()), OpenTorrentDialog, SLOT(OnUncheckAll()));

        QMetaObject::connectSlotsByName(OpenTorrentDialog);
    } // setupUi

    void retranslateUi(QDialog *OpenTorrentDialog)
    {
        OpenTorrentDialog->setWindowTitle(QApplication::translate("OpenTorrentDialog", "DIALOG_OPEN_TORRENT", 0));
#ifndef QT_NO_STATUSTIP
        OpenTorrentDialog->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        tbMenu->setText(QString());
        LTitle->setText(QApplication::translate("OpenTorrentDialog", "DIALOG_OPEN_TORRENT", 0));
        groupBox->setTitle(QApplication::translate("OpenTorrentDialog", "OPEN_TORRENT_SAVE_PATH", 0));
        label->setText(QApplication::translate("OpenTorrentDialog", "DISCRIBTION_LABEL", 0));
        browseButton->setText(QApplication::translate("OpenTorrentDialog", "OPEN_TORRENT_BROWSE_BUTTON", 0));
        groupBox_2->setTitle(QApplication::translate("OpenTorrentDialog", "OPEN_TORRENT_INFO", 0));
        labelNameTitile->setText(QApplication::translate("OpenTorrentDialog", "OPEN_TORRENT_INFO_NAME", 0));
        labelNameData->setText(QString());
        labelCommentTitle->setText(QApplication::translate("OpenTorrentDialog", "OPEN_TORRENT_INFO_DESCRIBTION", 0));
        labelComentData->setText(QString());
        labelSizeData->setText(QString());
        labelSizeTitle->setText(QApplication::translate("OpenTorrentDialog", "SIZE", 0));
        loaderGifLabel->setText(QString());
        loaderTextLabel->setText(QApplication::translate("OpenTorrentDialog", "RECIVING_METADATA", 0));
#ifndef QT_NO_TOOLTIP
        setPausedCheckBox->setToolTip(QApplication::translate("OpenTorrentDialog", "START_PAUSED_TOOLTIP", 0));
#endif // QT_NO_TOOLTIP
        setPausedCheckBox->setText(QApplication::translate("OpenTorrentDialog", "START_PAUSED", 0));
#ifndef QT_NO_TOOLTIP
        setSuperSeedCheckBox->setToolTip(QApplication::translate("OpenTorrentDialog", "START_WITH_SUPER_SEED", 0));
#endif // QT_NO_TOOLTIP
        setSuperSeedCheckBox->setText(QApplication::translate("OpenTorrentDialog", "START_WITH_SUPER_SEED", 0));
#ifndef QT_NO_TOOLTIP
        setSeedModeCheckBox->setToolTip(QApplication::translate("OpenTorrentDialog", "START_SEEDING_TOOLTIP", 0));
#endif // QT_NO_TOOLTIP
        setSeedModeCheckBox->setText(QApplication::translate("OpenTorrentDialog", "START_SEEDING", 0));
#ifndef QT_NO_TOOLTIP
        setSequntialCheckBox->setToolTip(QApplication::translate("OpenTorrentDialog", "START_SEQUENTIAL_TOOLTIP", 0));
#endif // QT_NO_TOOLTIP
        setSequntialCheckBox->setText(QApplication::translate("OpenTorrentDialog", "SET_SEQUNTIAL", 0));
        skipNameInPathCheckBox->setText(QApplication::translate("OpenTorrentDialog", "DO_NOT_CREATE_SUBFOLDER_WITH_TORRENT_NAME", 0));
        markAllButton->setText(QApplication::translate("OpenTorrentDialog", "MARK_ALL", 0));
        unmarkAllButton->setText(QApplication::translate("OpenTorrentDialog", "UNMARK_ALL", 0));
        cancelButton->setText(QApplication::translate("OpenTorrentDialog", "OPEN_TORRENT_CANCEL_BUTTON", 0));
        yesButton->setText(QApplication::translate("OpenTorrentDialog", "OPEN_TORRENT_ADD_BUTTON", 0));
    } // retranslateUi

};

namespace Ui {
    class OpenTorrentDialog: public Ui_OpenTorrentDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPENTORRENTDIALOG_H
