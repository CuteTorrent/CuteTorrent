#include "RegisterDialog.h"
#include <gui/Controls/messagebox.h>
#include <CommentsWebClient.h>
#include "User.h"
#include <QApplicationSettings.h>

RegisterDialog::RegisterDialog(QWidget* parent): BaseWindow<QDialog>(FullTitle, ResizeMode::NoResize, parent)
{
	setupUi(this);
	setupCustomWindow();
	setupWindowIcons();
	buttonBox->button(QDialogButtonBox::Ok)->setText(tr("REGISTER_BTN"));
}

void RegisterDialog::OnAccepted()
{
	if (loginEdit->text().isEmpty())
	{
		CustomMessageBox::critical("INPUT_NOT_VALID", "LOGIN_REQUIRED");
		return;
	}

	if (passwordEdit->text().isEmpty())
	{
		CustomMessageBox::critical("INPUT_NOT_VALID", "PASSWORD_REQUIRED");
		return;
	}

	if (mailEdit->text().isEmpty())
	{
		CustomMessageBox::critical("INPUT_NOT_VALID", "PASSWORD_REQUIRED");
		return;
	}

	boost::shared_ptr<CommentsWebClient> commentsWebClient = CommentsWebClient::getInstance();
	User user = {loginEdit->text(), passwordEdit->text(), mailEdit->text()};
	if (commentsWebClient->Register(user))
	{
		QApplicationSettingsPtr settings = QApplicationSettings::getInstance();
		settings->setSecuredValue("Comments", "login", loginEdit->text());
		settings->setSecuredValue("Comments", "password", passwordEdit->text());
		done(Accepted);
	}
}


QPushButton* RegisterDialog::getCloseBtn()
{
	return pbClose;
}

QWidget* RegisterDialog::getTitleBar()
{
	return titleBar;
}

QWidget* RegisterDialog::centralWidget()
{
	return m_centralWidget;
}

QLabel* RegisterDialog::getTitleLabel()
{
	return LTitle;
}

QLabel* RegisterDialog::getTitleIcon()
{
	return tbMenu;
}

