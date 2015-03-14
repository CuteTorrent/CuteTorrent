#include <QDebug>
#include <QPointer>

#include "messagebox.h"
#include "ui_messagebox.h"

MyMessageBox::MyMessageBox(QWidget* parent) :
	BaseWindow<QDialog> (OnlyCloseButton, NoResize),
	ui(new Ui::MessageBox)
{
	ui->setupUi(this);
}

MyMessageBox::MyMessageBox(QMessageBox::Icon icon, const QString& title, const QString& text,
                           QMessageBox::StandardButtons buttons, QWidget* parent, Qt::WindowFlags flags) :
	BaseWindow<QDialog> (OnlyCloseButton, NoResize),
	ui(new Ui::MessageBox)
{
	ui->setupUi(this);
	setupCustomWindow();
	setupWindowIcons();

	if(icon != QMessageBox::NoIcon)
	{
		ui->icon->setPixmap(standardIcon(icon));
	}
	else
	{
		ui->icon->setPixmap(* (ui->tbMenu->pixmap()));
	}

	ui->LTitle->setText(title);
	ui->text->setText(text);

	if(buttons == QMessageBox::NoButton)
	{
		buttons = QMessageBox::Ok | QMessageBox::Cancel;
	}

	clickedButton = NULL;
	uint mask = QMessageBox::FirstButton;

	while(mask <= QMessageBox::LastButton)
	{
		uint sb = buttons & mask;
		mask <<= 1;

		if(!sb)
		{
			continue;
		}

		QPushButton* button = ui->buttonBox->addButton((QDialogButtonBox::StandardButton) sb);
		QMessageBox::ButtonRole  role = (QMessageBox::ButtonRole) ui->buttonBox->buttonRole(button);

		if(role == QMessageBox::RejectRole || role == QMessageBox::NoRole)
		{
			cancelButton = (QMessageBox::StandardButton) ui->buttonBox->standardButton(button);
		}
	}

	setWindowModality(Qt::ApplicationModal);
	connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(buttonClicked(QAbstractButton*)));
}


QPixmap MyMessageBox::standardIcon(QMessageBox::Icon icon)
{
	QStyle* style =  QApplication::style();
	int iconSize = style->pixelMetric(QStyle::PM_MessageBoxIconSize);
	QIcon tmpIcon;

	switch(icon)
	{
		case QMessageBox::Information:
			tmpIcon = style->standardIcon(QStyle::SP_MessageBoxInformation);
			break;

		case QMessageBox::Warning:
			tmpIcon = style->standardIcon(QStyle::SP_MessageBoxWarning);
			break;

		case QMessageBox::Critical:
			tmpIcon = style->standardIcon(QStyle::SP_MessageBoxCritical);
			break;

		case QMessageBox::Question:
			tmpIcon = style->standardIcon(QStyle::SP_MessageBoxQuestion);

		default:
			break;
	}

	if(!tmpIcon.isNull())
	{
		return tmpIcon.pixmap(iconSize, iconSize);
	}

	return QPixmap();
}

QMessageBox::StandardButton MyMessageBox::_clickedButton()
{
	return  clickedButton != NULL ? (QMessageBox::StandardButton) ui->buttonBox->standardButton(clickedButton) : cancelButton;
}

MyMessageBox::~MyMessageBox()
{
	delete ui;
}

QMessageBox::StandardButton MyMessageBox::critical(QWidget* parent, const QString& title, const QString& text, QMessageBox::StandardButtons buttons)
{
	return showNewMessageBox(parent, QMessageBox::Critical, title, text, buttons);
}

QMessageBox::StandardButton MyMessageBox::information(QWidget* parent, const QString& title, const QString& text, QMessageBox::StandardButtons buttons)
{
	return showNewMessageBox(parent, QMessageBox::Information, title, text, buttons);
}

QMessageBox::StandardButton MyMessageBox::question(QWidget* parent, const QString& title, const QString& text, QMessageBox::StandardButtons buttons)
{
	return showNewMessageBox(parent, QMessageBox::Question, title, text, buttons);
}

QMessageBox::StandardButton MyMessageBox::warning(QWidget* parent, const QString& title, const QString& text, QMessageBox::StandardButtons buttons)
{
	return showNewMessageBox(parent, QMessageBox::Warning, title, text, buttons);
}

void MyMessageBox::about(QWidget* parent, const QString& title, const QString& text)
{
	showNewMessageBox(parent, QMessageBox::NoIcon, title, text, QMessageBox::Ok);
}

QMessageBox::StandardButton MyMessageBox::showNewMessageBox(QWidget* parent, QMessageBox::Icon icon,
        const QString& title, const QString& text,
        QMessageBox::StandardButtons buttons)
{
	MyMessageBox msgBox = MyMessageBox(icon, title, text, buttons, parent);
	msgBox.showDialog();
	return msgBox._clickedButton();
}

void MyMessageBox::showDialog()
{
	isClosed = false;
	show();

	while(!isClosed)
	{
		QApplication::processEvents(QEventLoop::AllEvents, 1000);
	}
}

void MyMessageBox::buttonClicked(QAbstractButton* button)
{
	clickedButton = button;
}

void MyMessageBox::accept()
{
	isClosed = true;
	done(QDialog::Accepted);
}

void MyMessageBox::reject()
{
	isClosed = true;
	done(QDialog::Rejected);
}

QWidget* MyMessageBox::getTitleBar()
{
	return ui->titleBar;
}

QLabel* MyMessageBox::getTitleLabel()
{
	return ui->LTitle;
}

QLabel* MyMessageBox::getTitleIcon()
{
	return ui->tbMenu;
}

QPushButton* MyMessageBox::getCloseBtn()
{
	return ui->pbClose;
}

QWidget* MyMessageBox::centralWidget()
{
	return ui->m_centralWidget;
}
