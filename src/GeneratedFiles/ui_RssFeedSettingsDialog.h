/********************************************************************************
** Form generated from reading UI file 'RssFeedSettingsDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RSSFEEDSETTINGSDIALOG_H
#define UI_RSSFEEDSETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTableWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RssSettings
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
    QLabel *label_3;
    QDialogButtonBox *buttonBox;
    QLineEdit *m_pRssUrlEdit;
    QSpinBox *m_pRefreshRateSpinBox;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QPushButton *removeRowButton;
    QPushButton *addRowButton;
    QTableWidget *m_pCoociesTabWidget;
    QLabel *label_2;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *RssSettings)
    {
        if (RssSettings->objectName().isEmpty())
            RssSettings->setObjectName(QString::fromUtf8("RssSettings"));
        RssSettings->resize(382, 246);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/app.ico"), QSize(), QIcon::Normal, QIcon::Off);
        RssSettings->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(RssSettings);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(1, 1, 1, 0);
        titleBar = new QWidget(RssSettings);
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

        m_centralWidget = new QWidget(RssSettings);
        m_centralWidget->setObjectName(QString::fromUtf8("m_centralWidget"));
        gridLayout = new QGridLayout(m_centralWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_3 = new QLabel(m_centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        buttonBox = new QDialogButtonBox(m_centralWidget);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 4, 0, 1, 3);

        m_pRssUrlEdit = new QLineEdit(m_centralWidget);
        m_pRssUrlEdit->setObjectName(QString::fromUtf8("m_pRssUrlEdit"));
        m_pRssUrlEdit->setReadOnly(true);

        gridLayout->addWidget(m_pRssUrlEdit, 0, 1, 1, 2);

        m_pRefreshRateSpinBox = new QSpinBox(m_centralWidget);
        m_pRefreshRateSpinBox->setObjectName(QString::fromUtf8("m_pRefreshRateSpinBox"));
        m_pRefreshRateSpinBox->setMaximum(300);

        gridLayout->addWidget(m_pRefreshRateSpinBox, 1, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 2, 1, 1);

        label = new QLabel(m_centralWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        groupBox = new QGroupBox(m_centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        removeRowButton = new QPushButton(groupBox);
        removeRowButton->setObjectName(QString::fromUtf8("removeRowButton"));

        gridLayout_2->addWidget(removeRowButton, 1, 1, 1, 1);

        addRowButton = new QPushButton(groupBox);
        addRowButton->setObjectName(QString::fromUtf8("addRowButton"));

        gridLayout_2->addWidget(addRowButton, 0, 1, 1, 1);

        m_pCoociesTabWidget = new QTableWidget(groupBox);
        if (m_pCoociesTabWidget->columnCount() < 2)
            m_pCoociesTabWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        m_pCoociesTabWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        m_pCoociesTabWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        if (m_pCoociesTabWidget->rowCount() < 1)
            m_pCoociesTabWidget->setRowCount(1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        m_pCoociesTabWidget->setVerticalHeaderItem(0, __qtablewidgetitem2);
        m_pCoociesTabWidget->setObjectName(QString::fromUtf8("m_pCoociesTabWidget"));
        m_pCoociesTabWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        m_pCoociesTabWidget->setColumnCount(2);
        m_pCoociesTabWidget->verticalHeader()->setVisible(false);
        m_pCoociesTabWidget->verticalHeader()->setDefaultSectionSize(19);

        gridLayout_2->addWidget(m_pCoociesTabWidget, 0, 0, 3, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setWordWrap(true);

        gridLayout_2->addWidget(label_2, 3, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 2, 1, 2, 1);


        gridLayout->addWidget(groupBox, 2, 0, 1, 3);


        verticalLayout->addWidget(m_centralWidget);

        QWidget::setTabOrder(m_pRssUrlEdit, m_pRefreshRateSpinBox);
        QWidget::setTabOrder(m_pRefreshRateSpinBox, pbClose);
        QWidget::setTabOrder(pbClose, buttonBox);

        retranslateUi(RssSettings);
        QObject::connect(buttonBox, SIGNAL(accepted()), RssSettings, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), RssSettings, SLOT(reject()));
        QObject::connect(addRowButton, SIGNAL(clicked()), RssSettings, SLOT(onAddRow()));
        QObject::connect(removeRowButton, SIGNAL(clicked()), RssSettings, SLOT(onRemoveRow()));

        QMetaObject::connectSlotsByName(RssSettings);
    } // setupUi

    void retranslateUi(QDialog *RssSettings)
    {
        RssSettings->setWindowTitle(QApplication::translate("RssSettings", "RSS_FEED_SETTINGS", 0, QApplication::UnicodeUTF8));
        tbMenu->setText(QString());
        LTitle->setText(QApplication::translate("RssSettings", "RSS_SETTINGS", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("RssSettings", "RSS_REFRASH_RATE", 0, QApplication::UnicodeUTF8));
        m_pRefreshRateSpinBox->setSuffix(QApplication::translate("RssSettings", " MINUTES", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("RssSettings", "RSS_URL", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("RssSettings", "COOKIES", 0, QApplication::UnicodeUTF8));
        removeRowButton->setText(QApplication::translate("RssSettings", "REMOVE_ROW", 0, QApplication::UnicodeUTF8));
        addRowButton->setText(QApplication::translate("RssSettings", "ADD_ROW", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = m_pCoociesTabWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("RssSettings", "COOKIE_ATTRIBUTE", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = m_pCoociesTabWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("RssSettings", "COOLIE_VALUE", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = m_pCoociesTabWidget->verticalHeaderItem(0);
        ___qtablewidgetitem2->setText(QApplication::translate("RssSettings", "\320\235\320\276\320\262\320\260\321\217 \321\201\321\202\321\200\320\276\320\272\320\260", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("RssSettings", "COOKIE_HINT", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RssSettings: public Ui_RssSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RSSFEEDSETTINGSDIALOG_H
