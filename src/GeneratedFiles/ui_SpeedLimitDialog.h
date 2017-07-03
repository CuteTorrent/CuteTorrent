/********************************************************************************
** Form generated from reading UI file 'SpeedLimitDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPEEDLIMITDIALOG_H
#define UI_SPEEDLIMITDIALOG_H

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
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SpeedLimitDialog
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
    QSpinBox *spinBox;
    QLabel *label;
    QSlider *horizontalSlider;

    void setupUi(QWidget *SpeedLimitDialog)
    {
        if (SpeedLimitDialog->objectName().isEmpty())
            SpeedLimitDialog->setObjectName(QStringLiteral("SpeedLimitDialog"));
        SpeedLimitDialog->resize(274, 119);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/app.ico"), QSize(), QIcon::Normal, QIcon::Off);
        SpeedLimitDialog->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(SpeedLimitDialog);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(1, 1, 1, 0);
        titleBar = new QWidget(SpeedLimitDialog);
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
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(8);
        font.setBold(false);
        font.setWeight(50);
        font.setStyleStrategy(QFont::PreferDefault);
        LTitle->setFont(font);
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

        m_centralWidget = new QWidget(SpeedLimitDialog);
        m_centralWidget->setObjectName(QStringLiteral("m_centralWidget"));
        gridLayout = new QGridLayout(m_centralWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        buttonBox = new QDialogButtonBox(m_centralWidget);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 2, 1, 1, 1);

        spinBox = new QSpinBox(m_centralWidget);
        spinBox->setObjectName(QStringLiteral("spinBox"));

        gridLayout->addWidget(spinBox, 0, 1, 1, 1);

        label = new QLabel(m_centralWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        horizontalSlider = new QSlider(m_centralWidget);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setMaximum(500000);
        horizontalSlider->setSingleStep(100);
        horizontalSlider->setPageStep(1000);
        horizontalSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSlider, 1, 0, 1, 2);


        verticalLayout->addWidget(m_centralWidget);


        retranslateUi(SpeedLimitDialog);
        QObject::connect(spinBox, SIGNAL(valueChanged(int)), SpeedLimitDialog, SLOT(OnSpeedChanged(int)));
        QObject::connect(horizontalSlider, SIGNAL(valueChanged(int)), SpeedLimitDialog, SLOT(OnSpeedChanged(int)));

        QMetaObject::connectSlotsByName(SpeedLimitDialog);
    } // setupUi

    void retranslateUi(QWidget *SpeedLimitDialog)
    {
        SpeedLimitDialog->setWindowTitle(QApplication::translate("SpeedLimitDialog", "SPEED_LIMIT_DIALOG", Q_NULLPTR));
        tbMenu->setText(QString());
        LTitle->setText(QApplication::translate("SpeedLimitDialog", "SPEED_LIMIT_DIALOG", Q_NULLPTR));
        label->setText(QApplication::translate("SpeedLimitDialog", "LIMIT", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class SpeedLimitDialog: public Ui_SpeedLimitDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPEEDLIMITDIALOG_H
