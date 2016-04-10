#include "ChoseDataPage.h"
#include "ImportWizard.h"
#include <QtGui>
#include "Importers/BaseImporter.h"
#include "messagebox.h"

ChoseDataPage::ChoseDataPage(QWidget* parent): QWizardPage(parent)
{
	setTitle(tr("IMPORT_CHOSE_DATA_TITLE"));

	gridLayout = new QGridLayout(this);

	label = new QLabel(this);
	label->setObjectName(QString::fromUtf8("label"));

	gridLayout->addWidget(label, 0, 0, 1, 3);

	pushButton = new QPushButton(this);
	pushButton->setText(tr("BROWSE"));
	connect(pushButton, SIGNAL(clicked()), SLOT(ChooseFile()));
	gridLayout->addWidget(pushButton, 1, 2, 1, 1);

	lineEdit = new QLineEdit(this);


	gridLayout->addWidget(lineEdit, 1, 0, 1, 1);

	verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

	gridLayout->addItem(verticalSpacer, 2, 0, 1, 3);
	registerField("CustomDataPath", this, "CustomDataPath");
	setLayout(gridLayout);
}

bool ChoseDataPage::isComplete() const
{
	return !m_customDataPath.isEmpty();
}

int ChoseDataPage::nextId() const
{
	return ImportWizard::Import;
}

QString ChoseDataPage::getCustomDataPath() const
{
	return m_customDataPath;
}

void ChoseDataPage::ChooseFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("CHOOSE_UT_RESUME_FILE"), QDir::homePath(), tr("uTorrent resume file (resume.dat);;All files (*.*)"));
	if (!fileName.isEmpty())
	{
		m_customDataPath = fileName;
		emit completeChanged();
	}
}

