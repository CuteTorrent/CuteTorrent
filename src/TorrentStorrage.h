#pragma once
#include <QObject>
#include "Torrent.h"
#include <QHash>
#include <QString>
#include <QMutex>
#include <QTimer>
class Torrent;
#include "defs.h"
class TorrentStorrage :
	public QObject
{
	Q_OBJECT

public:
	enum TorrentFilterType
	{
		ACTIVE,
		NOT_ACTIVE,
		SEEDING,
		DOWNLOADING,
		COMPLETED,
		NONE
	};


	void setTorrentFilter(TorrentFilterType filter);
	void setGroupFilter(QString filter);
	static TorrentStorrage* getInstance();
	static void freeInstance();
	QList<Torrent*>::iterator begin();
	QList<Torrent*>::iterator end();
	void append(Torrent*);
	void remove(Torrent*);
	void remove(QString);
	void sort();
	bool hasTorrent(Torrent*);
	bool hasTorrent(QString);
	Torrent* at(int index);
	Torrent* getTorrent(QString infoHash);
	void clear();
	int count();
	Torrent* operator [](int index);
	Torrent* operator [](QString index);
protected:
	TorrentStorrage(QObject* parrent = NULL);
	~TorrentStorrage(void);
private:
	static TorrentStorrage* m_pInstance;
	static int m_nInstanceCount;
	QMap<QString, Torrent*> m_torrentsMap;
	QVector<QMap<QString, Torrent*>::Iterator> m_torrents;
	QVector<QMap<QString, Torrent*>::Iterator> m_filteredTorrents;
	QMutex* locker;
	FilterType m_filterType;
	TorrentFilterType m_torrentFilter;
	QString m_groupFilter;
	QTimer* timer;
	void filterByGroup();
private slots:
	void filterData();
};

Q_DECLARE_METATYPE(TorrentStorrage::TorrentFilterType)