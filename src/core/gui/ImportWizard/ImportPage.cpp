#include "ImportPage.h"
#include <QtGui>
#include "ImportWizard.h"
#include "messagebox.h"
#include "Importers/BaseImporter.h"
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>




ImportPage::ImportPage(QWidget* parent) : QWizardPage(parent)
{
	gridLayout = new QGridLayout(this);

	label = new QLabel(this);

	gridLayout->addWidget(label, 1, 0, 1, 1);

	progressBar = new QProgressBar(this);

	gridLayout->addWidget(progressBar, 3, 0, 1, 1);

	verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

	gridLayout->addItem(verticalSpacer, 0, 0, 1, 1);

	verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

	gridLayout->addItem(verticalSpacer_2, 4, 0, 1, 1);

	registerField("ImportErrors", this, "ImportErrors");
	registerField("ImportResult", this, "ImportResult");
}

QString ImportPage::getImportErrors() const
{
	return m_importErrors;
}

int ImportPage::getImportResult() const
{
	return m_importResult;
}

void ImportPage::initializePage()
{
	BaseImporter* importer = field("Importer").value<BaseImporter*>();
	if (importer == NULL)
	{
		//CustomMessageBox::critical(NULL, "No importer", "Importer is NULL");
		return;
	}
	connect(importer, SIGNAL(progress(QString, int)), SLOT(progressCallback(QString, int)), Qt::QueuedConnection);
	connect(importer, SIGNAL(error(QString, QString)), SLOT(collectErrors(QString, QString)), Qt::QueuedConnection);
	QString dataPath = field("CustomDataPath").toString();
	if (dataPath.isEmpty())
	{
		importer->TryDetectResumePath(dataPath);
		
	}
	if (!dataPath.isEmpty())
	{
		importer->StartImport(dataPath);
	}
	
}

void ImportPage::cleanupPage()
{
	BaseImporter* importer = field("Importer").value<BaseImporter*>();
	if (importer == NULL)
	{
		//CustomMessageBox::critical(NULL, "No importer", "Importer is NULL");
		return;
	}
	if (importer->isRunning())
	{
		m_importResult = ImportWizard::Canceled;
		importer->CancelImport();
	}

	
}

int ImportPage::nextId() const
{
	return ImportWizard::Finish;
}

bool ImportPage::isComplete() const
{
	return progressBar->value() == 100;
}

void ImportPage::progressCallback(QString text, int progress)
{
	label->setText(text);
	progressBar->setValue(progress);
	if (progress == 100)
	{
		
		if (m_importErrors.isEmpty())
		{
			m_importResult = ImportWizard::Success;
		}
		else
		{
			m_importResult = ImportWizard::Failed;
		}
		emit completeChanged();
	}
//	boost::detail::win32::Sleep(500);
}

void ImportPage::collectErrors(QString name, QString error)
{
	m_importErrors.append(tr("IMPORTING_TORRENT %1 FAILED.\nERROR: %2\n").arg(name, error));
}
