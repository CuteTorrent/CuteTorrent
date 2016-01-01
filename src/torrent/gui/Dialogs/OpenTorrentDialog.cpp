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
#include <viewModel/itemDelegate/FileSizeItemDelegate.h>
#include <viewModel/FileTreeSortProxyModel.h>
#include "MetaDataDownloadWaiter.h"
#include "StaticHelpers.h"
#include "defs.h"
#include <TorrentGroupsManager.h>
#include "FiltersViewModel.h"
#include "AddRssDwonloadRuleDialog.h"
OpenTorrentDialog::OpenTorrentDialog(QWidget* parent, Qt::WindowFlags flags)
	: BaseWindow(BaseWindow::OnlyCloseButton, BaseWindow::NoResize, parent)
	, m_size(0)
	, m_bUseGroup(false)
	, m_pFileTreeModel(NULL)
{
	setupUi(this);
	setupCustomWindow();
	setupWindowIcons();
	GroupComboBox = new TreeBox(this);
	gridLayout->addWidget(GroupComboBox, 1, 0, 1, 1);
	m_pGroupsModel = new FiltersViewModel(FiltersViewModel::Groups, this);
	GroupComboBox->setModel(m_pGroupsModel);
	GroupComboBox->setRootModelIndex(m_pGroupsModel->index(0, 0, QModelIndex()));
	GroupComboBox->setCurrentIndex(-1);
	
	connect(GroupComboBox, SIGNAL(currentIndexChanged(int)), SLOT(ChangeGroup()));
	m_pTorrentManager = TorrentManager::getInstance();
	QCompleter* pathComplitter = new QCompleter(this);
	m_compliterModel = new QFileSystemModel(pathComplitter);
	m_compliterModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
	m_compliterModel->setRootPath("");
	pathComplitter->setModel(m_compliterModel);
	pathEdit->setCompleter(pathComplitter);
	validTorrent = true;
	torrentFilesTreeView->setItemDelegateForColumn(1, new FileSizeItemDelegate(this));
}

