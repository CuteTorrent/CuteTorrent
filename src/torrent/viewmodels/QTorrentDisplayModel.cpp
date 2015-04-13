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
#include <QDesktopServices>
#include <QDir>
#include <QItemSelectionModel>
#include <QProcess>
#include <QThread>
#include <QUrl>
#include <exception>
#include "MultipleDTDialog.h"
#include "DT_mounter.h"
#include "QApplicationSettings.h"
#include "QTorrentDisplayModel.h"
#include "StyleEngene.h"
#include "Torrent.h"
#include "TorrentManager.h"
#include "TorrentStorrage.h"
#include "VideoPlayer\VideoPlayerWindow.h"
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
	QObject::connect(m_pTorrentManager, SIGNAL(AddTorrentGui(Torrent*)), SLOT(OnAddTorrent()));
	QObject::connect(m_pTorrentManager, SIGNAL(TorrentRemove(QString)), SLOT(Update()));
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
class MountDialogThread : private QThread
{
	MultipleDTDialog* dlg;
public:
	void create(QStringList _files)
	{
		dlg = new MultipleDTDialog(_files);
		start();
	}
protected:
	void run()
	{
		dlg->exec();
		delete dlg;
	}
};

void QTorrentDisplayModel::MountDT()
{
	Torrent* tor = GetSelectedTorrent();

	if(tor != NULL)
	{
		if(tor->isDaemonToolsMountable() && (tor->isSeeding() || tor->isPaused()))
		{
			if(!tor->isPaused())
			{
				tor->pause();
			}

			QStringList images = tor->GetImageFiles();

			if(images.size() > 1)
			{
				MultipleDTDialog dlg(images, m_pTorrentListView);
				dlg.exec();
			}
			else
			{
				DT_mounter::mountImage(images.first());
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
		QString path = QFileInfo(QDir::toNativeSeparators(tor->GetSavePath() + tor->GetName())).absoluteFilePath();
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
#ifdef Q_WS_WIN
		QStringList args;
		args << "/select," << QDir::toNativeSeparators(path);
		QProcess::startDetached("explorer", args);
#endif
	}
}

void QTorrentDisplayModel::contextualMenu(const QPoint& point)
{
	if (m_pTorrentListView->model() != m_pProxyFilterModel)
	{
		return;
	}

	QModelIndex qmi = m_pTorrentListView->indexAt(point);

	if(qmi.isValid())
	{
		Torrent* torrent = qmi.data(TorrentRole).value<Torrent*>();

		if(torrent != NULL)
		{
			if(!torrent->isDaemonToolsMountable() || !torrent->isSeeding())
			{
				DTmount->setEnabled(false);
			}
			else
			{
				DTmount->setEnabled(true);
			}

			if(!torrent->isSuperSeed())
			{
				superSeed->setChecked(false);
			}
			else
			{
				superSeed->setChecked(true);
			}

			if(!torrent->hasMediaFiles())
			{
				PlayInPlayer->setEnabled(false);
			}
			else
			{
				if(!PlayInPlayer->isEnabled())
				{
					PlayInPlayer->setEnabled(true);
				}
			}

			QString group = torrent->GetGroup();
			QList<QAction*> actions = groupsMenu->actions();

			for(int i = 0; i < actions.size(); i++)
			{
				actions[i]->setChecked(actions[i]->text() == group);
			}

			setSequentual->setChecked(torrent->isSquential());
			menu->exec(m_pTorrentListView->mapToGlobal(point));
		}
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
			CurrentTorrent = indexes[0].data(TorrentRole).value<Torrent*>();
		}
		else
		{
			selectedRow = -1;
			CurrentTorrent = NULL;
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
		return m_pTorrentStorrage->count();
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

		return CurrentTorrent;
	}
	catch(std::exception e)
	{
		qDebug() << "Exception QTorrentDisplayModel::GetSelectedTorrent" << e.what();
	}

	return NULL;
}

void QTorrentDisplayModel::ActionOnSelectedItem(action wtf)
{
	if (m_pTorrentListView->model() != m_pProxyFilterModel)
	{
		return;
	}

	try
	{
		QModelIndexList indexes = StaticHelpers::reversed(m_pTorrentListView->selectionModel()->selectedIndexes());

		if(rowCount() == 0)
		{
			return;
		}

		if(selectedRow < 0)
		{
			return;
		}

		if(selectedRow >= rowCount())
		{
			return;
		}

		switch(wtf)
		{
			case stop:
			{
				foreach(QModelIndex index, indexes)
				{
					Torrent* torrent = index.data(TorrentRole).value<Torrent*>();
					torrent->stop();
				}

				break;
			}

			case pause:
			{
				foreach(QModelIndex index, indexes)
				{
					Torrent* torrent = index.data(TorrentRole).value<Torrent*>();
					torrent->pause();
				}

				break;
			}

			case remove:
			{
				bool yesToAll = false;

				foreach(QModelIndex index, indexes)
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
						removeRow(m_pProxyFilterModel->mapToSource(index).row(), false);
					}
				}
			}
			break;

			case remove_all:
			{
				bool yesToAll = false;

				foreach(QModelIndex index, indexes)
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
						removeRow(m_pProxyFilterModel->mapToSource(index).row(), true);
					}
				}

				break;
			}

			case resume:
				foreach(QModelIndex index, indexes)
				{
					Torrent* torrent = index.data(TorrentRole).value<Torrent*>();
					torrent->resume();
				}

				break;

			case rehash:
			{
				foreach(QModelIndex index, indexes)
				{
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
					foreach(QModelIndex index, indexes)
					{
						Torrent* torrent = index.data(TorrentRole).value<Torrent*>();
						torrent->MoveStorrage(path + QDir::separator());
					}
				}

				break;
			}

			case set_sequntial:
			{
				foreach(QModelIndex index, indexes)
				{
					Torrent* torrent = index.data(TorrentRole).value<Torrent*>();
					torrent->seqensialDownload();
				}

				break;
			}

			case set_superseed:
			{
				foreach(QModelIndex index, indexes)
				{
					Torrent* torrent = index.data(TorrentRole).value<Torrent*>();
					torrent->SuperSeed();
				}

				break;
			}

			case update_trackers:
			{
				foreach(QModelIndex index, indexes)
				{
					Torrent* torrent = index.data(TorrentRole).value<Torrent*>();
					torrent->updateTrackers();
				}

				break;
			}

			case generate_magmet:
			{
				QString clipboardData;

				foreach(QModelIndex index, indexes)
				{
					Torrent* torrent = index.data(TorrentRole).value<Torrent*>();
					clipboardData += torrent->generateMagnetLink();
					clipboardData += "\n";
				}

				QApplication::clipboard()->setText(clipboardData);
				break;
			}
		case change_group: break;
		default: break;
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

	m_pTorrentListView->selectionModel()->reset();
	beginRemoveRows(QModelIndex(), row, row);

	if(m_pTorrentStorrage->at(row) == CurrentTorrent)
	{
		CurrentTorrent = nullptr;
	}

	m_pTorrentStorrage->at(row)->RemoveTorrent(delFiles);
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
		m_pTorrentStorrage->at(i)->RemoveTorrent(m_pTorrentManager);
	}

	endRemoveRows();
	return true;
}

