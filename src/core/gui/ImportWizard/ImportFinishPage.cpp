#include "ImportFinishPage.h"
#include "ImportWizard.h"
#include <QtGui>

void ImportFinishPage::initializePage()
{
	ImportWizard::ImportResult res = static_cast<ImportWizard::ImportResult>(field("ImportResult").value<int>());
	errorDisplay->setVisible(false);
	switch (res)
	{
		case ImportWizard::Success:
		{
			resultLabel->setText(tr("IMPORT_SUCCES"));
			break;
		}
		case ImportWizard::Canceled:
		{
			resultLabel->setText(tr("IMPORT_CANCELED_BY_USER"));
			break;
		}

		case ImportWizard::Failed:
		{
			QString error = field("ImportErrors").toString();
			resultLabel->setText(tr("IMPORT_ERROR"));
			errorDisplay->setVisible(true);
			errorDisplay->setPlainText(error);
			break;
		}
	}
}

ImportFinishPage::ImportFinishPage(QWidget* parent) : QWizardPage(parent)
{
	setFinalPage(true);
	gridLayout = new QGridLayout(this);
	gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
	resultLabel = new QLabel(this);
	resultLabel->setObjectName(QString::fromUtf8("resultLabel"));

	gridLayout->addWidget(resultLabel, 0, 0, 1, 1);

	errorDisplay = new QPlainTextEdit(this);
	errorDisplay->setObjectName(QString::fromUtf8("errorDisplay"));

	gridLayout->addWidget(errorDisplay, 1, 0, 1, 1);

	setLayout(gridLayout);
}

