/********************************************************************************
** Form generated from reading UI file 'InitializtionDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INITIALIZTIONDIALOG_H
#define UI_INITIALIZTIONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InitializationDialog
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
    QProgressBar *progressBar;
    QLabel *stateLabel;

    void setupUi(QWidget *InitializationDialog)
    {
        if (InitializationDialog->objectName().isEmpty())
            InitializationDialog->setObjectName(QString::fromUtf8("InitializationDialog"));
        InitializationDialog->resize(349, 102);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/app.ico"), QSize(), QIcon::Normal, QIcon::Off);
        InitializationDialog->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(InitializationDialog);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(1, 1, 1, 0);
        titleBar = new QWidget(InitializationDialog);
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

        m_centralWidget = new QWidget(InitializationDialog);
        m_centralWidget->setObjectName(QString::fromUtf8("m_centralWidget"));
        gridLayout = new QGridLayout(m_centralWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        progressBar = new QProgressBar(m_centralWidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(0);
        progressBar->setTextVisible(false);

        gridLayout->addWidget(progressBar, 2, 0, 1, 1);

        stateLabel = new QLabel(m_centralWidget);
        stateLabel->setObjectName(QString::fromUtf8("stateLabel"));
        stateLabel->setTextFormat(Qt::PlainText);
        stateLabel->setWordWrap(true);

        gridLayout->addWidget(stateLabel, 1, 0, 1, 1);


        verticalLayout->addWidget(m_centralWidget);


        retranslateUi(InitializationDialog);

        QMetaObject::connectSlotsByName(InitializationDialog);
    } // setupUi

    void retranslateUi(QWidget *InitializationDialog)
    {
        tbMenu->setText(QString());
        LTitle->setText(QApplication::translate("InitializationDialog", "INITIALIZATION_DIALOG", 0));
        stateLabel->setText(QString());
        Q_UNUSED(InitializationDialog);
    } // retranslateUi

};

namespace Ui {
    class InitializationDialog: public Ui_InitializationDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INITIALIZTIONDIALOG_H
