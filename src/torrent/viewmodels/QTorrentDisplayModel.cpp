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
#include <QClipboard>
#include <QDebug>
#include <QDir>
#include <QItemSelectionModel>
#include <exception>
#include "MultipleDTDialog.h"
#include "VirtualDiskMounter.h"
#include "QApplicationSettings.h"
#include "QTorrentDisplayModel.h"
#include "StyleEngene.h"
#include "Torrent.h"
#include "TorrentManager.h"
#include "TorrentStorrage.h"
#include "VideoPlayerWindow.h"
#include "messagebox.h"
#include "QTorrentFilterProxyModel.h"

QTorrentDisplayModel::QTorrentDisplayModel(QTreeView* _parrent, QTorrentFilterProxyModel* pProxyFilterModel, QObject* __parrent) : QAbstractListModel(__parrent)
{
	m_pTorrentListView = _parrent;
	m_pTorrentManager = TorrentManager::getInstance();
	m_pTorrentStorrage = TorrentStorrage::getInstance();
	m_pProxyFilterModel = pProxyFilterModel;
	selectedRow = -1;
	setupContextMenu();
	locker = new QMutex();
	connect(m_pTorrentManager.get(), SIGNAL(AddTorrentGui(Torrent*)), SLOT(OnAddTorrent()));
	connect(m_pTorrentManager.get(), SIGNAL(TorrentRemove(QString)), SLOT(Update()));
}

QString QTorrentDisplayModel::getFirstAvailibleFile(files_info& filesInfo)
{
	int numFiles = filesInfo.storrage.num_files();
	for (int i = 0; i < numFiles; i++)
	{
		if (filesInfo.priorities[i] > 0)
		{
			return QString::fromUtf8(filesInfo.storrage.file_name(i).c_str());
		}
	}
	return "";
}

bool QTorrentDisplayModel::checkAllMountable(QModelIndexList& indexes)
{
	for (int i = 0; i < indexes.size(); i++)
	{
		Torrent* pTorrent = indexes[i].data(TorrentRole).value<Torrent*>();
		if (!pTorrent->isDaemonToolsMountable())
		{
			return false;
		}
	}
	return true;
}

bool QTorrentDisplayModel::checkAllSuperSeed(QModelIndexList& indexes)
{
	for (int i = 0; i < indexes.size(); i++)
	{
		Torrent* pTorrent = indexes[i].data(TorrentRole).value<Torrent*>();
		if (!pTorrent->isSuperSeed())
		{
			return false;
		}
	}
	return true;
}

bool QTorrentDisplayModel::checkAllHasMedia(QModelIndexList& indexes)
{
	for (int i = 0; i < indexes.size(); i++)
	{
		Torrent* pTorrent = indexes[i].data(TorrentRole).value<Torrent*>();
		if (!pTorrent->hasMediaFiles())
		{
			return false;
		}
	}
	return true;
}

bool QTorrentDisplayModel::checkAllSequential(QModelIndexList& indexes)
{
	for (int i = 0; i < indexes.size(); i++)
	{
		Torrent* pTorrent = indexes[i].data(TorrentRole).value<Torrent*>();
		if (!pTorrent->isSquential())
		{
			return false;
		}
	}
	return true;
}

void QTorrentDisplayModel::checkPausedResumed(QModelIndexList indexes, bool& isAllPaused, bool& isAllResumed, bool& hasPaused, bool& hasResumed)
{
	for (int i = 0; i < indexes.size(); i++)
	{
		Torrent* pTorrent = indexes[i].data(TorrentRole).value<Torrent*>();
		bool isPaused = pTorrent->isPaused();
		bool isResumed = !isPaused;

		isAllPaused = isAllPaused && isPaused;
		isAllResumed = isAllResumed && isResumed;

		if (isPaused && !hasPaused)
		{
			hasPaused = true;
		}

		if (isResumed && !hasResumed)
		{
			hasResumed = true;
		}
	}
}

void QTorrentDisplayModel::Rehash()
{
	ActionOnSelectedItem(rehash);
}
void QTorrentDisplayModel::DellTorrentOnly()
{
	ActionOnSelectedItem(remove);
}

