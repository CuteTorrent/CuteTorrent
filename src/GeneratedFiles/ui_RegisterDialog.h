/********************************************************************************
** Form generated from reading UI file 'RegisterDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERDIALOG_H
#define UI_REGISTERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RegisterDialog
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
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *mailEdit;
    QLineEdit *passwordEdit;
    QLineEdit *loginEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QWidget *RegisterDialog)
    {
        if (RegisterDialog->objectName().isEmpty())
            RegisterDialog->setObjectName(QString::fromUtf8("RegisterDialog"));
        RegisterDialog->resize(295, 143);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/app.ico"), QSize(), QIcon::Normal, QIcon::Off);
        RegisterDialog->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(RegisterDialog);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(1, 1, 1, 0);
        titleBar = new QWidget(RegisterDialog);
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

        m_centralWidget = new QWidget(RegisterDialog);
        m_centralWidget->setObjectName(QString::fromUtf8("m_centralWidget"));
        rootContentLayout = new QGridLayout(m_centralWidget);
        rootContentLayout->setObjectName(QString::fromUtf8("rootContentLayout"));
        label = new QLabel(m_centralWidget);
        label->setObjectName(QString::fromUtf8("label"));

        rootContentLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(m_centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        rootContentLayout->addWidget(label_2, 1, 0, 1, 1);

        label_3 = new QLabel(m_centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        rootContentLayout->addWidget(label_3, 2, 0, 1, 1);

        mailEdit = new QLineEdit(m_centralWidget);
        mailEdit->setObjectName(QString::fromUtf8("mailEdit"));
        mailEdit->setFrame(true);

        rootContentLayout->addWidget(mailEdit, 2, 1, 1, 1);

        passwordEdit = new QLineEdit(m_centralWidget);
        passwordEdit->setObjectName(QString::fromUtf8("passwordEdit"));
        passwordEdit->setEchoMode(QLineEdit::Password);

        rootContentLayout->addWidget(passwordEdit, 1, 1, 1, 1);

        loginEdit = new QLineEdit(m_centralWidget);
        loginEdit->setObjectName(QString::fromUtf8("loginEdit"));

        rootContentLayout->addWidget(loginEdit, 0, 1, 1, 1);

        buttonBox = new QDialogButtonBox(m_centralWidget);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);

        rootContentLayout->addWidget(buttonBox, 3, 0, 1, 2);


        verticalLayout->addWidget(m_centralWidget);


        retranslateUi(RegisterDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), RegisterDialog, SLOT(OnAccepted()));
        QObject::connect(buttonBox, SIGNAL(rejected()), RegisterDialog, SLOT(close()));

        QMetaObject::connectSlotsByName(RegisterDialog);
    } // setupUi

    void retranslateUi(QWidget *RegisterDialog)
    {
        RegisterDialog->setWindowTitle(QApplication::translate("RegisterDialog", "REGISTER_DLG", 0));
        tbMenu->setText(QString());
        LTitle->setText(QApplication::translate("RegisterDialog", "REGISTER_DLG", 0));
        label->setText(QApplication::translate("RegisterDialog", "LOGIN", 0));
        label_2->setText(QApplication::translate("RegisterDialog", "PASSWORD", 0));
        label_3->setText(QApplication::translate("RegisterDialog", "EMAIL", 0));
        mailEdit->setPlaceholderText(QApplication::translate("RegisterDialog", "ENTER MAIL", 0));
        passwordEdit->setPlaceholderText(QApplication::translate("RegisterDialog", "ENTER PASSWORD", 0));
        loginEdit->setPlaceholderText(QApplication::translate("RegisterDialog", "ENTER LOGIN", 0));
    } // retranslateUi

};

namespace Ui {
    class RegisterDialog: public Ui_RegisterDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERDIALOG_H
