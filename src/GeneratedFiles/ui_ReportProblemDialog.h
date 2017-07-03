/********************************************************************************
** Form generated from reading UI file 'ReportProblemDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REPORTPROBLEMDIALOG_H
#define UI_REPORTPROBLEMDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

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
            ReportProblemDialog->setObjectName(QStringLiteral("ReportProblemDialog"));
        ReportProblemDialog->resize(389, 302);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/app.ico"), QSize(), QIcon::Normal, QIcon::Off);
        ReportProblemDialog->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(ReportProblemDialog);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(1, 1, 1, 0);
        titleBar = new QWidget(ReportProblemDialog);
        titleBar->setObjectName(QStringLiteral("titleBar"));
        titleBar->setMinimumSize(QSize(0, 23));
        titleBar->setMaximumSize(QSize(16777215, 23));
        titleBar->setStyleSheet(QStringLiteral(""));
        horizontalLayout = new QHBoxLayout(titleBar);
        horizontalLayout->setSpacing(4);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(5, 0, 1, 0);
        tbMenu = new QLabel(titleBar);
        tbMenu->setObjectName(QStringLiteral("tbMenu"));
        tbMenu->setMaximumSize(QSize(15, 15));
        tbMenu->setPixmap(QPixmap(QString::fromUtf8(":/icons/app.ico")));
        tbMenu->setScaledContents(true);

        horizontalLayout->addWidget(tbMenu);

        LTitle = new QLabel(titleBar);
        LTitle->setObjectName(QStringLiteral("LTitle"));
        LTitle->setMinimumSize(QSize(100, 22));
        LTitle->setMaximumSize(QSize(16777215, 22));
        LTitle->setStyleSheet(QStringLiteral(""));
        LTitle->setTextFormat(Qt::AutoText);

        horizontalLayout->addWidget(LTitle);

        pbClose = new QPushButton(titleBar);
        pbClose->setObjectName(QStringLiteral("pbClose"));
        pbClose->setMinimumSize(QSize(22, 22));
        pbClose->setMaximumSize(QSize(22, 22));
        pbClose->setIconSize(QSize(22, 22));
        pbClose->setFlat(true);

        horizontalLayout->addWidget(pbClose);


        verticalLayout->addWidget(titleBar);

        m_centralWidget = new QWidget(ReportProblemDialog);
        m_centralWidget->setObjectName(QStringLiteral("m_centralWidget"));
        gridLayout = new QGridLayout(m_centralWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        sendReportButton = new QPushButton(m_centralWidget);
        sendReportButton->setObjectName(QStringLiteral("sendReportButton"));

        horizontalLayout_2->addWidget(sendReportButton);

        cancelButton = new QPushButton(m_centralWidget);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));

        horizontalLayout_2->addWidget(cancelButton);


        gridLayout->addLayout(horizontalLayout_2, 6, 0, 1, 2);

        label = new QLabel(m_centralWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        nameEdit = new QLineEdit(m_centralWidget);
        nameEdit->setObjectName(QStringLiteral("nameEdit"));

        gridLayout->addWidget(nameEdit, 0, 1, 1, 1);

        label_3 = new QLabel(m_centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setWordWrap(true);

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        label_2 = new QLabel(m_centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        emailEdit = new QLineEdit(m_centralWidget);
        emailEdit->setObjectName(QStringLiteral("emailEdit"));

        gridLayout->addWidget(emailEdit, 1, 1, 1, 1);

        problemDescriptionEdit = new QTextEdit(m_centralWidget);
        problemDescriptionEdit->setObjectName(QStringLiteral("problemDescriptionEdit"));

        gridLayout->addWidget(problemDescriptionEdit, 2, 1, 2, 1);

        sendOSInfoCheckBox = new QCheckBox(m_centralWidget);
        sendOSInfoCheckBox->setObjectName(QStringLiteral("sendOSInfoCheckBox"));

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
        ReportProblemDialog->setWindowTitle(QApplication::translate("ReportProblemDialog", "DT_MOUNT_DILAOG", Q_NULLPTR));
        tbMenu->setText(QString());
        LTitle->setText(QApplication::translate("ReportProblemDialog", "REPORT_PROBLEM", Q_NULLPTR));
        sendReportButton->setText(QApplication::translate("ReportProblemDialog", "SEND_REPORT", Q_NULLPTR));
        cancelButton->setText(QApplication::translate("ReportProblemDialog", "CANCEL", Q_NULLPTR));
        label->setText(QApplication::translate("ReportProblemDialog", "NAME", Q_NULLPTR));
        label_3->setText(QApplication::translate("ReportProblemDialog", "PROBLEM DESCRIBTION", Q_NULLPTR));
        label_2->setText(QApplication::translate("ReportProblemDialog", "EMAIL", Q_NULLPTR));
        sendOSInfoCheckBox->setText(QApplication::translate("ReportProblemDialog", "SEND_OS_INFORMATION", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ReportProblemDialog: public Ui_ReportProblemDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REPORTPROBLEMDIALOG_H
