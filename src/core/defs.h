#ifndef _DEFS_H
#define _DEFS_H

#include <QObject>
#include <QString>
#ifdef Q_OS_WIN
#include <WinSock2.h>
#include <windows.h>
#endif
#include <QMetaType>
#include "libtorrent/config.hpp"
#include "libtorrent/file_storage.hpp"
#include "libtorrent/torrent_handle.hpp"
using namespace libtorrent;

struct opentorrent_info
{
	QString name, describtion, baseSuffix, infoHash;
	boost::shared_ptr<libtorrent::torrent_info const> torrentInfo;
	boost::int64_t size;

};

struct openmagnet_info : opentorrent_info
{
	QString link;
	libtorrent::torrent_handle handle;
};

Q_DECLARE_METATYPE(openmagnet_info)
enum FilterType
{
	GROUP_FILTER_TYPE,
	SEARCH,
	RSS_FEED,
	TORRENT,
	NONE
};

Q_DECLARE_METATYPE(FilterType)

struct torrent_creation_info
{
	std::string path, filter, describtion;
	std::vector<std::string> web_seeds;
	std::vector<std::string> trackers;
	quint64 pice_size;
	bool _private, startSeed, saveFileOrder;
};

struct files_info
{
	QString infoHash;
	file_storage storrage;
	std::vector<float> progresses;
	std::vector<int> priorities;
};

#define KbFloat 1024.0
#define KbInt 1024
#endif //_DEFS_H


