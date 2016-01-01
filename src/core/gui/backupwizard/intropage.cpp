#include "intropage.h"
#include "backupwizard.h"
#include <QDebug>

IntroPage::IntroPage(QWidget* parent)
	: QWizardPage(parent)
{
	setTitle(tr("BAKUP_INTRO_TITLE"));
	QLabel* label = new QLabel(tr("BACKUP_INTRO_MESSAGE"));
	label->setWordWrap(true);
	careateRadioButton = new QRadioButton(tr("CREATE_BAKUP"));
	applyRadioButton  = new QRadioButton(tr("APPLY_BAKUP"));
	careateRadioButton->setChecked(true);
	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(label);

	layout->addWidget(careateRadioButton);
	layout->addWidget(applyRadioButton);
	setLayout(layout);
}

int IntroPage::nextId() const
{
	if(careateRadioButton->isChecked())
	{
		return BackupWizard::Page_CreateBakup;
	}
	else
	{
		return BackupWizard::Page_ApplyBakup;
	}
}

bool IntroPage::validatePage()
{
	return careateRadioButton->isChecked() || applyRadioButton->isChecked();
}