void QTorrentDisplayModel::DellAll()
{
	ActionOnSelectedItem(remove_all);
}

void QTorrentDisplayModel::MountDT()
{
	QModelIndexList indexes = m_pProxyFilterModel->mapSelectionToSource(m_pTorrentListView->selectionModel()->selection()).indexes();

	for (int i = 0; i < indexes.size(); i++)
	{
		Torrent* torrent = indexes[i].data(TorrentRole).value<Torrent*>();
		if (torrent != NULL)
		{
			if (!torrent->isPaused())
			{
				torrent->pause();
			}

			QStringList images = torrent->GetImageFiles();

			if (images.size() > 1)
			{
				MultipleDTDialog dlg(images, m_pTorrentListView);
				dlg.exec();
			}
			else
			{
				if (images.size() == 1)
				{
					VirtualDiskMounterPtr pDiskMounter = VirtualDiskMounter::getInstance();
					pDiskMounter->MountVirualDiskImage(images.first());
				}
			}
		}
	}
	
}
void QTorrentDisplayModel::OpenDirSelected()
{
	if (m_pTorrentListView->model() != m_pProxyFilterModel)
	{
		return;
	}

	Torrent* tor = GetSelectedTorrent();

	if(tor != NULL)
	{
		files_info fileDownloadInfo = tor->GetFileDownloadInfo();
		QString pathPart = (tor->isSingleFile() ? getFirstAvailibleFile(fileDownloadInfo) : tor->GetName());
		QString path = QFileInfo(StaticHelpers::CombinePathes(tor->GetSavePath(), pathPart)).absoluteFilePath();
#ifdef Q_WS_MAC
		QStringList args;
		args << "-e";
		args << "tell application \"Finder\"";
		args << "-e";
		args << "activate";
		args << "-e";
		args << "select POSIX file \"" + path + "\"";
		args << "-e";
		args << "end tell";
		QProcess::startDetached("osascript", args);
#endif
#ifdef Q_WS_X11
		StaticHelpers::OpenFolderNautilus(path);
#endif
#ifdef Q_WS_WIN
		StaticHelpers::OpenFileInExplorer(path);
#endif
	}
}



void QTorrentDisplayModel::contextualMenu(const QPoint& point)
{
	if (m_pTorrentListView->model() != m_pProxyFilterModel)
	{
		return;
	}

	QModelIndexList indexes = m_pProxyFilterModel->mapSelectionToSource(m_pTorrentListView->selectionModel()->selection()).indexes();

	if (indexes.size() > 0)
	{
		bool isAllMountable = checkAllMountable(indexes);
		bool isAllSuperSeed = checkAllSuperSeed(indexes);
		bool isAllHasMedia = checkAllHasMedia(indexes);
		bool isAllSequential = checkAllSequential(indexes);
		bool isAllPaused = true, isAllResumed = true, hasPaused = false, hasResumed = false;
		menu->removeAction(pauseTorrent);
		menu->removeAction(resumeTorrent);
		checkPausedResumed(indexes, isAllPaused, isAllResumed, hasPaused, hasResumed);
		if (isAllPaused || hasPaused)
		{
			menu->insertAction(pauseResumeSeparator, resumeTorrent);
		}
		if (isAllResumed || hasResumed)
		{
			menu->insertAction(pauseResumeSeparator, pauseTorrent);
		}
		mountDiskImage->setEnabled(isAllMountable);
		superSeed->setChecked(isAllSuperSeed);
		playInMediaPlayer->setEnabled(isAllHasMedia);
		setSequentual->setChecked(isAllSequential);
		
		if (indexes.size() == 1)
		{
			Torrent* torrent = indexes[0].data(TorrentRole).value<Torrent*>();
			QString group = torrent->GetGroup();
			QList<QAction*> actions = groupsMenu->actions();
			if (!group.isEmpty())
			{
				for (int i = 0; i < actions.size(); i++)
				{
					actions[i]->setChecked(actions[i]->text() == group);
				}
			}
		}
			
		menu->exec(m_pTorrentListView->mapToGlobal(point));
	}
	else
	{
		QMutexLocker lockMutex(locker);
		m_pTorrentListView->selectionModel()->reset();
		selectedRow = -1;
	}
}


