#pragma once
#include <QObject>
#include <QHostAddress>
#include "TorrentManager.h"
class PeerInfo
{

public:
	QString peer_id;
	QHostAddress ip;
	int port ;
	QString getID();
	libtorrent::entry toEntry(bool no_peer_id, bool compact);
};
class AnnounceRequest
{

public:
	QString info_hash;
	QString peer_id;
	QHostAddress ip;
	int port ;
	int uploaded ;
	int downloaded ;
	int left;
	QString event ;
	int numwant ;
	bool no_peer_id ;
	bool compact ;
	PeerInfo& getPeerInfo();
};
