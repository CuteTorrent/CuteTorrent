#include "applybackuppage.h"
#include "backupwizard.h"
#include "../FileTreeModel.h"
#include "../QApplicationSettings.h"
#include "../TorrentManager.h"
//#include "patheditor.h"
#include <algorithm>
#include <QApplication>
#include <QDebug>
template <class T> void swap(T& a, T& b)
{
	T c(a);
	a = b;
	b = c;
}

ApplyBackupPage::ApplyBackupPage(QWidget* parent) :
	QWizardPage(parent)
{
	setTitle("APPLY_BAKUP_TITLE");
	gridLayout = new QGridLayout(this);
	gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
	backupPathLineEdit = new QLineEdit(this);
	backupPathLineEdit->setObjectName(QString::fromUtf8("lineEdit"));
	backupPathLineEdit->setEnabled(false);
	gridLayout->addWidget(backupPathLineEdit, 0, 0, 1, 1);
	browsePushButton = new QPushButton(this);
	browsePushButton->setText(tr("BROWSE"));
	QObject::connect(browsePushButton, SIGNAL(clicked()), this, SLOT(browseButtonClicked()));
	gridLayout->addWidget(browsePushButton, 0, 1, 1, 1);
	changePathGroupBox = new QGroupBox(this);
	changePathGroupBox->setObjectName(QString::fromUtf8("groupBox"));
	changePathGroupBox->setCheckable(true);
	changePathGroupBox->setTitle(tr("CHANGE_PATH"));
	changePathGroupBox->setChecked(false);
	gridLayout_2 = new QGridLayout(changePathGroupBox);
	gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
	tableWidget = new QTableWidget(changePathGroupBox);

	if(tableWidget->columnCount() < 2)
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
	if(!pathResumeData.isEmpty())
	{
		int rowCount = tableWidget->rowCount();
		QMap<QString, QString> chmagePathMap;

		for(int i = 0; i < rowCount; i++)
		{
			QTableWidgetItem* originalPathItem = tableWidget->item(i, 0);
			QTableWidgetItem* newPathItem = tableWidget->item(i, 1);
			QString newPath = newPathItem->text();

			if(!newPath.isEmpty())
			{
				chmagePathMap.insert(originalPathItem->text(), newPath);
			}
		}

		QStringList lines = pathResumeData.split("\n");

		for(int i = 0; i < lines.count(); i++)
		{
			QString line = lines[i];
			QStringList parts = line.split('|');

			if(parts.length() > 2)
			{
				if(chmagePathMap.contains(parts[1]))
				{
					lines[i] = parts[0] + "|" + chmagePathMap[parts[1]] + "|" + parts[2];
				}
			}
		}

		QString newPathResumeData = lines.join("\n");
		zipReader->extractAll(QApplication::applicationDirPath());
		QFile pathResumeFile(QApplication::applicationDirPath() + "/CT_DATA/path.resume");

		if(pathResumeFile.open(QIODevice::WriteOnly))
		{
			QTextStream strm(&pathResumeFile);
			strm.setCodec("UTF-8");
			strm << newPathResumeData;
			pathResumeFile.close();
		}

		QApplicationSettings::getInstance()->ReedSettings();
		QApplicationSettings::FreeInstance();
		TorrentManager::getInstance()->InitSession();
		TorrentManager::freeInstance();
	}
}

int ApplyBackupPage::nextId() const
{
	ApplyBackup();
	return BackupWizard::Page_Finish;
}
QStringList ApplyBackupPage::GetLongestCommonSubstr(QStringList strings)
{
	strings.sort();
	FileTreeModel model;

	foreach(QString string, strings)
	{
		model.addPath(string, "");
	}

	return model.getUnicPathes();
}


bool ApplyBackupPage::parseData(QZipReader* reader)
{
	pathResumeData = reader->fileData("CT_DATA/path.resume");

	if(pathResumeData.isEmpty())
	{
		QMessageBox::critical(this, tr("ERROR"), tr("BAD_RESUME_DATA"));
		return false;
	}

	QStringList pathes;
	QStringList lines = pathResumeData.split("\n");

	foreach(QString line, lines)
	{
		QStringList parts = line.split('|');

		if(parts.length() > 2)
		{
			pathes << parts[1];
		}
	}

	QStringList unick = GetLongestCommonSubstr(pathes);
	int row = 0;
	tableWidget->setRowCount(unick.size());

	foreach(QString line, unick)
	{
		QTableWidgetItem* tableItem = new QTableWidgetItem();
		tableItem->setText(line);
		tableItem->setFlags(tableItem->flags() & ~Qt::ItemIsEditable);
		tableWidget->setItem(row, 0, tableItem);
		tableWidget->setItem(row, 1, new QTableWidgetItem());
		row++;
	}

	//tableWidget->setItemDelegateForColumn(1, new PathEditor);
	tableWidget->resizeColumnsToContents();
	tableWidget->resizeRowsToContents();
	return true;
}

void ApplyBackupPage::browseButtonClicked()
{
	backupPathLineEdit->setText(QFileDialog::getOpenFileName(this, tr("BACKUP_CHOOSE"), QApplication::applicationDirPath() + QDir::separator()));
	zipReader = new QZipReader(backupPathLineEdit->text());

	if(zipReader->isReadable())
	{
		parseData(zipReader);
	}
	else
	{
		QMessageBox::critical(this, tr("ERROR"), tr("UNABLE_TO_OPEN_BACKUP"));
		return;
	}
}