void QTorrentDisplayModel::UpdateSelectedIndex(const QItemSelection& selection)
{
	if (m_pTorrentListView->model() != m_pProxyFilterModel)
	{
		return;
	}

	try
	{
		QModelIndexList indexes = selection.indexes();

		if(indexes.count() >= 1)
		{
			selectedRow = indexes[0].row();
			m_pCurrentTorrent = indexes[0].data(TorrentRole).value<Torrent*>();
		}
		else
		{
			selectedRow = -1;
			m_pCurrentTorrent = NULL;
		}
	}
	catch(std::exception e)
	{
		qDebug() << "Exception QTorrentDisplayModel::UpdateSelectedIndex" << e.what();
	}
}

int QTorrentDisplayModel::rowCount(const QModelIndex& parent) const
{
	try
	{
		if (!parent.isValid())
		{
			return m_pTorrentStorrage->count();
		}
	}
	catch(std::exception e)
	{
		qDebug() << "Exception QTorrentDisplayModel::rowCount" << e.what();
	}

	return 0;
}
Torrent* QTorrentDisplayModel::GetSelectedTorrent()
{
	try
	{
		if(rowCount() == 0)
		{
			return NULL;
		}

		if(selectedRow >= rowCount())
		{
			return NULL;
		}

		if(selectedRow < 0)
		{
			return NULL;
		}

		return m_pCurrentTorrent;
	}
	catch(std::exception e)
	{
		qDebug() << "Exception QTorrentDisplayModel::GetSelectedTorrent" << e.what();
	}

	return NULL;
}
bool indexByRowLessThan(const QModelIndex& right, const QModelIndex& left)
{
	return right.row() < left.row();
}
void QTorrentDisplayModel::ActionOnSelectedItem(action wtf)
{
	if (m_pTorrentListView->model() != m_pProxyFilterModel)
	{
		return;
	}

	try
	{
		QModelIndexList indexes = m_pTorrentListView->selectionModel()->selectedIndexes();
		qSort(indexes);

		if(rowCount() == 0)
		{
			return;
		}

		switch(wtf)
		{
			case stop:
			{
				for (int i = indexes.count() - 1; i >= 0; i -= 1)
				{
					QModelIndex index = indexes[i];
					Torrent* torrent = index.data(TorrentRole).value<Torrent*>();
					torrent->stop();
				}

				break;
			}

			case pause:
			{
				for (int i = indexes.count() - 1; i >= 0; i -= 1)
				{
					QModelIndex index = indexes[i];
					Torrent* torrent = index.data(TorrentRole).value<Torrent*>();
					torrent->pause();
				}

				break;
			}

			case remove:
			{
				bool yesToAll = false;

				for (int i = indexes.count() - 1; i >= 0; i -= 1)
				{
					QModelIndex& index = indexes[i];

					if (index.isValid())
					{
						QMessageBox::StandardButton button;

						if (!yesToAll)
						{
							Torrent* torrent = index.data(TorrentRole).value<Torrent*>();
							QMessageBox::StandardButtons buttons = QMessageBox::Yes | QMessageBox::No;

							if (indexes.length() > 1)
							{
								buttons |= QMessageBox::YesToAll;
								buttons |= QMessageBox::NoToAll;
							}

							button = CustomMessageBox::question(m_pTorrentListView, tr("TORRENT_DELITION"), tr("TORRENT_DELITION_MSG").arg(torrent->GetName()), buttons);

							if (button == QMessageBox::YesToAll)
							{
								yesToAll = true;
							}
							else if (button == QMessageBox::NoToAll)
							{
								break;
							}
						}
						else
						{
							button = QMessageBox::YesToAll;
						}

						if (QMessageBox::No != button || yesToAll)
						{
							removeRow(index.row(), false);
						}
					}
				}

				break;
			}

			case remove_all:
			{
				bool yesToAll = false;

				for (int i = indexes.count() - 1; i >= 0; i -= 1)
				{
					QModelIndex& index = indexes[i];
					QMessageBox::StandardButton button;

					if (!yesToAll)
					{
						Torrent* torrent = index.data(TorrentRole).value<Torrent*>();
						QMessageBox::StandardButtons buttons = QMessageBox::Yes | QMessageBox::No;

						if (indexes.length() > 1)
						{
							buttons |= QMessageBox::YesToAll;
							buttons |= QMessageBox::NoToAll;
						}

						button = CustomMessageBox::question(m_pTorrentListView, tr("TORRENT_DELITION"), tr("TORRENT_ALL_DELITION_MSG").arg(torrent->GetName()), buttons);

						if (button == QMessageBox::YesToAll)
						{
							yesToAll = true;
						}
						else if (button == QMessageBox::NoToAll)
						{
							break;
						}
					}
					else
					{
						button = QMessageBox::YesToAll;
					}

					if (QMessageBox::No != button || yesToAll)
					{
						removeRow(index.row(), true);
					}
				}

				break;
			}

			case resume:
			{
				for (int i = indexes.count() - 1; i >= 0; i -= 1)
				{
					QModelIndex index = indexes[i];
					Torrent* torrent = index.data(TorrentRole).value<Torrent*>();
					torrent->resume();
				}

				break;
			}

			case rehash:
			{
				for (int i = indexes.count() - 1; i >= 0; i -= 1)
				{
					QModelIndex index = indexes[i];
					Torrent* torrent = index.data(TorrentRole).value<Torrent*>();
					torrent->announceRehash();
				}

				break;
			}

			case move_storrage:
			{
				Torrent* firstTorrent = indexes.first().data(TorrentRole).value<Torrent*>();
				QString path = QFileDialog::getExistingDirectory(m_pTorrentListView, tr("DIALOG_OPEN_FOLDER"),
				               firstTorrent->GetSavePath(),
				               QFileDialog::ShowDirsOnly
				               | QFileDialog::DontResolveSymlinks);

				if(!path.isEmpty())
				{
					for (int i = indexes.count() - 1; i >= 0; i -= 1)
					{
						QModelIndex index = indexes[i];
						Torrent* torrent = index.data(TorrentRole).value<Torrent*>();
						torrent->MoveStorrage(path + QDir::separator());
					}
				}

				break;
			}

			case set_sequntial:
			{
				for (int i = indexes.count() - 1; i >= 0; i -= 1)
				{
					QModelIndex index = indexes[i];
					Torrent* torrent = index.data(TorrentRole).value<Torrent*>();
					torrent->seqensialDownload();
				}

				break;
			}

			case set_superseed:
			{
				for (int i = indexes.count() - 1; i >= 0; i -= 1)
				{
					QModelIndex index = indexes[i];
					Torrent* torrent = index.data(TorrentRole).value<Torrent*>();
					torrent->SwitchSuperSeed();
				}

				break;
			}

			case update_trackers:
			{
				for (int i = indexes.count() - 1; i >= 0; i -= 1)
				{
					QModelIndex index = indexes[i];
					Torrent* torrent = index.data(TorrentRole).value<Torrent*>();
					torrent->UpdateTrackers();
				}

				break;
			}

			case generate_magmet:
			{
				QString clipboardData;

				for (int i = indexes.count() - 1; i >= 0; i -= 1)
				{
					QModelIndex index = indexes[i];
					Torrent* torrent = index.data(TorrentRole).value<Torrent*>();
					clipboardData += torrent->generateMagnetLink();
					clipboardData += "\n";
				}

				QApplication::clipboard()->setText(clipboardData);
				break;
			}

			case change_group:
				break;

			default:
				break;
		}
	}
	catch(std::exception e)
	{
		qDebug() << "Exception QTorrentDisplayModel::ActionOnSelectedItem" << e.what();
	}
}

