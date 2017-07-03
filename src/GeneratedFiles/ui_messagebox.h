/********************************************************************************
** Form generated from reading UI file 'messagebox.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MESSAGEBOX_H
#define UI_MESSAGEBOX_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CustomMessageBox
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
    QLabel *icon;
    QLabel *text;

    void setupUi(QDialog *CustomMessageBox)
    {
        if (CustomMessageBox->objectName().isEmpty())
            CustomMessageBox->setObjectName(QStringLiteral("CustomMessageBox"));
        CustomMessageBox->setWindowModality(Qt::WindowModal);
        CustomMessageBox->resize(352, 120);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CustomMessageBox->sizePolicy().hasHeightForWidth());
        CustomMessageBox->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(CustomMessageBox);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(1, 1, 1, 1);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        titleBar = new QWidget(CustomMessageBox);
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

        horizontalLayout->addWidget(LTitle);

        pbClose = new QPushButton(titleBar);
        pbClose->setObjectName(QStringLiteral("pbClose"));
        pbClose->setMinimumSize(QSize(22, 22));
        pbClose->setMaximumSize(QSize(22, 22));
        pbClose->setIconSize(QSize(22, 22));
        pbClose->setFlat(true);

        horizontalLayout->addWidget(pbClose);


        verticalLayout->addWidget(titleBar);

        m_centralWidget = new QWidget(CustomMessageBox);
        m_centralWidget->setObjectName(QStringLiteral("m_centralWidget"));
        m_centralWidget->setStyleSheet(QStringLiteral(""));
        gridLayout = new QGridLayout(m_centralWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetNoConstraint);
        buttonBox = new QDialogButtonBox(m_centralWidget);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setCenterButtons(true);

        gridLayout->addWidget(buttonBox, 1, 0, 1, 2);

        icon = new QLabel(m_centralWidget);
        icon->setObjectName(QStringLiteral("icon"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(icon->sizePolicy().hasHeightForWidth());
        icon->setSizePolicy(sizePolicy1);
        icon->setMinimumSize(QSize(48, 48));
        icon->setMaximumSize(QSize(48, 48));
        icon->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(icon, 0, 0, 1, 1);

        text = new QLabel(m_centralWidget);
        text->setObjectName(QStringLiteral("text"));
        text->setWordWrap(true);

        gridLayout->addWidget(text, 0, 1, 1, 1);

        gridLayout->setRowStretch(1, 100);
        gridLayout->setRowMinimumHeight(0, 2);
        gridLayout->setRowMinimumHeight(1, 6);

        verticalLayout->addWidget(m_centralWidget);

        QWidget::setTabOrder(buttonBox, pbClose);

        retranslateUi(CustomMessageBox);
        QObject::connect(buttonBox, SIGNAL(accepted()), CustomMessageBox, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CustomMessageBox, SLOT(reject()));

        QMetaObject::connectSlotsByName(CustomMessageBox);
    } // setupUi

    void retranslateUi(QDialog *CustomMessageBox)
    {
        tbMenu->setText(QString());
        text->setText(QString());
        Q_UNUSED(CustomMessageBox);
    } // retranslateUi

};

namespace Ui {
    class CustomMessageBox: public Ui_CustomMessageBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MESSAGEBOX_H
