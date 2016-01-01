/********************************************************************************
** Form generated from reading UI file 'VideoFileChoose.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIDEOFILECHOOSE_H
#define UI_VIDEOFILECHOOSE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListView>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VideoFileChooseDialog
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *titleBar;
    QHBoxLayout *horizontalLayout;
    QLabel *tbMenu;
    QLabel *LTitle;
    QPushButton *pbClose;
    QWidget *m_centralWidget;
    QGridLayout *mainLayout;
    QDialogButtonBox *buttonBox;
    QListView *listView;

    void setupUi(QWidget *VideoFileChooseDialog)
    {
        if (VideoFileChooseDialog->objectName().isEmpty())
            VideoFileChooseDialog->setObjectName(QString::fromUtf8("VideoFileChooseDialog"));
        VideoFileChooseDialog->resize(426, 253);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/app.ico"), QSize(), QIcon::Normal, QIcon::Off);
        VideoFileChooseDialog->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(VideoFileChooseDialog);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(1, 1, 1, 0);
        titleBar = new QWidget(VideoFileChooseDialog);
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

        m_centralWidget = new QWidget(VideoFileChooseDialog);
        m_centralWidget->setObjectName(QString::fromUtf8("m_centralWidget"));
        mainLayout = new QGridLayout(m_centralWidget);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        buttonBox = new QDialogButtonBox(m_centralWidget);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        mainLayout->addWidget(buttonBox, 1, 0, 1, 2);

        listView = new QListView(m_centralWidget);
        listView->setObjectName(QString::fromUtf8("listView"));

        mainLayout->addWidget(listView, 0, 0, 1, 1);

        mainLayout->setRowStretch(0, 2);

        verticalLayout->addWidget(m_centralWidget);


        retranslateUi(VideoFileChooseDialog);

        QMetaObject::connectSlotsByName(VideoFileChooseDialog);
    } // setupUi

    void retranslateUi(QWidget *VideoFileChooseDialog)
    {
        VideoFileChooseDialog->setWindowTitle(QApplication::translate("VideoFileChooseDialog", "CHOOSE_VIDEO_FILE", 0, QApplication::UnicodeUTF8));
        tbMenu->setText(QString());
        LTitle->setText(QApplication::translate("VideoFileChooseDialog", "CHOOSE_VIDEO_FILE", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class VideoFileChooseDialog: public Ui_VideoFileChooseDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIDEOFILECHOOSE_H