QTorrentDisplayModel::~QTorrentDisplayModel()
{
	TorrentManager::freeInstance();
	TorrentStorrage::freeInstance();
}

void QTorrentDisplayModel::retranslate()
{
	openDir->setText(tr("ACTION_OPEN_FOLDER"));
	DTmount->setText(tr("ACTION_DT_MOUNT"));
	DelAll->setText(tr("ACTION_DELETE_ALL"));
	DelTorrentOnly->setText(tr("ACTION_DELETE_TORRENT"));
	HashRecheck->setText(tr("ACTION_REHASH"));
	setSequentual->setText(tr("ACTION_SET_SEQUENTIAL"));
	updateTrackers->setText(tr("ACTION_UPDATE_TRACKERS"));
	MoveStorrage->setText(tr("ACTION_MOVE_STORRAGE"));
	PlayInPlayer->setText(tr("ACTION_PLAY_IN_PLAYER"));
	superSeed->setText(tr("ACTION_SET_SUPERSEED"));
	GenerateMagnet->setText(tr("ACTION_GENERATE_MAGNET"));
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
	try
	{
		VideoPlayerWindow* vpw = new VideoPlayerWindow(m_pTorrentListView);
		vpw->openFile(CurrentTorrent->GetSavePath() + QString::fromStdString(CurrentTorrent->GetFileDownloadInfo().storrage.at(0).path));
		vpw->show();
	}
	catch(...)
	{
		qDebug() << "Exception in QTorrentDisplayModel::playInPlayer";
	}
}

