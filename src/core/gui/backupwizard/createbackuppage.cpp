#include "createbackuppage.h"
#include "backupwizard.h"
#include <private/qzipwriter_p.h>
CreateBackupPage::CreateBackupPage(QWidget* parent) :
	QWizardPage(parent)
{
	setTitle(tr("CREATE_BAKUP_TITLE"));
	layout = new QGridLayout;
	messageLabel = new QLabel(tr("BACKUP_CREATE_BAKUP_MESSAGE"));
	messageLabel->setWordWrap(true);
	layout->addWidget(messageLabel, 0, 0, 1, 2);
	savePathEdit = new QLineEdit(this);
	savePathEdit->setEnabled(false);
	savePathEdit->setObjectName(QString::fromUtf8("savePathEdit"));
	savePathEdit->setMaximumWidth(200);
	layout->addWidget(savePathEdit, 1, 0, 1, 1);
	browseButton = new QPushButton(this);
	browseButton->setObjectName(QString::fromUtf8("browseButton"));
	browseButton->setText(tr("Browse"));
	QObject::connect(browseButton, SIGNAL(clicked()), this, SLOT(browseButtonClick()));
	layout->addWidget(browseButton, 1, 1, 1, 1);
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
	return  !savePathEdit->text().isEmpty();
}

void CreateBackupPage::createBackup() const
{
	QString path = savePathEdit->text();
	QZipWriter zip(path);

	if(zip.status() != QZipWriter::NoError)
	{
		return ;
	}

	zip.setCompressionPolicy(QZipWriter::AutoCompress);
	QString dataDir;
#ifdef Q_WS_MAC
	dataDir = "/Library/CuteTorrent/";
#else
	dataDir = QDir::toNativeSeparators(QApplication::applicationDirPath() + QDir::separator());
#endif
	QString iniPath = "CuteTorrent.ini" ;
	QFile iniFile(dataDir + iniPath);

	if(iniFile.open(QIODevice::ReadOnly))
	{
		zip.addFile(iniPath, &iniFile);
	}

	QString dataPath = dataDir + "CT_DATA" + QDir::separator();
	zip.setCreationPermissions(QFile::permissions(dataPath));
	zip.addDirectory("CT_DATA");
	QDirIterator it(dataPath, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

	while(it.hasNext())
	{
		QString file_path = QDir::toNativeSeparators(it.next());

		if(it.fileInfo().isDir())
		{
			zip.setCreationPermissions(QFile::permissions(file_path));
			zip.addDirectory(file_path.remove(path));
		}
		else if(it.fileInfo().isFile())
		{
			QFile file(file_path);

			if(!file.open(QIODevice::ReadOnly))
			{
				continue;
			}

			zip.setCreationPermissions(QFile::permissions(file_path));
			QByteArray ba = file.readAll();
			zip.addFile(file_path.remove(dataDir), ba);
		}
	}

	zip.close();
}


