#include "AnnounceRequest.h"


QString PeerInfo::getID()
{
	return ip.toString() + ":" + QString::number(port);
}

libtorrent::entry PeerInfo::toEntry(bool no_peer_id, bool compact)
{
	libtorrent::entry::dictionary_type peer_map;

	if (!no_peer_id)
	{
		peer_map["id"] = libtorrent::entry(peer_id.toStdString());
	}

	if (!compact)
	{
		peer_map["ip"] = libtorrent::entry(ip.toString().toStdString());
	}
	else
	{
		peer_map["ip"] = libtorrent::entry(ip.toIPv4Address());
	}

	peer_map["port"] = libtorrent::entry(port);
	return libtorrent::entry(peer_map);
}


PeerInfo& AnnounceRequest::getPeerInfo()
{
	PeerInfo* item = new PeerInfo;
	item->ip = ip;
	item->port = port;
	item->peer_id = peer_id;
	return *item;
}