QVariant QTorrentDisplayModel::data(const QModelIndex& index, int role) const
{
	QVariant var;
	const int row = index.row();

	if(row < 0 || row >= m_pTorrentStorrage->count())
	{
		return QVariant();
	}

	Torrent* t = m_pTorrentStorrage->at(row);

	switch(role)
	{
		case Qt::DisplayRole:
			var = t->GetName() ;
			break;

		case Qt::DecorationRole:
			var = t->GetMimeTypeIcon();
			break;

		case TorrentRole:
			var = qVariantFromValue(t);
			break;

		default:
			break;
	}

	return var;
}

bool QTorrentDisplayModel::removeRow(int row, bool delFiles)
{
	if((row > m_pTorrentStorrage->count()) || (row < 0))
	{
		return false;
	}

	if(rowCount() == 0)
	{
		return false;
	}

	beginRemoveRows(QModelIndex(), row, row);

	if(m_pTorrentStorrage->at(row) == m_pCurrentTorrent)
	{
		m_pCurrentTorrent = NULL;
	}

	m_pTorrentManager->RemoveTorrent(m_pTorrentStorrage->at(row)->GetInfoHash(), delFiles);
	endRemoveRows();
	return true;
}

bool QTorrentDisplayModel::removeRows(int row, int count, const QModelIndex& parent)
{
	if((row + count > m_pTorrentStorrage->count()) || (row < 0))
	{
		return false;
	}

	if(rowCount() == 0)
	{
		return false;
	}

	selectedRow = -1;
	m_pTorrentListView->selectionModel()->reset();
	beginRemoveRows(parent, row, row + count);

	for(int i = row; i < row + count; i++)
	{
		m_pTorrentManager->RemoveTorrent(m_pTorrentStorrage->at(i)->GetInfoHash(), true);
	}

	endRemoveRows();
	return true;
}

