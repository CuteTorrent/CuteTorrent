#include "finishpage.h"
FinishPage::FinishPage(QWidget* parent) :
	QWizardPage(parent)
{
	setTitle(tr("BAKUP_FINISH_TITLE"));
	layout = new QVBoxLayout;
	messageLabel = new QLabel(tr("BACKUP_FINISH_MESSAGE"));
	messageLabel->setWordWrap(true);
	layout->addWidget(messageLabel);
	setLayout(layout);
}

int FinishPage::nextId() const
{
	return -1;
}


