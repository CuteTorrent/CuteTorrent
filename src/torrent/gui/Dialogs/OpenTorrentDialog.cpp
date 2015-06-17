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

OpenTorrentDialog::OpenTorrentDialog(QWidget* parent, Qt::WindowFlags flags) : BaseWindow(BaseWindow::OnlyCloseButton, BaseWindow::NoResize, parent), m_bUseGroup(false), m_pFileTreeModel(NULL)
{
	setupUi(this);
	setupCustomWindow();
	setupWindowIcons();
	m_pTorrentManager = TorrentManager::getInstance();
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
	TorrentManager::freeInstance();

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
				QApplicationSettings* instance = QApplicationSettings::getInstance();
				m_lFilters = instance->GetFileFilterGroups();
				int selected = -1;

				for(int i = 0; i < m_lFilters.count(); i++)
				{
					GroupComboBox->addItem(m_lFilters[i].Name());

					if(m_lFilters.at(i).Contains(info->baseSuffix) && selected < 0)
					{
						selected = i;
						pathEdit->setText(m_lFilters.at(i).SavePath());
					}
				}

				if(selected >= 0)
				{
					GroupComboBox->setCurrentIndex(selected);
				}
				else
				{
					QApplicationSettings* settings = QApplicationSettings::getInstance();
					QString lastDir = settings->valueString("System", "LastSaveTorrentDir");
					pathEdit->setText(lastDir);
					GroupComboBox->setCurrentIndex(-1);
					QApplicationSettings::FreeInstance();
				}

				QApplicationSettings::FreeInstance();
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
	QApplicationSettings* settings = QApplicationSettings::getInstance();
	QString lastDir = pathEdit->text();
	QString dir = QFileDialog::getExistingDirectory(this, tr("DIALOF_OPEN_DIR"),
	              lastDir,
	              QFileDialog::ShowDirsOnly
	              | QFileDialog::DontResolveSymlinks);

	if(!dir.isEmpty())
	{
		dir.append(QDir::separator());
		dir = QDir::toNativeSeparators(dir);
		settings->setValue("System", "LastSaveTorrentDir", dir);
		GroupComboBox->setCurrentIndex(-1);
		pathEdit->setText(dir);
		m_bUseGroup = false;
	}

	QApplicationSettings::FreeInstance();
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
			QApplicationSettings* instance = QApplicationSettings::getInstance();
			m_lFilters = instance->GetFileFilterGroups();
			int selected = -1;

			for(int i = 0; i < m_lFilters.count(); i++)
			{
				GroupComboBox->addItem(m_lFilters[i].Name());

				if(m_lFilters.at(i).Contains(info.baseSuffix) && selected < 0)
				{
					selected = i;
					pathEdit->setText(m_lFilters.at(i).SavePath());
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

		QApplicationSettings::FreeInstance();
	}
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
