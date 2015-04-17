#ifndef _DEFS_H
#define _DEFS_H


#include "libtorrent/config.hpp"
#include "libtorrent/file_storage.hpp"
#include "libtorrent/torrent_handle.hpp"
#include <libtorrent/size_type.hpp>
using namespace libtorrent;
struct opentorrent_info
{
	QString name, describtion, baseSuffix, infoHash;
	libtorrent::file_storage files;
	libtorrent::size_type size;
};
struct openmagnet_info
{
	QString name, describtion, baseSuffix, link, infoHash;
	libtorrent::torrent_handle handle;
	libtorrent::file_storage files;
	libtorrent::size_type size;
};
Q_DECLARE_METATYPE(openmagnet_info);
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
#endif _DEFS_H
