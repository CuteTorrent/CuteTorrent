/********************************************************************************
** Form generated from reading UI file 'ReportProblemDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REPORTPROBLEMDIALOG_H
#define UI_REPORTPROBLEMDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ReportProblemDialog
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
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *sendReportButton;
    QPushButton *cancelButton;
    QLabel *label;
    QLineEdit *nameEdit;
    QLabel *label_3;
    QLabel *label_2;
    QLineEdit *emailEdit;
    QTextEdit *problemDescriptionEdit;
    QCheckBox *sendOSInfoCheckBox;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *ReportProblemDialog)
    {
        if (ReportProblemDialog->objectName().isEmpty())
            ReportProblemDialog->setObjectName(QString::fromUtf8("ReportProblemDialog"));
        ReportProblemDialog->resize(389, 302);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/app.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ReportProblemDialog->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(ReportProblemDialog);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(1, 1, 1, 0);
        titleBar = new QWidget(ReportProblemDialog);
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

        m_centralWidget = new QWidget(ReportProblemDialog);
        m_centralWidget->setObjectName(QString::fromUtf8("m_centralWidget"));
        gridLayout = new QGridLayout(m_centralWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        sendReportButton = new QPushButton(m_centralWidget);
        sendReportButton->setObjectName(QString::fromUtf8("sendReportButton"));

        horizontalLayout_2->addWidget(sendReportButton);

        cancelButton = new QPushButton(m_centralWidget);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        horizontalLayout_2->addWidget(cancelButton);


        gridLayout->addLayout(horizontalLayout_2, 6, 0, 1, 2);

        label = new QLabel(m_centralWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        nameEdit = new QLineEdit(m_centralWidget);
        nameEdit->setObjectName(QString::fromUtf8("nameEdit"));

        gridLayout->addWidget(nameEdit, 0, 1, 1, 1);

        label_3 = new QLabel(m_centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setWordWrap(true);

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        label_2 = new QLabel(m_centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        emailEdit = new QLineEdit(m_centralWidget);
        emailEdit->setObjectName(QString::fromUtf8("emailEdit"));

        gridLayout->addWidget(emailEdit, 1, 1, 1, 1);

        problemDescriptionEdit = new QTextEdit(m_centralWidget);
        problemDescriptionEdit->setObjectName(QString::fromUtf8("problemDescriptionEdit"));

        gridLayout->addWidget(problemDescriptionEdit, 2, 1, 2, 1);

        sendOSInfoCheckBox = new QCheckBox(m_centralWidget);
        sendOSInfoCheckBox->setObjectName(QString::fromUtf8("sendOSInfoCheckBox"));

        gridLayout->addWidget(sendOSInfoCheckBox, 5, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 3, 0, 1, 1);


        verticalLayout->addWidget(m_centralWidget);


        retranslateUi(ReportProblemDialog);
        QObject::connect(cancelButton, SIGNAL(clicked()), ReportProblemDialog, SLOT(close()));
        QObject::connect(sendReportButton, SIGNAL(clicked()), ReportProblemDialog, SLOT(SendReport()));

        QMetaObject::connectSlotsByName(ReportProblemDialog);
    } // setupUi

    void retranslateUi(QWidget *ReportProblemDialog)
    {
        ReportProblemDialog->setWindowTitle(QApplication::translate("ReportProblemDialog", "DT_MOUNT_DILAOG", 0, QApplication::UnicodeUTF8));
        tbMenu->setText(QString());
        LTitle->setText(QApplication::translate("ReportProblemDialog", "REPORT_PROBLEM", 0, QApplication::UnicodeUTF8));
        sendReportButton->setText(QApplication::translate("ReportProblemDialog", "SEND_REPORT", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("ReportProblemDialog", "CANCEL", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ReportProblemDialog", "NAME", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ReportProblemDialog", "PROBLEM DESCRIBTION", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ReportProblemDialog", "EMAIL", 0, QApplication::UnicodeUTF8));
        sendOSInfoCheckBox->setText(QApplication::translate("ReportProblemDialog", "SEND_OS_INFORMATION", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ReportProblemDialog: public Ui_ReportProblemDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REPORTPROBLEMDIALOG_H
