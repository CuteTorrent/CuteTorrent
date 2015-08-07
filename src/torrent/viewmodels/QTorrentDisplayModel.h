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
along with this program.  If not, see .
*/
#ifndef _TOR_DIPL_MODEL_H
#define _TOR_DIPL_MODEL_H
#include <TorrentStorrage.h>
#include <stddef.h>
#include <QAbstractListModel>
#include <QListView>
#include <QMap>
#include <QMenu>
#include <QMutex>
#include <QSet>
#include <QSignalMapper>
#include <QThread>
#include <QTimer>
#include <QVector>

#include "Torrent.h"
#include "TorrentManager.h"

class CuteTorrentMainWindow;
class Torrent;
class TorrentManager;
class QTorrentFilterProxyModel;
class QTreeView;
class QTorrentDisplayModel: public QAbstractListModel
{
	Q_OBJECT
	TorrentStorragePtr m_pTorrentStorrage;
	int auto_id;
	QTreeView* m_pTorrentListView;
	int selectedRow;
	QMenu* menu;
	QMenu* groupsMenu;
	QMenu* queueMenu;
	QAction* queueUp;
	QAction* queueDown;
	QAction* queueTop;
	QAction* queueBottom;
	QAction* pauseTorrent;
	QAction* resumeTorrent;
	QAction* openDir;
	QAction* superSeed;
	QAction* mountDiskImage;
	QAction* HashRecheck;
	QAction* deleteWithFiles;
	QAction* deleteTorrentOnly;
	QAction* setSequentual;
	QAction* updateTrackers;
	QAction* storrageMove;
	QAction* playInMediaPlayer;
	QAction* generateMagnet;
	QAction* pauseResumeSeparator;
	TorrentManagerPtr m_pTorrentManager;
	Torrent* m_pCurrentTorrent;
	QTimer* m_pUpdateTimer;
	QMutex* locker;
	QTorrentFilterProxyModel* m_pProxyFilterModel;
	QString getFirstAvailibleFile(files_info& filesInfo);
	bool checkAllMountable(QModelIndexList& indexes);
	bool checkAllSuperSeed(QModelIndexList& indexes);
	bool checkAllHasMedia(QModelIndexList& indexes);
	bool checkAllSequential(QModelIndexList& indexes);
	bool checkAllQueueable(QModelIndexList& indexes);
	void checkPausedResumed(QModelIndexList indexes, bool& isAllPaused, bool& isAllResumed, bool& hasPaused, bool& hasResumed);
public:
	QTorrentDisplayModel(QTreeView*, QTorrentFilterProxyModel*, QObject*);
	~QTorrentDisplayModel();
	enum action
	{
		stop, pause, resume, remove, remove_all, move_storrage,
		set_sequntial, set_superseed, generate_magmet, update_trackers,
		rehash, change_group, queue_up, queue_down, queue_top, queue_bottom
	};
	void ActionOnSelectedItem(action wtf);
	void retranslate();

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role =	Qt::DisplayRole) const override;
	bool removeRow(const QModelIndex& index, bool delFiles);
	enum Role
	{
		TorrentRole = Qt::UserRole,
		TorrentQueuePosition,
		TorrentName,
		TorrentSize,
		TorrentTotalDownloaded,
		TorrentTotalUploaded,
		TorrentUptime,
		TorrentRemainingTime
	};
	Torrent* GetSelectedTorrent();

signals:
	void initCompleted();
public slots:
	void Update();
	void UpdateSelectedIndex(const QItemSelection&);
	void contextualMenu(const QPoint&);
	void OpenDirSelected();
	void DellTorrentOnly();
	void Rehash();
	void UpdateTrackers();
	void DellAll();
	void MountDT();

	void playInPlayer();
	void setSequentualDL();
	void moveStorrage();
	void SetSuperSeed();
	void initSessionFinished();
	void generateMagnetLink();
	void changeGroup();
	void setupContextMenu();
	void OnTorrentAdded();
	void PauseSelected();
	void ResumeSelected();

	void MoveUpQueue();
	void MoveDownQueue();
	void MoveTopQueue();
	void MoveBottomQueue();
};


#endif
