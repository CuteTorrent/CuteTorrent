/********************************************************************************
** Form generated from reading UI file 'multipleDt.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MULTIPLEDT_H
#define UI_MULTIPLEDT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DTMountDialog
{
public:
	QVBoxLayout* verticalLayout;
	QWidget* titleBar;
	QHBoxLayout* horizontalLayout;
	QLabel* tbMenu;
	QLabel* LTitle;
	QPushButton* pbClose;
	QWidget* m_centralWidget;
	QGridLayout* gridLayout;
	QListView* listView;
	QVBoxLayout* vboxLayout;
	QPushButton* okButton;
	QPushButton* cancelButton;
	QSpacerItem* spacerItem;

	void setupUi(QWidget* DTMountDialog)
	{
		if (DTMountDialog->objectName().isEmpty())
		{
			DTMountDialog->setObjectName(QString::fromUtf8("DTMountDialog"));
		}

		DTMountDialog->resize(357, 196);
		QIcon icon;
		icon.addFile(QString::fromUtf8(":/icons/app.ico"), QSize(), QIcon::Normal, QIcon::Off);
		DTMountDialog->setWindowIcon(icon);
		verticalLayout = new QVBoxLayout(DTMountDialog);
		verticalLayout->setSpacing(0);
		verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
		verticalLayout->setContentsMargins(1, 1, 1, 0);
		titleBar = new QWidget(DTMountDialog);
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
		m_centralWidget = new QWidget(DTMountDialog);
		m_centralWidget->setObjectName(QString::fromUtf8("m_centralWidget"));
		gridLayout = new QGridLayout(m_centralWidget);
		gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
		listView = new QListView(m_centralWidget);
		listView->setObjectName(QString::fromUtf8("listView"));
		listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
		gridLayout->addWidget(listView, 0, 0, 1, 1);
		vboxLayout = new QVBoxLayout();
#ifndef Q_OS_MAC
		vboxLayout->setSpacing(6);
#endif
		vboxLayout->setContentsMargins(0, 0, 0, 0);
		vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
		okButton = new QPushButton(m_centralWidget);
		okButton->setObjectName(QString::fromUtf8("okButton"));
		vboxLayout->addWidget(okButton);
		cancelButton = new QPushButton(m_centralWidget);
		cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
		vboxLayout->addWidget(cancelButton);
		spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
		vboxLayout->addItem(spacerItem);
		gridLayout->addLayout(vboxLayout, 0, 1, 1, 1);
		verticalLayout->addWidget(m_centralWidget);
		retranslateUi(DTMountDialog);
		QObject::connect(cancelButton, SIGNAL(clicked()), DTMountDialog, SLOT(reject()));
		QMetaObject::connectSlotsByName(DTMountDialog);
	} // setupUi

	void retranslateUi(QWidget* DTMountDialog)
	{
		DTMountDialog->setWindowTitle(QApplication::translate("DTMountDialog", "DT_MOUNT_DILAOG", 0, QApplication::UnicodeUTF8));
		tbMenu->setText(QString());
		LTitle->setText(QApplication::translate("DTMountDialog", "DT_MOUNT_DILAOG", 0, QApplication::UnicodeUTF8));
		okButton->setText(QApplication::translate("DTMountDialog", "OK", 0, QApplication::UnicodeUTF8));
		cancelButton->setText(QApplication::translate("DTMountDialog", "Cancel", 0, QApplication::UnicodeUTF8));
	} // retranslateUi

};

namespace Ui
{
class DTMountDialog: public Ui_DTMountDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MULTIPLEDT_H
