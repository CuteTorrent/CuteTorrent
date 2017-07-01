/********************************************************************************
** Form generated from reading UI file 'AddCommentDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDCOMMENTDIALOG_H
#define UI_ADDCOMMENTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QLayout>

QT_BEGIN_NAMESPACE

class Ui_AddCommentDialog
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *titleBar;
    QHBoxLayout *horizontalLayout;
    QLabel *tbMenu;
    QLabel *LTitle;
    QPushButton *pbClose;
    QWidget *m_centralWidget;
    QGridLayout *rootContentLayout;
    QWidget *widget_2;
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QWidget *widget;

    void setupUi(QWidget *AddCommentDialog)
    {
        if (AddCommentDialog->objectName().isEmpty())
            AddCommentDialog->setObjectName(QString::fromUtf8("AddCommentDialog"));
        AddCommentDialog->resize(545, 348);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/app.ico"), QSize(), QIcon::Normal, QIcon::Off);
        AddCommentDialog->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(AddCommentDialog);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(1, 1, 1, 0);
        titleBar = new QWidget(AddCommentDialog);
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

        m_centralWidget = new QWidget(AddCommentDialog);
        m_centralWidget->setObjectName(QString::fromUtf8("m_centralWidget"));
        rootContentLayout = new QGridLayout(m_centralWidget);
        rootContentLayout->setObjectName(QString::fromUtf8("rootContentLayout"));
        widget_2 = new QWidget(m_centralWidget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));

        rootContentLayout->addWidget(widget_2, 1, 1, 1, 1);

        buttonBox = new QDialogButtonBox(m_centralWidget);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        rootContentLayout->addWidget(buttonBox, 2, 1, 1, 1);

        label = new QLabel(m_centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMaximumSize(QSize(16777215, 35));

        rootContentLayout->addWidget(label, 1, 0, 1, 1);

        widget = new QWidget(m_centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));

        rootContentLayout->addWidget(widget, 0, 0, 1, 2);


        verticalLayout->addWidget(m_centralWidget);


        retranslateUi(AddCommentDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), AddCommentDialog, SLOT(OnAccepted()));
        QObject::connect(buttonBox, SIGNAL(rejected()), AddCommentDialog, SLOT(close()));

        QMetaObject::connectSlotsByName(AddCommentDialog);
    } // setupUi

    void retranslateUi(QWidget *AddCommentDialog)
    {
        AddCommentDialog->setWindowTitle(QApplication::translate("AddCommentDialog", "ADD_COMMENT_DIALOG", 0));
        tbMenu->setText(QString());
        LTitle->setText(QApplication::translate("AddCommentDialog", "ADD_COMMENT_DIALOG", 0));
        label->setText(QApplication::translate("AddCommentDialog", "RATING:", 0));
    } // retranslateUi

};

namespace Ui {
    class AddCommentDialog: public Ui_AddCommentDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDCOMMENTDIALOG_H
