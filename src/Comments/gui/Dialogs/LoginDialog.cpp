#include "LoginDialog.h"
#include <CommentsWebClient.h>
#include <QApplicationSettings.h>

LoginDialog::LoginDialog(QWidget* parent) : BaseWindow<QDialog>(FullTitle, ResizeMode::NoResize, parent)
{
	setupUi(this);
	setupCustomWindow();
	setupWindowIcons();
	buttonBox->addButton(tr("LOGIN_BTN"), QDialogButtonBox::AcceptRole);
	QApplicationSettingsPtr settings = QApplicationSettings::getInstance();
	QString login = settings->securedValueString("Comments", "login", loginEdit->text());
	loginEdit->setText(login);
	QString password = settings->securedValueString("Comments", "password", passwordEdit->text());
	passwordEdit->setText(password);
	saveCredentialsCheckBox->setChecked(!login.isEmpty() && !password.isEmpty());
}

void LoginDialog::OnAccepted()
{
	boost::shared_ptr<CommentsWebClient> commentsWebClient = CommentsWebClient::getInstance();
	if (commentsWebClient->Login(loginEdit->text(), passwordEdit->text()))
	{
		QApplicationSettingsPtr settings = QApplicationSettings::getInstance();
		if (saveCredentialsCheckBox->isChecked())
		{
			settings->setSecuredValue("Comments", "login", loginEdit->text());
			settings->setSecuredValue("Comments", "password", passwordEdit->text());
		}
		else
		{
			settings->setSecuredValue("Comments", "login", "");
			settings->setSecuredValue("Comments", "password", "");
		}
		done(Accepted);
	}
}

QPushButton* LoginDialog::getCloseBtn()
{
	return pbClose;
}

QWidget* LoginDialog::getTitleBar()
{
	return titleBar;
}

QWidget* LoginDialog::centralWidget()
{
	return m_centralWidget;
}

QLabel* LoginDialog::getTitleLabel()
{
	return LTitle;
}

QLabel* LoginDialog::getTitleIcon()
{
	return tbMenu;
}

