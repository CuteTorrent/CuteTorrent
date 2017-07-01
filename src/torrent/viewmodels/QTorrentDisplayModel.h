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
#include <QVector>

#include "Torrent.h"
#include "TorrentManager.h"

class CuteTorrentMainWindow;
class Torrent;
class TorrentManager;
class QTorrentFilterProxyModel;
class QTreeView;
class StyleEngene;
#define EXTENDED_COLUMN_COUNT 11
#define COMPACT_COLUMN_COUNT 1

class QTorrentDisplayModel: public QAbstractItemModel
{
public:
	enum ViewMode
	{
		Compact,
		Extended
	};

private:
	Q_OBJECT
	TorrentStorragePtr m_pTorrentStorrage;
	int auto_id;
	QTreeView* m_pTorrentListView;
	int selectedRow;
	QMenu* menu;
	QMenu* groupsMenu;
	QMenu* queueMenu;
	QMenu* priorityMenu;
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
	QAction* generateMagnet;
	QAction* pauseResumeSeparator;
	QAction* lowPriority;
	QAction* belowAvgPriority;
	QAction* mediumPriority;
	QAction* abowAvgPriority;
	QAction* highPriority;
	TorrentManagerPtr m_pTorrentManager;
	Torrent* m_pCurrentTorrent;
	//QTimer* m_pUpdateTimer;
	QMutex* locker;
	QActionGroup* m_pGroupsActionGroup;
	QActionGroup* m_pPriorityActionGroup;
	QSignalMapper* m_pGroupMapper;
	QSignalMapper* m_pPriorityMapper;
	QTorrentFilterProxyModel* m_pProxyFilterModel;
	QMap<QUuid, QAction*> m_groupsUidToActions;
	ViewMode m_viewMode;
	static QString getFirstAvailibleFile(files_info& filesInfo);
	static bool checkAllMountable(QModelIndexList& indexes);
	static bool checkAllSuperSeed(QModelIndexList& indexes);
	static bool checkAllHasMedia(QModelIndexList& indexes);
	static bool checkAllSequential(QModelIndexList& indexes);
	static bool checkAllQueueable(QModelIndexList& indexes);
	static void checkPausedResumed(QModelIndexList indexes, bool& isAllPaused, bool& isAllResumed, bool& hasPaused, bool& hasResumed);
	static int getCommonPriority(QModelIndexList& indexes);
	void AddGroupsLevel(StyleEngene*, QList<TorrentGroup*>&, QMenu*);
	void setGroupsUnchecked() const;
	int columnCount(const QModelIndex& parent) const override;
public:

	QTorrentDisplayModel(ViewMode, QTreeView*, QTorrentFilterProxyModel*, QObject*);
	void setViewMode(ViewMode);
	ViewMode viewMode();
	~QTorrentDisplayModel();

	enum action
	{
		stop,
		pause,
		resume,
		remove,
		remove_all,
		move_storage,
		set_sequential,
		set_superseed,
		generate_magmet,
		update_trackers,
		rehash,
		change_group,
		queue_up,
		queue_down,
		queue_top,
		queue_bottom
	};

	void ActionOnSelectedItem(action wtf);
	void retranslate() const;
	void setSequentualDL();

	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex& child) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
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
		TorrentRemainingTime,
		TorrentSeeds,
		TorrentPeers
	};

	enum Column
	{
		Name,
		Progress,
		Size,
		TotalDownloaded,
		TotalUploaded,
		DownloadSpeed,
		UploadSpeed,
		Uptime,
		RemainingTime,
		Seeds,
		Peers
	};

	Torrent* GetSelectedTorrent() const;

	signals:
	void initCompleted();
public slots:
	void UpdateMenu();
	void Update(QSet<QString>);
	void UpdateSelectedIndex(const QItemSelection&);
	void contextualMenu(const QPoint&);
	void OpenDirSelected() const;
	void DellTorrentOnly();
	void Rehash();
	void UpdateTrackers();
	void DellAll();
	void MountDT() const;
	void SetPriority(int prio) const;
	void moveStorrage();
	void SetSuperSeed();
	void generateMagnetLink();
	void changeGroup(const QString& group) const;
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

