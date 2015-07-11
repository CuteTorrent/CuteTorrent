/********************************************************************************
** Form generated from reading UI file 'messagebox.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MESSAGEBOX_H
#define UI_MESSAGEBOX_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CustomMessageBox
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
	QDialogButtonBox* buttonBox;
	QLabel* icon;
	QLabel* text;

	void setupUi(QDialog* CustomMessageBox)
	{
		if (CustomMessageBox->objectName().isEmpty())
		{
			CustomMessageBox->setObjectName(QString::fromUtf8("CustomMessageBox"));
		}

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
		verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
		titleBar = new QWidget(CustomMessageBox);
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
		m_centralWidget = new QWidget(CustomMessageBox);
		m_centralWidget->setObjectName(QString::fromUtf8("m_centralWidget"));
		m_centralWidget->setStyleSheet(QString::fromUtf8(""));
		gridLayout = new QGridLayout(m_centralWidget);
		gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
		gridLayout->setSizeConstraint(QLayout::SetNoConstraint);
		buttonBox = new QDialogButtonBox(m_centralWidget);
		buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
		buttonBox->setCenterButtons(true);
		gridLayout->addWidget(buttonBox, 1, 0, 1, 2);
		icon = new QLabel(m_centralWidget);
		icon->setObjectName(QString::fromUtf8("icon"));
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
		text->setObjectName(QString::fromUtf8("text"));
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

	void retranslateUi(QDialog* CustomMessageBox)
	{
		CustomMessageBox->setWindowTitle(QApplication::translate("CustomMessageBox", "Dialog", 0, QApplication::UnicodeUTF8));
		tbMenu->setText(QString());
		LTitle->setText(QApplication::translate("CustomMessageBox", "MB_TITLE", 0, QApplication::UnicodeUTF8));
		text->setText(QString());
	} // retranslateUi

};

namespace Ui
{
class CustomMessageBox: public Ui_CustomMessageBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MESSAGEBOX_H