QTorrentDisplayModel::~QTorrentDisplayModel()
{
}

void QTorrentDisplayModel::retranslate()
{
	pauseTorrent->setText(tr("ACTION_TORRENTLIST_PAUSE"));
	resumeTorrent->setText(tr("ACTION_TORRENTLIST_RESUME"));
	openDir->setText(tr("ACTION_OPEN_FOLDER"));
	mountDiskImage->setText(tr("ACTION_DT_MOUNT"));
	deleteWithFiles->setText(tr("ACTION_DELETE_ALL"));
	deleteTorrentOnly->setText(tr("ACTION_DELETE_TORRENT"));
	HashRecheck->setText(tr("ACTION_REHASH"));
	setSequentual->setText(tr("ACTION_SET_SEQUENTIAL"));
	updateTrackers->setText(tr("ACTION_UPDATE_TRACKERS"));
	storrageMove->setText(tr("ACTION_MOVE_STORRAGE"));
	playInMediaPlayer->setText(tr("ACTION_PLAY_IN_PLAYER"));
	superSeed->setText(tr("ACTION_SET_SUPERSEED"));
	generateMagnet->setText(tr("ACTION_GENERATE_MAGNET"));
	groupsMenu->setTitle(tr("ACTION_CHANGE_GROUP"));
}

void QTorrentDisplayModel::setSequentualDL()
{
	ActionOnSelectedItem(set_sequntial);
}


void QTorrentDisplayModel::UpdateTrackers()
{
	ActionOnSelectedItem(update_trackers);
}

void QTorrentDisplayModel::moveStorrage()
{
	ActionOnSelectedItem(move_storrage);
}

void QTorrentDisplayModel::playInPlayer()
{
	if (m_pCurrentTorrent != NULL)
	{
		VideoPlayerWindow* vpw = new VideoPlayerWindow(m_pTorrentListView);
		files_info filesInfo = m_pCurrentTorrent->GetFileDownloadInfo();
		QString firstFileSubPath = getFirstAvailibleFile(filesInfo);
		QString filePath;
		if (m_pCurrentTorrent->isSingleFile())
		{
			filePath = StaticHelpers::CombinePathes(m_pCurrentTorrent->GetSavePath(), firstFileSubPath);
		}
		else
		{
			filePath = StaticHelpers::CombinePathes(m_pCurrentTorrent->GetSavePath(), m_pCurrentTorrent->GetName(), firstFileSubPath);
		}
		vpw->openFile(filePath);
		vpw->show();
	}
}

