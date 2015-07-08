#pragma once
#include "Torrent.h"
class Torrent;
#include "defs.h"
#include "Singleton.h"
class TorrentStorrage : public QList<Torrent*>, public Singleton<TorrentStorrage>
{
//	Q_OBJECT
	friend class Singleton<TorrentStorrage>;
public:
	void append(Torrent*);
	void remove(Torrent* torrent);
	void remove(QString);
	void sort();
	bool hasTorrent(Torrent*);
	bool hasTorrent(QString);
	Torrent* getTorrent(QString infoHash);
	void clear();
	Torrent* operator [](QString index);
	~TorrentStorrage(void);
protected:
	TorrentStorrage(QObject* parrent = NULL);
	
private:
	QMap<QString, Torrent*> m_torrentsMap;
	QMutex* m_pMapSynkMutex;
	FilterType m_filterType;
	QString m_groupFilter;
};


//Q_DECLARE_METATYPE(TorrentStorrage::TorrentFilterType)
