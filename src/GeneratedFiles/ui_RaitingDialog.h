/********************************************************************************
** Form generated from reading UI file 'RaitingDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RAITINGDIALOG_H
#define UI_RAITINGDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RaitingDialog
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
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label;
    QLabel *label_5;
    QLabel *label_4;
    QCheckBox *checkBox;
    QPlainTextEdit *plainTextEdit;

    void setupUi(QWidget *RaitingDialog)
    {
        if (RaitingDialog->objectName().isEmpty())
            RaitingDialog->setObjectName(QString::fromUtf8("RaitingDialog"));
        RaitingDialog->resize(377, 440);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/app.ico"), QSize(), QIcon::Normal, QIcon::Off);
        RaitingDialog->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(RaitingDialog);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(1, 1, 1, 0);
        titleBar = new QWidget(RaitingDialog);
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

        m_centralWidget = new QWidget(RaitingDialog);
        m_centralWidget->setObjectName(QString::fromUtf8("m_centralWidget"));
        mainLayout = new QGridLayout(m_centralWidget);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        buttonBox = new QDialogButtonBox(m_centralWidget);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        mainLayout->addWidget(buttonBox, 7, 0, 1, 2);

        label_2 = new QLabel(m_centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        mainLayout->addWidget(label_2, 1, 0, 1, 1);

        label_3 = new QLabel(m_centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        mainLayout->addWidget(label_3, 2, 0, 1, 1);

        label = new QLabel(m_centralWidget);
        label->setObjectName(QString::fromUtf8("label"));

        mainLayout->addWidget(label, 0, 0, 1, 1);

        label_5 = new QLabel(m_centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        mainLayout->addWidget(label_5, 4, 0, 1, 1);

        label_4 = new QLabel(m_centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        mainLayout->addWidget(label_4, 3, 0, 1, 1);

        checkBox = new QCheckBox(m_centralWidget);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));

        mainLayout->addWidget(checkBox, 6, 1, 1, 1);

        plainTextEdit = new QPlainTextEdit(m_centralWidget);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));

        mainLayout->addWidget(plainTextEdit, 5, 0, 1, 2);

        mainLayout->setRowStretch(0, 2);
        mainLayout->setRowStretch(1, 2);
        mainLayout->setRowStretch(2, 2);
        mainLayout->setRowStretch(3, 2);

        verticalLayout->addWidget(m_centralWidget);


        retranslateUi(RaitingDialog);

        QMetaObject::connectSlotsByName(RaitingDialog);
    } // setupUi

    void retranslateUi(QWidget *RaitingDialog)
    {
        RaitingDialog->setWindowTitle(QApplication::translate("RaitingDialog", "RAITE_CLIENT", 0, QApplication::UnicodeUTF8));
        tbMenu->setText(QString());
        LTitle->setText(QApplication::translate("RaitingDialog", "RAITE_CLIENT", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("RaitingDialog", "DESIGN", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("RaitingDialog", "USABLILITY", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("RaitingDialog", "SPEED", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("RaitingDialog", "YOUR_OPINION", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("RaitingDialog", "POSIBILITIES", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("RaitingDialog", "DO_NOT_DISTURB_ME", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RaitingDialog: public Ui_RaitingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RAITINGDIALOG_H
