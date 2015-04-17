#pragma once
#include "Torrent.h"
#include <collections/ObservableList.h>
class Torrent;
#include "defs.h"
class TorrentStorrage : public ObservableList<Torrent*>
{
	Q_OBJECT

public:
	static TorrentStorrage* getInstance();
	static void freeInstance();
	void append(Torrent*);
	void remove(Torrent* torrent);
	void remove(QString);
	void sort();
	bool hasTorrent(Torrent*);
	bool hasTorrent(QString);
	Torrent* getTorrent(QString infoHash);
	void clear();
	Torrent* operator [](QString index);
protected:
	TorrentStorrage(QObject* parrent = nullptr);
	~TorrentStorrage(void);
private:
	static TorrentStorrage* m_pInstance;
	static int m_nInstanceCount;
	QMap<QString, Torrent*> m_torrentsMap;
	QMutex* m_pMapSynkMutex;
	FilterType m_filterType;
	QString m_groupFilter;
};

//Q_DECLARE_METATYPE(TorrentStorrage::TorrentFilterType)