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
#ifndef _TOR_H
#define _TOR_H
#include <QFileIconProvider>
#include <QHostAddress>
#include <QIcon>
#include <QList>
#include <QString>
#include <QStringList>
#include <QVector>

#include "StaticHelpers.h"
#include "StyleEngene.h"
#include "defs.h"

class StaticHelpers;
class TorrentManager;

class Torrent : public QObject
{
	Q_OBJECT

private:
	QMap<QString, int> trackerPeerCountMap;
	bool m_hasMedia;
	bool m_isPrevSeed;
	torrent_handle m_hTorrent;
	libtorrent::size_type size;
	QIcon icon;
	QString ErrorString;
	bool mountable;
	QString base_suffix;
	QString type;
	QString group;
	QStringList imageFiles;

//	bool prevHaserror,prevIsCompleted;
public :

	Torrent(torrent_handle torrentStatus, QString group);
	~Torrent() {};
	void SetFilePriority(int index, int prioryty);
	QString GetRemainingTime();
	QStringList& GetImageFiles();
	QString GetSuffix();
	QString GetErrorMessage() const;

	QString GetGroup();
	QString GetDwonloadSpeed();
	QString GetUploadSpeed();
	QString GetSavePath();
	QString GetTotalUploaded() const;
	QString GetTotalDownloaded() const;
	QString GetActiveTime();
	QString GetTotalSize() const;
	QString GetSeedString();
	QString GetPeerString();
	QString GetName() const;
	QString GetType();
	QString GetDiscribtion();
	int GetStatus();
	torrent_handle& GetInternalHandle();
	QString GetInfoHash();
	int GetProgress() const;
	int GetPeerCount();
	int GetActivePeerCount();
	QIcon GetMimeTypeIcon() const;
	QIcon GetMimeTypeIcon();
	files_info GetFileDownloadInfo();
	bool isPrevioslySeeded() const
	{
		return m_isPrevSeed;
	}
	void setIsPrevioslySeeded(bool value)
	{
		m_isPrevSeed = value;
	}
	bool isPaused() const;
	bool isSeeding() const;
	bool isStoped() const;
	bool hasMetadata() const;
	bool isDownloading() const;
	bool isDaemonToolsMountable();
	bool isSquential() const;
	bool hasError() const;
	bool isActive() const;
	bool hasMediaFiles();
	int GetPieceCount();
	QVector<int> GetDownloadedPieces();
	QVector<int> GetDownloadingPieces();
	QString GetStatusString() const;
	QString GetProgresString() const;
	QString GetDwonloadSpeed() const;
	QString GetUploadSpeed() const;
	int GetPeerCountForTracker(QString url);
	void RemoveTorrent(bool delfiles = false);
	std::vector<peer_info> GetPeerInfo();
	std::vector<announce_entry> GetTrackerInfo();
	void SetTrackerPeersCnt(QString trackerUrl, int peerCount);
	void announceRehash();
	void updateTrackers();
	void seqensialDownload();
	void pause();
	void stop();
	void resume();
	void MoveStorrage(QString path);
	void AddTracker(QString url);
	void RemoveTracker(QString url);
	void AddPeer(QHostAddress adr, short port);
	void SetUlLimit(int val);
	void SetDlLimit(int val);
	void setGroup(QString group);
	int GetDownloadLimit();
	int GetUploadLimit();
	bool operator< (const Torrent&) const;
	bool operator< (Torrent*);
	void SuperSeed();
	bool isSuperSeed();
	QString generateMagnetLink();
	void RemoveTrackers(QStringList trackers);
	void ReplaceTracker(QString tracker2edit, QString newTrackerUrl);
	void addWebSeed(QString webSeedUrl);
};

//Q_DECLARE_METATYPE(Torrent)
Q_DECLARE_METATYPE(Torrent*)

#endif
