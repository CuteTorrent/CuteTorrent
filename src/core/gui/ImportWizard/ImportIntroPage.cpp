#include "IntroPage.h"
#include "ImportWizard.h"
#include "ImportIntroPage.h"
#include "messagebox.h"
#include <QDebug>
#include "Importers/ImportersManager.h"
#include "Importers/BaseImporter.h"
ImportIntroPage::ImportIntroPage(QWidget* parent) :QWizardPage(parent), m_pCurrentImporter(NULL)
{
	setTitle(tr("IMPORT_INTRO_TITLE"));
	m_ptorrentImportersMapper = new QSignalMapper(this);
	QLabel* label = new QLabel(tr("IMPORT_INTRO_MESSAGE"));
	label->setWordWrap(true);
	QGridLayout* pClientGrid = new QGridLayout(this);
	ImportersManagerPtr importersManager = ImportersManager::getInstance();
	QList<BaseImporter*> importers = importersManager->getImporters();
	for (int i = 0; i < importers.size(); i++)
	{
		QRadioButton* pUtRadio = new QRadioButton(this);
		QString btClientName = importers[i]->BTClientName();
		pUtRadio->setText(btClientName);
		pUtRadio->setObjectName(btClientName);
		pClientGrid->addWidget(pUtRadio);
		m_ptorrentImportersMapper->setMapping(pUtRadio, btClientName);
		connect(pUtRadio, SIGNAL(toggled(bool)), m_ptorrentImportersMapper, SLOT(map()));
	}

	connect(m_ptorrentImportersMapper, SIGNAL(mapped(QString)), SLOT(setImporter(QString)));
	
	
	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(label);
	layout->addLayout(pClientGrid);
	
	setLayout(layout);
	registerField("Importer", this, "CurrentImporter");
	
}

void ImportIntroPage::initializePage()
{
}

void ImportIntroPage::cleanupPage()
{
}

int ImportIntroPage::nextId() const
{
	BaseImporter* importer = field("Importer").value<BaseImporter*>();
	if (importer != NULL)
	{
		QString resumePath;
		if (importer->TryDetectResumePath(resumePath))
		{
			return ImportWizard::Import;
		}
	}
	/*else
	{
		CustomMessageBox::critical(NULL, "No importer", "Importer is NULL");
	}*/
	return ImportWizard::ChoseData;
}

BaseImporter* ImportIntroPage::getCurrentImporter() const
{
	return m_pCurrentImporter;
}

bool ImportIntroPage::isComplete() const
{
	return m_pCurrentImporter != NULL;
}

void ImportIntroPage::setImporter(QString name)
{
	ImportersManagerPtr importersManager = ImportersManager::getInstance();
	QList<BaseImporter*> importers = importersManager->getImporters();
	QRadioButton* radio = findChild<QRadioButton*>(name);
	if (radio->isChecked())
	{
		for (int i = 0; i < importers.size(); i++)
		{
			QString btClientName = importers[i]->BTClientName();
			if (btClientName == name)
			{
				m_pCurrentImporter = importers[i];
				emit completeChanged();
				return;
			}
		}
		qCritical() << "No impoerter found for name " << name;
	}
	else
	{
		m_pCurrentImporter = NULL;
		emit completeChanged();
	}
	
	
}
