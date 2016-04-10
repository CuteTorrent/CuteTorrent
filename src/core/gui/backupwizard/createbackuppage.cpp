#include "createbackuppage.h"
#include "backupwizard.h"
#include "JlCompress.h"

CreateBackupPage::CreateBackupPage(QWidget* parent) :
	QWizardPage(parent)
{
	setTitle(tr("CREATE_BAKUP_TITLE"));
	layout = new QGridLayout(this);
	messageLabel = new QLabel(tr("BACKUP_CREATE_BAKUP_MESSAGE"));
	messageLabel->setWordWrap(true);
	layout->addWidget(messageLabel, 0, 0, 1, 4);
	savePathEdit = new QLineEdit(this);
	savePathEdit->setEnabled(false);
	savePathEdit->setObjectName(QString::fromUtf8("savePathEdit"));
	savePathEdit->setMaximumWidth(200);
	layout->addWidget(savePathEdit, 1, 0, 1, 3);
	browseButton = new QPushButton(this);
	browseButton->setObjectName(QString::fromUtf8("browseButton"));
	browseButton->setText(tr("Browse"));
	QObject::connect(browseButton, SIGNAL(clicked()), this, SLOT(browseButtonClick()));
	layout->addWidget(browseButton, 1, 3, 1, 1);
	setLayout(layout);
}

int CreateBackupPage::nextId() const
{
	createBackup();
	return BackupWizard::Page_Finish;
}

void CreateBackupPage::browseButtonClick()
{
	savePathEdit->setText(QFileDialog::getSaveFileName(this, tr("BACKUP_SAVE"), QApplication::applicationDirPath() + QDir::separator() + "CuteTorrent.backup"));
}


bool CreateBackupPage::validatePage()
{
	return !savePathEdit->text().isEmpty();
}

void CreateBackupPage::createBackup() const
{
	QString path = savePathEdit->text();
	QString dataDir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
	JlCompress::compressDir(path, dataDir, true);
}