void QTorrentDisplayModel::setupContextMenu()
{
	menu = new QMenu(m_pTorrentListView);
	StyleEngene* style = StyleEngene::getInstance();
	openDir = new QAction(style->getIcon("open_folder"), tr("ACTION_OPEN_FOLDER"), this);
	openDir->setObjectName("ACTION_TORRENTLIST_OPEN_DIR");
	QObject::connect(openDir, SIGNAL(triggered()), this, SLOT(OpenDirSelected()));
	openDir->setShortcut(Qt::Key_Enter);
	menu->addAction(openDir);
	menu->addSeparator();
	DTmount = new QAction(style->getIcon("daemon_tools"), tr("ACTION_DT_MOUNT"), this);
	DTmount->setObjectName("ACTION_TORRENTLIST_DT_MOUNT");
	QObject::connect(DTmount, SIGNAL(triggered()), this, SLOT(MountDT()));
	menu->addAction(DTmount);
	PlayInPlayer = new QAction(style->getIcon("play"), tr("ACTION_PLAY_IN_PLAYER"), this);
	PlayInPlayer->setObjectName("ACTION_TORRENTLIST_PLAY");
	QObject::connect(PlayInPlayer, SIGNAL(triggered()), this, SLOT(playInPlayer()));
	menu->addAction(PlayInPlayer);
	MoveStorrage = new QAction(style->getIcon("move_folder"), tr("ACTION_MOVE_STORRAGE"), this);
	MoveStorrage->setObjectName("ACTION_TORRENTLIST_MOVE_STORRAGE");
	QObject::connect(MoveStorrage, SIGNAL(triggered()), this, SLOT(moveStorrage()));
	menu->addAction(MoveStorrage);
	menu->addSeparator();
	superSeed = new QAction(style->getIcon("super_seed"), tr("ACTION_SET_SUPERSEED"), this);
	superSeed->setObjectName("ACTION_TORRENTLIST_SUPER_SEED");
	superSeed->setCheckable(true);
	QObject::connect(superSeed, SIGNAL(triggered()), this, SLOT(SetSuperSeed()));
	menu->addAction(superSeed);
	HashRecheck = new QAction(style->getIcon("recheck"), tr("ACTION_REHASH"), this);
	HashRecheck->setObjectName("ACTION_TORRENTLIST_RECHECK");
	QObject::connect(HashRecheck, SIGNAL(triggered()), this, SLOT(Rehash()));
	menu->addAction(HashRecheck);
	updateTrackers = new QAction(style->getIcon("update_trackers"), tr("ACTION_UPDATE_TRACKERS"), this);
	updateTrackers->setObjectName("ACTION_TORRENTLIST_UPDATE_TRACKERS");
	QObject::connect(updateTrackers, SIGNAL(triggered()), this, SLOT(UpdateTrackers()));
	menu->addAction(updateTrackers);
	setSequentual = new QAction(style->getIcon("sequential"), tr("ACTION_SET_SEQUENTIAL"), this);
	setSequentual->setObjectName("ACTION_TORRENTLIST_SET_SEQUNTIAL");
	setSequentual->setCheckable(true);
	QObject::connect(setSequentual, SIGNAL(triggered()), this, SLOT(setSequentualDL()));
	menu->addAction(setSequentual);
	GenerateMagnet = new QAction(style->getIcon("magnet"), tr("ACTION_GENERATE_MAGNET"), this);
	GenerateMagnet->setObjectName("ACTION_TORRENTLIST_GENERATE_MAGNET");
	QObject::connect(GenerateMagnet, SIGNAL(triggered()), this, SLOT(generateMagnetLink()));
	menu->addAction(GenerateMagnet);
	menu->addSeparator();
	DelAll = new QAction(style->getIcon("delete"), tr("ACTION_DELETE_ALL"), this);
	DelAll->setObjectName("ACTION_TORRENTLIST_DEL_ALL");
	QObject::connect(DelAll, SIGNAL(triggered()), this, SLOT(DellAll()));
	DelAll->setShortcut(Qt::Key_Shift | Qt::Key_Delete);
	menu->addAction(DelAll);
	DelTorrentOnly = new QAction(style->getIcon("delete"), tr("ACTION_DELETE_TORRENT"), this);
	DelTorrentOnly->setObjectName("ACTION_TORRENTLIST_DEL_TORRENT");
	QObject::connect(DelTorrentOnly, SIGNAL(triggered()), this, SLOT(DellTorrentOnly()));
	DelTorrentOnly->setShortcut(Qt::Key_Delete);
	menu->addAction(DelTorrentOnly);
	groupsMenu = new QMenu(tr("ACTION_CHANGE_GROUP"), menu);
	groupsMenu->setIcon(style->getIcon("groups"));
	QList<GroupForFileFiltering> filters = QApplicationSettings::getInstance()->GetFileFilterGroups();
	QApplicationSettings::FreeInstance();
	QString type;

	for(int i = 0; i < filters.size(); i++)
	{
		QAction* changeGroupAction = new QAction(style->guessMimeIcon(filters[i].Extensions().split('|') [0], type), filters[i].Name(), groupsMenu);
		changeGroupAction->setObjectName(filters[i].Name());
		QObject::connect(changeGroupAction, SIGNAL(triggered()), this, SLOT(changeGroup()));
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
		QAction* senderAct = (QAction*) sender();
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
	m_pTorrentListView->updateGeometry();
}

void QTorrentDisplayModel::OnAddTorrent()
{
	Update();
	m_pTorrentListView->scrollToBottom();
	m_pTorrentListView->setCurrentIndex(index(m_pTorrentStorrage->count() - 1));
}