void QTorrentDisplayModel::setupContextMenu()
{
	menu = new QMenu(m_pTorrentListView);
	StyleEngene* pStyleEngene = StyleEngene::getInstance();
	
	pauseTorrent = new QAction(pStyleEngene->getIcon("pause"), tr("ACTION_TORRENTLIST_PAUSE"), this);
	pauseTorrent->setObjectName("ACTION_TORRENTLIST_PAUSE");
	connect(pauseTorrent, SIGNAL(triggered()), this, SLOT(PauseSelected()));
	resumeTorrent = new QAction(pStyleEngene->getIcon("play"), tr("ACTION_TORRENTLIST_RESUME"), this);
	resumeTorrent->setObjectName("ACTION_TORRENTLIST_RESUME");
	connect(resumeTorrent, SIGNAL(triggered()), this, SLOT(ResumeSelected()));
	pauseResumeSeparator = menu->addSeparator();
	openDir = new QAction(pStyleEngene->getIcon("open_folder"), tr("ACTION_OPEN_FOLDER"), this);
	openDir->setObjectName("ACTION_TORRENTLIST_OPEN_DIR");
	connect(openDir, SIGNAL(triggered()), this, SLOT(OpenDirSelected()));
	openDir->setShortcut(Qt::Key_Enter);
	menu->addAction(openDir);
	menu->addSeparator();
	mountDiskImage = new QAction(pStyleEngene->getIcon("daemon_tools"), tr("ACTION_DT_MOUNT"), this);
	mountDiskImage->setObjectName("ACTION_TORRENTLIST_DT_MOUNT");
	connect(mountDiskImage, SIGNAL(triggered()), this, SLOT(MountDT()));
	menu->addAction(mountDiskImage);
	playInMediaPlayer = new QAction(pStyleEngene->getIcon("play"), tr("ACTION_PLAY_IN_PLAYER"), this);
	playInMediaPlayer->setObjectName("ACTION_TORRENTLIST_PLAY");
	connect(playInMediaPlayer, SIGNAL(triggered()), this, SLOT(playInPlayer()));
	menu->addAction(playInMediaPlayer);
	storrageMove = new QAction(pStyleEngene->getIcon("move_folder"), tr("ACTION_MOVE_STORRAGE"), this);
	storrageMove->setObjectName("ACTION_TORRENTLIST_MOVE_STORRAGE");
	connect(storrageMove, SIGNAL(triggered()), this, SLOT(moveStorrage()));
	menu->addAction(storrageMove);
	menu->addSeparator();
	superSeed = new QAction(pStyleEngene->getIcon("super_seed"), tr("ACTION_SET_SUPERSEED"), this);
	superSeed->setObjectName("ACTION_TORRENTLIST_SUPER_SEED");
	superSeed->setCheckable(true);
	connect(superSeed, SIGNAL(triggered()), this, SLOT(SetSuperSeed()));
	menu->addAction(superSeed);
	HashRecheck = new QAction(pStyleEngene->getIcon("recheck"), tr("ACTION_REHASH"), this);
	HashRecheck->setObjectName("ACTION_TORRENTLIST_RECHECK");
	connect(HashRecheck, SIGNAL(triggered()), this, SLOT(Rehash()));
	menu->addAction(HashRecheck);
	updateTrackers = new QAction(pStyleEngene->getIcon("update_trackers"), tr("ACTION_UPDATE_TRACKERS"), this);
	updateTrackers->setObjectName("ACTION_TORRENTLIST_UPDATE_TRACKERS");
	connect(updateTrackers, SIGNAL(triggered()), this, SLOT(UpdateTrackers()));
	menu->addAction(updateTrackers);
	setSequentual = new QAction(pStyleEngene->getIcon("sequential"), tr("ACTION_SET_SEQUENTIAL"), this);
	setSequentual->setObjectName("ACTION_TORRENTLIST_SET_SEQUNTIAL");
	setSequentual->setCheckable(true);
	connect(setSequentual, SIGNAL(triggered()), this, SLOT(setSequentualDL()));
	menu->addAction(setSequentual);
	generateMagnet = new QAction(pStyleEngene->getIcon("magnet"), tr("ACTION_GENERATE_MAGNET"), this);
	generateMagnet->setObjectName("ACTION_TORRENTLIST_GENERATE_MAGNET");
	connect(generateMagnet, SIGNAL(triggered()), this, SLOT(generateMagnetLink()));
	menu->addAction(generateMagnet);
	menu->addSeparator();
	deleteWithFiles = new QAction(pStyleEngene->getIcon("delete"), tr("ACTION_DELETE_ALL"), this);
	deleteWithFiles->setObjectName("ACTION_TORRENTLIST_DEL_ALL");
	connect(deleteWithFiles, SIGNAL(triggered()), this, SLOT(DellAll()));
	deleteWithFiles->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_Delete));
	menu->addAction(deleteWithFiles);
	deleteTorrentOnly = new QAction(pStyleEngene->getIcon("delete"), tr("ACTION_DELETE_TORRENT"), this);
	deleteTorrentOnly->setObjectName("ACTION_TORRENTLIST_DEL_TORRENT");
	connect(deleteTorrentOnly, SIGNAL(triggered()), this, SLOT(DellTorrentOnly()));
	deleteTorrentOnly->setShortcut(Qt::Key_Delete);
	menu->addAction(deleteTorrentOnly);
	groupsMenu = new QMenu(tr("ACTION_CHANGE_GROUP"), menu);
	groupsMenu->setIcon(pStyleEngene->getIcon("groups"));
	QList<GroupForFileFiltering> filters = QApplicationSettings::getInstance()->GetFileFilterGroups();
	QString type;

	for(int i = 0; i < filters.size(); i++)
	{
		QAction* changeGroupAction = new QAction(pStyleEngene->guessMimeIcon(filters[i].Extensions().split('|') [0], type), filters[i].Name(), groupsMenu);
		changeGroupAction->setObjectName(filters[i].Name());
		connect(changeGroupAction, SIGNAL(triggered()), this, SLOT(changeGroup()));
		changeGroupAction->setCheckable(true);
		groupsMenu->addAction(changeGroupAction);
	}

	menu->addSeparator();
	menu->addMenu(groupsMenu);
}
bool LessThan(const Torrent* a, const Torrent* b)
{
	return (*a) < (*b);
}


