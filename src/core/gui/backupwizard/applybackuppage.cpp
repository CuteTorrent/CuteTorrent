#include "applybackuppage.h"
#include "backupwizard.h"
#include "FileTreeModel.h"
#include "QApplicationSettings.h"
#include "TorrentManager.h"
#include <libtorrent/bencode.hpp>
#include "JlCompress.h"
#include <helpers/StaticHelpers.h>
#include <core/RssManager.h>
#include <gui/Dialogs/InitializationDialog.h>

ApplyBackupPage::ApplyBackupPage(QWidget* parent) :
	QWizardPage(parent)
{
	setTitle(tr("APPLY_BACKUP_TITLE"));
	gridLayout = new QGridLayout(this);
	gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
	backupPathLineEdit = new QLineEdit(this);
	backupPathLineEdit->setObjectName(QString::fromUtf8("lineEdit"));
	backupPathLabel = new QLabel(this);
	backupPathLabel->setText(tr("PATH_TO_BACKUP_FILE"));
	gridLayout->addWidget(backupPathLabel, 0, 0, 1, 1);
	backupPathLineEdit->setEnabled(false);
	gridLayout->addWidget(backupPathLineEdit, 0, 1, 1, 1);
	browsePushButton = new QPushButton(this);
	browsePushButton->setText(tr("BROWSE"));
	connect(browsePushButton, SIGNAL(clicked()), this, SLOT(browseButtonClicked()));
	gridLayout->addWidget(browsePushButton, 0, 2, 1, 1);
	changePathGroupBox = new QGroupBox(this);
	changePathGroupBox->setObjectName(QString::fromUtf8("groupBox"));
	changePathGroupBox->setCheckable(true);
	changePathGroupBox->setTitle(tr("CHANGE_PATH"));
	changePathGroupBox->setChecked(false);
	gridLayout_2 = new QGridLayout(changePathGroupBox);
	gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
	tableWidget = new QTableWidget(changePathGroupBox);

	if (tableWidget->columnCount() < 2)
	{
		tableWidget->setColumnCount(2);
	}

	QTableWidgetItem* __qtablewidgetitem1 = new QTableWidgetItem();
	__qtablewidgetitem1->setText(tr("PATH"));
	QSize sz = __qtablewidgetitem1->sizeHint();
	sz.setWidth(190);
	sz.setHeight(20);
	__qtablewidgetitem1->setSizeHint(sz);
	tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem1);
	QTableWidgetItem* __qtablewidgetitem2 = new QTableWidgetItem();
	__qtablewidgetitem2->setText(tr("NEW_PATH"));
	__qtablewidgetitem2->setSizeHint(sz);
	tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem2);
	tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
	tableWidget->verticalHeader()->hide();
	gridLayout_2->addWidget(tableWidget, 0, 0, 1, 1);
	gridLayout->addWidget(changePathGroupBox, 1, 0, 1, 3);
	setLayout(gridLayout);
}

void ApplyBackupPage::ApplyBackup() const
{
	if (!backupPathLineEdit->text().isEmpty())
	{
		QString dataDir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
		JlCompress::extractDir(backupPathLineEdit->text(), dataDir);
		QString btSessionDirPath = StaticHelpers::CombinePathes(dataDir, "BtSessionData");
		QDir btSessionDir(btSessionDirPath);
		QStringList resumeFiles = btSessionDir.entryList(QStringList("*.resume"), QDir::Files | QDir::NoDotAndDotDot);
		QMap<QString, QString> pathMap;

		for (int i = 0; i < tableWidget->rowCount(); i++)
		{
			QTableWidgetItem* sourceItem = tableWidget->item(i, 0);
			QTableWidgetItem* destItem = tableWidget->item(i, 1);

			if (sourceItem != NULL && destItem != NULL)
			{
				pathMap.insert(sourceItem->text(), destItem->text());
			}
		}

		for (int i = 0; i < resumeFiles.size(); i++)
		{
			QFile file(btSessionDir.absoluteFilePath(resumeFiles[i]));

			if (file.open(QIODevice::ReadWrite))
			{
				QByteArray resumeData = file.readAll();
				entry e = bdecode(resumeData.begin(), resumeData.end());

				if (entry* savePathEntry = e.find_key("save_path"))
				{
					QString savePath = QString::fromUtf8(savePathEntry->string().c_str());

					if (pathMap.contains(savePath))
					{
						QString newPath = pathMap[savePath];
						e["save_path"] = newPath.toUtf8().data();
						file.seek(0);
						QByteArray out;
						bencode(std::back_inserter(out), e);
						file.write(out);
					}
				}

				file.close();
			}
		}

		QApplicationSettings::getInstance()->ReedSettings();
		RssManagerPtr rssManager = RssManager::getInstance();
		rssManager->LoadDownloadRules();
		rssManager->LoadFeeds();
		boost::scoped_ptr<InitializationDialog> pDlg(new InitializationDialog());
		pDlg->exec();
	}
}

int ApplyBackupPage::nextId() const
{
	ApplyBackup();
	return BackupWizard::Page_Finish;
}

bool ApplyBackupPage::isComplete() const
{
	return !backupPathLineEdit->text().isEmpty();
}

void ApplyBackupPage::initializePage()
{
	backupPathLineEdit->setText("");
	tableWidget->clearContents();
}

QStringList ApplyBackupPage::GetLongestCommonSubstr(QStringList strings)
{
	return strings.toSet().toList();
}

void ApplyBackupPage::browseButtonClicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("BACKUP_CHOOSE"), QApplication::applicationDirPath() + QDir::separator(), "Backup files (*.backup)");

	if (fileName.isEmpty() || !QFile::exists(fileName))
	{
		return;
	}

	backupPathLineEdit->setText(fileName);
	QuaZip backupFile(fileName);
	QStringList savePathes;

	if (backupFile.open(QuaZip::mdUnzip))
	{
		QList<QuaZipFileInfo64> fileInfos = backupFile.getFileInfoList64();

		for (int i = 0; i < fileInfos.size(); i++)
		{
			QuaZipFileInfo64 fileInfo = fileInfos[i];

			if (fileInfo.name.endsWith(".resume"))
			{
				backupFile.setCurrentFile(fileInfo.name);
				QStringList parts = fileInfo.name.split('/');
				QString resumeFileName = parts.last();
				QuaZipFile file(&backupFile);

				if (file.open(QIODevice::ReadOnly))
				{
					QByteArray resumeData;

					while (!file.atEnd())
					{
						char buf[4096];
						qint64 readLen = file.read(buf, 4096);

						if (readLen > 0)
						{
							resumeData.append(QByteArray(buf, readLen));
						}
					}

					entry e = bdecode(resumeData.begin(), resumeData.end());
					QString savePath;

					if (entry* i = e.find_key("save_path"))
					{
						savePath = QString::fromUtf8(i->string().c_str());
						savePathes.append(savePath);
					}
				}
				else
				{
					//Show error
				}
			}
		}
		
		QStringList uniquePathes = GetLongestCommonSubstr(savePathes);

		for (int i = 0; i < uniquePathes.size(); i++)
		{
			tableWidget->insertRow(i);
			tableWidget->setItem(i, 0, new QTableWidgetItem(uniquePathes[i]));
		}
	}
	else
	{
		//int zipError = backupFile.getZipError();
	}
	emit completeChanged();
}