void OpenTorrentDialog::reject()
{
	if (m_torrentFilename.startsWith("magnet"))
	{
		m_pTorrentManager->CancelMagnetLink(m_torrentFilename);
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


void OpenTorrentDialog::FillData(opentorrent_info* info)
{
	labelNameData->setText(info->name);
	QFontMetrics metrics(labelComentData->font());
	QString elidedText = metrics.elidedText(info->describtion, Qt::ElideRight, labelComentData->width());
	labelComentData->setText(elidedText);
	m_size = info->size;
	labelSizeData->setText(StaticHelpers::toKbMbGb(info->size));
	m_pFileTreeModel = new FileTreeModel();
	FileTreeSortProxyModel* sortModel = new FileTreeSortProxyModel(this);

	for (int i = 0; i < info->files.num_files(); i++)
	{
		m_pFileTreeModel->addPath(QString::fromUtf8(info->files.file_path(i).c_str()), info->files.file_size(i));
	}

	sortModel->setSourceModel(m_pFileTreeModel);
	torrentFilesTreeView->setModel(sortModel);
	torrentFilesTreeView->expandToDepth(0);
	torrentFilesTreeView->setColumnWidth(0, 300);
	torrentFilesTreeView->setColumnWidth(1, 60);
	torrentFilesTreeView->header()->setSortIndicator(0, Qt::AscendingOrder);

	if(!info->baseSuffix.isEmpty())
	{
		TorrentGroup* pTorrentGroup = TorrentGroupsManager::getInstance()->GetGroupByExtentions(info->baseSuffix);
		if (pTorrentGroup != NULL)
		{
			pathEdit->setText(pTorrentGroup->savePath());
			QModelIndex groupIndex = m_pGroupsModel->index(pTorrentGroup->uid());
			if (groupIndex.isValid())
			{
				GroupComboBox->treeView()->expand(groupIndex);
				GroupComboBox->setCurrentIndex(groupIndex);
			}
			else
			{
				qDebug() << "Invalid QModelIndex recived for group" << pTorrentGroup->name() ;
			}
		}
		else
		{
			qDebug() << "No group found for ext. " << info->baseSuffix;
		}
		
	}
}

void OpenTorrentDialog::SetData(QString filename)
{
	m_torrentFilename = filename;
	QApplicationSettingsPtr settings = QApplicationSettings::getInstance();
	QString lastSaveDir = settings->valueString("System", "LastSaveTorrentDir", QDir::homePath());
	pathEdit->setText(lastSaveDir);
	qDebug() << "lastSaveDir" << lastSaveDir;
	if(filename.startsWith("magnet"))
	{
		QMovie* movie = new QMovie(":/images/loader.gif");
		loaderGifLabel->setMovie(movie);
		movie->start();
		qRegisterMetaType<openmagnet_info>("openmagnet_info");
		MetaDataDownloadWaiter* magnetWaiter = new MetaDataDownloadWaiter(filename, this);
		connect(magnetWaiter, SIGNAL(DownloadCompleted(openmagnet_info)), this, SLOT(DownloadMetadataCompleted(openmagnet_info)));
		connect(magnetWaiter, SIGNAL(ErrorOccured(QString)), this, SLOT(OnError(QString)));
		connect(magnetWaiter, SIGNAL(finished()), magnetWaiter, SLOT(deleteLater()));
		magnetWaiter->start(QThread::HighPriority);
		yesButton->setEnabled(false);
		setSeedModeCheckBox->setEnabled(false);
		setSeedModeCheckBox->setToolTip(tr("MAGNET_LINKS_DONT_SUPORT_SEED_MODE"));
	}
	else
	{
		loaderGifLabel->hide();
		loaderTextLabel->hide();
		error_code ec;
		boost::scoped_ptr<opentorrent_info> info (m_pTorrentManager->GetTorrentInfo(m_torrentFilename, ec));

		if(info != NULL && ec == 0)
		{
			FillData(info.get());
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



bool OpenTorrentDialog::AccepTorrent()
{
	if(validTorrent)
	{
		QMap<QString, quint8> filePriorities = m_pFileTreeModel->getFilePiorites();
		QList<quint8> values = filePriorities.values();
		qDebug() << filePriorities;
		if (values.count(0) == values.size())
		{
			CustomMessageBox::critical(this, "Adding torrent Error", tr("SELECT_AT_LEAST_ONE_FILE"));
			return false;
		}

		error_code ec;
		QModelIndex groupIndex = GroupComboBox->currentIndex();
		QUuid groupUid = groupIndex.data(FiltersViewModel::FilterRole).value<QUuid>();
		TorrentGroup* group = TorrentGroupsManager::getInstance()->GetGroup(groupUid);
		TorrentManager::AddTorrentFlags flags = static_cast<TorrentManager::AddTorrentFlags>(BuildFlags());
		QString savePath = pathEdit->displayText();

		if(m_torrentFilename.startsWith("magnet"))
		{
			m_pTorrentManager->AddMagnet(m_info.handle, savePath, group, filePriorities, flags);
		}
		else
		{
			m_pTorrentManager->AddTorrent(m_torrentFilename, savePath, ec, labelNameData->text(), filePriorities, group, flags);
		}

		if(ec)
		{
			CustomMessageBox::critical(this, "Adding torrent Error", StaticHelpers::translateLibTorrentError(ec));
			return false;
		}
	}

	return true;
}

void OpenTorrentDialog::ChangeGroup()
{
	QModelIndex index = GroupComboBox->view()->currentIndex();

	if (index.isValid())
	{
		QUuid groupUid = index.data(FiltersViewModel::FilterRole).value<QUuid>();
		if (!groupUid.isNull())
		{
			TorrentGroup* pGroup = TorrentGroupsManager::getInstance()->GetGroup(groupUid);
			if (pGroup != NULL)
			{
				pathEdit->setText(pGroup->savePath());
			}
		}
			
	}
}

void OpenTorrentDialog::DownloadMetadataCompleted(openmagnet_info info)
{
	yesButton->setEnabled(true);
	loaderGifLabel->hide();
	loaderTextLabel->hide();
	m_info = info;
	FillData(&info);
}



void OpenTorrentDialog::OnPathChanged(QString path)
{
	QStorageInfo storageInfo(path);
	labelSizeData->setText(tr("%1 (AVAILABLE: %2)").arg(StaticHelpers::toKbMbGb(m_size), StaticHelpers::toKbMbGb(storageInfo.bytesAvailable())));
	if (m_size + 20l * 1024l * 1024l >storageInfo.bytesAvailable())
	{
		if (CustomMessageBox::critical(this, tr("NOT_ENOUGH_SPACE"), tr("NOT_ENOGH_STORRAGE_SPACE_TO_STORE_TORRENT"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
		{
			reject();
		}
	}
}

void OpenTorrentDialog::setCheckedValue(bool val)
{
	QAbstractItemModel* itemModel = torrentFilesTreeView->model();

	if (itemModel != NULL)
	{
		int rowCount = itemModel->rowCount();

		for (int i = 0; i < rowCount; i++)
		{
			itemModel->setData(itemModel->index(i, 0), val ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
		}
	}
}

void OpenTorrentDialog::OnCheckAll()
{
	setCheckedValue(true);
}

void OpenTorrentDialog::OnUncheckAll()
{
	setCheckedValue(false);
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

int OpenTorrentDialog::BuildFlags()
{
	TorrentManager::AddTorrentFlags res = 0;

	if (setSequntialCheckBox->isChecked())
	{
		res |= TorrentManager::SEQUENTIAL_MODE;
	}

	if (setSeedModeCheckBox->isChecked())
	{
		res |= TorrentManager::SEED_MODE;
	}

	if (setPausedCheckBox->isChecked())
	{
		res |= TorrentManager::PAUSED_MODE;
	}

	if (setSuperSeedCheckBox->isChecked())
	{
		res |= TorrentManager::SUPER_SEED_MODE;
	}

	return res;
}

void OpenTorrentDialog::done(int r)
{
	if (Accepted == r)
	{
		if (AccepTorrent())
		{
			QDialog::done(r);
		}
	}
	else if (Rejected == r)
	{
		QDialog::done(r);
	}
}

QWidget* OpenTorrentDialog::centralWidget()
{
	return m_centralWidget;
}

void OpenTorrentDialog::OnError(QString error_msg)
{
	CustomMessageBox::critical(this, "Error", error_msg);
	reject();
}
