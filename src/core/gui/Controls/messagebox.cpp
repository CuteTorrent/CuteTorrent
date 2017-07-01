#include <QDebug>
#include "messagebox.h"
#ifdef Q_OS_WIN 
#include <winsock2.h>
#include <windows.h>
#endif

CustomMessageBox::CustomMessageBox(QMessageBox::Icon icon, const QString& title, const QString& text,
                                   Buttons buttons, QWidget* parent, CustomButtonInfoList customButtonsText) :
	BaseWindow<QDialog>(OnlyCloseButton, NoResize, parent),
	customButtonsInfo(customButtonsText),
	m_icon(icon),
	ui(new Ui::CustomMessageBox)
{
	ui->setupUi(this);
	setupCustomWindow();
	setupWindowIcons();
	ui->icon->setPixmap(standardIcon(icon));
	ui->LTitle->setText(title);
	ui->text->setText(text);

	if (parent != NULL)
	{
#ifdef Q_OS_WIN 
		SetWindowPos((HWND)effectiveWinId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
		SetWindowPos((HWND)effectiveWinId(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
#endif
		// HACK END
		parent->showNormal();
		parent->setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
		parent->raise();
		parent->activateWindow();
	}

	if (buttons == NoButton)
	{
		buttons = Ok;
		buttons |= Cancel;
	}

	clickedButton = NULL;
	uint mask = FirstButton;
	bool isFirst = true;

	while (mask <= LastButton)
	{
		uint sb = buttons & mask;
		mask <<= 1;

		if (!sb)
		{
			continue;
		}

		QPushButton* button = CreateButton(sb, customButtonsText);

		if (isFirst)
		{
			isFirst = false;
			button->setFocus();
		}

		QMessageBox::ButtonRole role = static_cast<QMessageBox::ButtonRole>(ui->buttonBox->buttonRole(button));

		if (role == QMessageBox::RejectRole || role == QMessageBox::NoRole)
		{
			cancelButton = static_cast<Button>(ui->buttonBox->standardButton(button));
		}
	}

	setWindowModality(Qt::ApplicationModal);
	connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
}

void CustomMessageBox::showEvent(QShowEvent* pEvent)
{
#ifndef QT_NO_ACCESSIBILITY

	if (m_icon == QMessageBox::Critical || m_icon == QMessageBox::Warning)
	{
		QAccessible::updateAccessibility(new QAccessibleEvent(this, QAccessible::Alert));
	}

#endif
	QDialog::showEvent(pEvent);
}

CustomMessageBox::CustomMessageBox(QWidget* /*parent*/) :
	BaseWindow<QDialog>(OnlyCloseButton, NoResize),
	ui(new Ui::CustomMessageBox)
{
	ui->setupUi(this);
}

QPushButton* CustomMessageBox::CreateButton(uint sb, CustomButtonInfoList customButtonsText) const
{
	if (sb <= QDialogButtonBox::LastButton)
	{
		return ui->buttonBox->addButton(static_cast<QDialogButtonBox::StandardButton>(sb));
	}
	else
	{
		int buttonInfoIndex = (log(sb) - log(uint(CustomButton1))) / log(2);
		if (buttonInfoIndex < 0 || buttonInfoIndex > customButtonsText.size())
		{
			qDebug() << "unable to create button for mask " << sb;
			return nullptr;
		}
		CustomButonInfo info = customButtonsText[buttonInfoIndex];
		return ui->buttonBox->addButton(info.text, info.role);
	}
}

QPixmap CustomMessageBox::standardIcon(QMessageBox::Icon icon) const
{
	QStyle* style = this->style();
	int iconSize = style->pixelMetric(QStyle::PM_MessageBoxIconSize, NULL, this);
	QIcon tmpIcon;

	switch (icon)
	{
		case QMessageBox::Information:
			tmpIcon = style->standardIcon(QStyle::SP_MessageBoxInformation, NULL, this);
			break;

		case QMessageBox::Warning:
			tmpIcon = style->standardIcon(QStyle::SP_MessageBoxWarning, NULL, this);
			break;

		case QMessageBox::Critical:
			tmpIcon = style->standardIcon(QStyle::SP_MessageBoxCritical, NULL, this);
			break;

		case QMessageBox::Question:
			tmpIcon = style->standardIcon(QStyle::SP_MessageBoxQuestion, NULL, this);
			break;

		case QMessageBox::NoIcon:
			return ui->tbMenu->pixmap()->scaled(iconSize, iconSize);

		default:
			break;
	}

	if (!tmpIcon.isNull())
	{
		return tmpIcon.pixmap(iconSize);
	}

	return QPixmap();
}

CustomMessageBox::Button CustomMessageBox::_clickedButton() const
{
	if (clickedButton != NULL)
	{
		Button button = static_cast<Button>(ui->buttonBox->standardButton(clickedButton));
		if (button == NoButton)
		{
			QString text = clickedButton->text();
			for (int i = 0; i < customButtonsInfo.size(); i++)
			{
				const CustomButonInfo customButonInfo = customButtonsInfo[i];
				if (customButonInfo.text == text && ui->buttonBox->buttonRole(clickedButton) == customButonInfo.role)
				{
					return static_cast<Button>(CustomButton1 << i);
				}
			}
		}
		return button;
	}
	else
	{
		return cancelButton;
	}
}

CustomMessageBox::~CustomMessageBox()
{
	delete ui;
}

CustomMessageBox::Button CustomMessageBox::critical(QWidget* parent, const QString& title, const QString& text, Buttons buttons, CustomButtonInfoList customButtonsText)
{
	return showNewMessageBox(parent, QMessageBox::Critical, title, text, buttons, customButtonsText);
}

CustomMessageBox::Button CustomMessageBox::information(QWidget* parent, const QString& title, const QString& text, Buttons buttons, CustomButtonInfoList customButtonsText)
{
	return showNewMessageBox(parent, QMessageBox::Information, title, text, buttons, customButtonsText);
}

CustomMessageBox::Button CustomMessageBox::question(QWidget* parent, const QString& title, const QString& text, Buttons buttons, CustomButtonInfoList customButtonsText)
{
	return showNewMessageBox(parent, QMessageBox::Question, title, text, buttons, customButtonsText);
}

CustomMessageBox::Button CustomMessageBox::warning(QWidget* parent, const QString& title, const QString& text, Buttons buttons, CustomButtonInfoList customButtonsText)
{
	return showNewMessageBox(parent, QMessageBox::Warning, title, text, buttons, customButtonsText);
}

CustomMessageBox::Button CustomMessageBox::critical(const QString& title, const QString& text, Buttons buttons, CustomButtonInfoList customButtonsText)
{
	return showNewMessageBox(NULL, QMessageBox::Critical, title, text, buttons, customButtonsText);
}

CustomMessageBox::Button CustomMessageBox::information(const QString& title, const QString& text, Buttons buttons, CustomButtonInfoList customButtonsText)
{
	return showNewMessageBox(NULL, QMessageBox::Information, title, text, buttons, customButtonsText);
}

CustomMessageBox::Button CustomMessageBox::question(const QString& title, const QString& text, Buttons buttons, CustomButtonInfoList customButtonsText)
{
	return showNewMessageBox(NULL, QMessageBox::Question, title, text, buttons, customButtonsText);
}

CustomMessageBox::Button CustomMessageBox::warning(const QString& title, const QString& text, Buttons buttons, CustomButtonInfoList customButtonsText)
{
	return showNewMessageBox(NULL, QMessageBox::Warning, title, text, buttons, customButtonsText);
}

void CustomMessageBox::about(const QString& title, const QString& text)
{
	showNewMessageBox(NULL, QMessageBox::NoIcon, title, text, Ok, CustomButtonInfoList());
}

void CustomMessageBox::about(QWidget* parent, const QString& title, const QString& text)
{
	showNewMessageBox(parent, QMessageBox::NoIcon, title, text, Ok, CustomButtonInfoList());
}

CustomMessageBox::Button CustomMessageBox::showNewMessageBox(QWidget* parent, QMessageBox::Icon icon,
                                                             const QString& title, const QString& text,
                                                             Buttons buttons, CustomButtonInfoList customButtonsText)
{
	CustomMessageBox msgBox(icon, title, text, buttons, parent, customButtonsText);
	msgBox.exec();
	return msgBox._clickedButton();
}

void CustomMessageBox::buttonClicked(QAbstractButton* button)
{
	clickedButton = button;
}

void CustomMessageBox::accept()
{
	isClosed = true;
	done(Accepted);
}

void CustomMessageBox::reject()
{
	isClosed = true;
	done(Rejected);
}

QWidget* CustomMessageBox::getTitleBar()
{
	return ui->titleBar;
}

QLabel* CustomMessageBox::getTitleLabel()
{
	return ui->LTitle;
}

QLabel* CustomMessageBox::getTitleIcon()
{
	return ui->tbMenu;
}

QPushButton* CustomMessageBox::getCloseBtn()
{
	return ui->pbClose;
}

QWidget* CustomMessageBox::centralWidget()
{
	return ui->m_centralWidget;
}