void QTorrentDisplayModel::SetSuperSeed()
{
	ActionOnSelectedItem(set_superseed);
}

void QTorrentDisplayModel::initSessionFinished()
{
	m_pTorrentStorrage->sort();
	QTimer::singleShot(3000, this, SIGNAL(initCompleted()));
}

void QTorrentDisplayModel::generateMagnetLink()
{
	ActionOnSelectedItem(generate_magmet);
}

void QTorrentDisplayModel::changeGroup()
{
	try
	{
		QAction* senderAct = static_cast<QAction*>(sender());
		QString group = senderAct->text();
		QList<QAction*> actions = groupsMenu->actions();

		for(int i = 0; i < actions.size(); i++)
		{
			actions[i]->setChecked(actions[i] == senderAct);
		}

		QModelIndexList indexes = m_pTorrentListView->selectionModel()->selectedIndexes();

		foreach(QModelIndex index, indexes)
		{
			index.data(TorrentRole).value<Torrent*>()->setGroup(group);
		}
	}
	catch(...)
	{
		qDebug() << "Exception in QTorrentDisplayModel::changeGroup";
	}
}

void QTorrentDisplayModel::Update()
{
	QMutexLocker lockMutex(locker);
	reset();
}

void QTorrentDisplayModel::OnAddTorrent()
{
	Update();
	if (m_pTorrentListView->model() != m_pProxyFilterModel)
	{
		return;
	}
	m_pTorrentListView->scrollToBottom();
	m_pTorrentListView->setCurrentIndex(index(m_pTorrentStorrage->count() - 1));
}

void QTorrentDisplayModel::PauseSelected()
{
	ActionOnSelectedItem(pause);
}

void QTorrentDisplayModel::ResumeSelected()
{
	ActionOnSelectedItem(resume);
}

