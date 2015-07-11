/*
CuteTorrent BitTorrent Client with dht support, userfriendly interface
and some additional features which make it more convenient.
Copyright (C) 2012 Ruslan Fedoseyenko

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stddef.h>
#include <QDir>
#include <QMap>
#include <QMouseEvent>
#include <QMovie>
#include <QPainter>
#include <QTextCodec>
#include "messagebox.h"
#include "FileTreeModel.h"
#include "OpenTorrentDialog.h"
#include "QApplicationSettings.h"
#include "TorrentManager.h"
#include "qstorageinfo.h"

OpenTorrentDialog::OpenTorrentDialog(QWidget* parent, Qt::WindowFlags flags)
	: BaseWindow(BaseWindow::OnlyCloseButton, BaseWindow::NoResize, parent)
	, m_bUseGroup(false)
	, m_pFileTreeModel(NULL)
	, m_size(0)
{
	setupUi(this);
	setupCustomWindow();
	setupWindowIcons();
	m_pTorrentManager = TorrentManager::getInstance();
	QCompleter* pathComplitter = new QCompleter(this);
	m_compliterModel = new QFileSystemModel(pathComplitter);
	m_compliterModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
	m_compliterModel->setRootPath("");
	pathComplitter->setModel(m_compliterModel);
	pathComplitter->setCaseSensitivity(Qt::CaseInsensitive);
	pathComplitter->setCompletionMode(QCompleter::PopupCompletion);
	pathEdit->setCompleter(pathComplitter);
	validTorrent = true;
	/*QTextCodec* wantUnicode = QTextCodec::codecForName("UTF-8");
	QTextCodec::setCodecForTr(wantUnicode);
	QTextCodec::setCodecForLocale(wantUnicode);
	QTextCodec::setCodecForLocale(wantUnicode);*/
}

void OpenTorrentDialog::reject()
{
	if (m_pTorrentFilename.startsWith("magnet"))
	{
		m_pTorrentManager->CancelMagnetLink(m_pTorrentFilename);
	}

	QDialog::reject();
}
int OpenTorrentDialog::exec()
{
	return validTorrent ? QDialog::exec() : Rejected;
}
OpenTorrentDialog::~OpenTorrentDialog()
{
	if (m_pFileTreeModel != NULL)
	{
		delete m_pFileTreeModel;
	}
}



void OpenTorrentDialog::SetData(QString filename)
{
	m_pTorrentFilename = filename;

	if(filename.startsWith("magnet"))
	{
		QMovie* movie = new QMovie(":/images/loader.gif");
		loaderGifLabel->setMovie(movie);
		movie->start();
		qRegisterMetaType<openmagnet_info>("openmagnet_info");
		MetaDataDownloadWaiter* magnetWaiter = new MetaDataDownloadWaiter(filename, this);
		QObject::connect(magnetWaiter, SIGNAL(DownloadCompleted(openmagnet_info)), this, SLOT(DownloadMetadataCompleted(openmagnet_info)));
		QObject::connect(magnetWaiter, SIGNAL(ErrorOccured(QString)), this, SLOT(OnError(QString)));
		QObject::connect(magnetWaiter, SIGNAL(finished()), magnetWaiter, SLOT(deleteLater()));
		magnetWaiter->start(QThread::HighPriority);
		yesButton->setEnabled(false);
	}
	else
	{
		loaderGifLabel->hide();
		loaderTextLabel->hide();
		error_code ec;
		opentorrent_info* info = m_pTorrentManager->GetTorrentInfo(m_pTorrentFilename, ec);

		if(info != NULL && ec == 0)
		{
			setUpdatesEnabled(false);
			labelNameData->setText(info->name);
			QFontMetrics metrics(labelComentData->font());
			QString elidedText = metrics.elidedText(info->describtion, Qt::ElideRight, labelComentData->width());
			labelComentData->setText(elidedText);
			m_size = info->size;
			labelSizeData->setText(StaticHelpers::toKbMbGb(info->size));
			m_pFileTreeModel = new FileTreeModel();

			for (int i = 0; i < info->files.num_files(); i++)
			{
				m_pFileTreeModel->addPath(QString::fromUtf8(info->files.file_path(i).c_str()), StaticHelpers::toKbMbGb(info->files.file_size(i)));
			}

			torrentFilesTreeView->setModel(m_pFileTreeModel);
			torrentFilesTreeView->setColumnWidth(0, 300);
			setUpdatesEnabled(true);

			if(!info->baseSuffix.isEmpty())
			{
				QApplicationSettingsPtr settings = QApplicationSettings::getInstance();
				m_lFilters = settings->GetFileFilterGroups();
				int selected = -1;

				for(int i = 0; i < m_lFilters.count(); i++)
				{
					GroupComboBox->addItem(m_lFilters[i].Name());

					if(m_lFilters.at(i).Contains(info->baseSuffix) && selected < 0)
					{
						selected = i;
						QString path = m_lFilters.at(i).SavePath();
						m_compliterModel->setRootPath(path);
						pathEdit->setText(path);
					}
				}

				if(selected >= 0)
				{
					GroupComboBox->setCurrentIndex(selected);
				}
				else
				{
					QString lastDir = settings->valueString("System", "LastSaveTorrentDir", QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation));
					pathEdit->setText(lastDir);
					GroupComboBox->setCurrentIndex(-1);
				}
			}

			delete info;
		}
		else
		{
			validTorrent = false;
			CustomMessageBox::critical(this, tr("OPEN_TORRENT_ERROR"), StaticHelpers::translateLibTorrentError(ec));
		}
	}
}



void OpenTorrentDialog::BrowseButton()
{
	QApplicationSettingsPtr settings = QApplicationSettings::getInstance();
	QString lastDir = pathEdit->text();
	QString dir = QFileDialog::getExistingDirectory(this, tr("DIALOG_OPEN_DIR"),
	              lastDir,
	              QFileDialog::ShowDirsOnly
	              | QFileDialog::DontResolveSymlinks);

	if(!dir.isEmpty())
	{
		dir.append(QDir::separator());
		dir = QDir::toNativeSeparators(dir);
		settings->setValue("System", "LastSaveTorrentDir", dir);
		GroupComboBox->setCurrentIndex(-1);
		m_compliterModel->setRootPath(dir);
		pathEdit->setText(dir);
		m_bUseGroup = false;
	}
}



void OpenTorrentDialog::AccepTorrent()
{
	if(validTorrent)
	{
		QMap<QString, quint8> filePriorities = m_pFileTreeModel->getFilePiorites();
		error_code ec;
		int groupIndex = GroupComboBox->currentIndex();
		QString group = groupIndex >= 0 ? m_lFilters[groupIndex].Name() : "";

		if(!m_pTorrentFilename.startsWith("magnet"))
		{
			m_pTorrentManager->AddTorrent(m_pTorrentFilename, pathEdit->displayText(), labelNameData->text(), ec, filePriorities, group, setSequntialCheckBox->isChecked());
		}
		else
		{
			m_pTorrentManager->AddMagnet(m_info.handle, pathEdit->displayText(), group, filePriorities);
		}

		if(ec)
		{
			CustomMessageBox::critical(this, "Adding torrent Error", StaticHelpers::translateLibTorrentError(ec));
			return;
		}
	}

	accept();
}

void OpenTorrentDialog::ChangeGroup()
{
	if(GroupComboBox->currentIndex() >= 0 && GroupComboBox->currentIndex() < m_lFilters.length())
	{
		pathEdit->setText(m_lFilters[GroupComboBox->currentIndex()].SavePath());
		m_bUseGroup = true;
	}
}

void OpenTorrentDialog::DownloadMetadataCompleted(openmagnet_info info)
{
	loaderGifLabel->hide();
	loaderTextLabel->hide();
	yesButton->setEnabled(true);
	m_info = info;
	setUpdatesEnabled(false);
	labelNameData->setText(info.name);
	QFontMetrics metrics(labelComentData->font());
	QString elidedText = metrics.elidedText(info.describtion, Qt::ElideRight, labelComentData->width());
	labelComentData->setText(elidedText);
	m_size = info.size;
	labelSizeData->setText(StaticHelpers::toKbMbGb(info.size));
	m_pFileTreeModel = new FileTreeModel();

	for (int i = 0; i < info.files.num_files(); i++)
	{
		m_pFileTreeModel->addPath(QString::fromUtf8(info.files.file_path(i).c_str()), StaticHelpers::toKbMbGb(info.files.file_size(i)));
	}

	torrentFilesTreeView->setModel(m_pFileTreeModel);
	torrentFilesTreeView->setColumnWidth(0, 300);
	setUpdatesEnabled(true);

	if(!info.baseSuffix.isEmpty())
	{
		try
		{
			QApplicationSettingsPtr instance = QApplicationSettings::getInstance();
			m_lFilters = instance->GetFileFilterGroups();
			int selected = -1;

			for(int i = 0; i < m_lFilters.count(); i++)
			{
				GroupComboBox->addItem(m_lFilters[i].Name());

				if(m_lFilters.at(i).Contains(info.baseSuffix) && selected < 0)
				{
					selected = i;
					QString path = m_lFilters.at(i).SavePath();
					m_compliterModel->setRootPath(path);
					pathEdit->setText(path);
				}
			}

			if(selected >= 0)
			{
				GroupComboBox->setCurrentIndex(selected);
			}
		}
		catch(std::exception ex)
		{
			qDebug() << ex.what();
		}
	}
}



void OpenTorrentDialog::OnPathChanged(QString path)
{
	QStorageInfo storageInfo(path);
	labelSizeData->setText(tr("%1 (AVAILABLE: %2)").arg(StaticHelpers::toKbMbGb(m_size), StaticHelpers::toKbMbGb(storageInfo.bytesAvailable())));
}

void OpenTorrentDialog::changeEvent(QEvent* event)
{
	if(event->type() == QEvent::LanguageChange)
	{
		retranslateUi(this);
	}
}


QPushButton* OpenTorrentDialog::getCloseBtn()
{
	return pbClose;
}

QWidget* OpenTorrentDialog::getTitleBar()
{
	return titleBar;
}

QLabel* OpenTorrentDialog::getTitleLabel()
{
	return LTitle;
}

QLabel* OpenTorrentDialog::getTitleIcon()
{
	return tbMenu;
}

QWidget* OpenTorrentDialog::centralWidget()
{
	return m_centralWidget;
}

void OpenTorrentDialog::OnError(QString error_msg)
{
	CustomMessageBox::critical(this, "Error", error_msg);
	accept();
}
