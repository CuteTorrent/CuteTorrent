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
#include "StaticHelpers.h"
#include <QDebug>


QString StaticHelpers::toKbMbGb(libtorrent::size_type size)
{
	float val = size;
	char* Suffix[] = { " B\0", " KB\0", " MB\0", " GB\0", " TB\0", " PB\0", " EB\0" , " ZB\0" };
	int i = 0;
	float dblSByte = val;

	if(size > KbInt)
		for(i ; (libtorrent::size_type)(val / KbInt) > 0; i++, val /= KbInt)
		{
			dblSByte = val / KbFloat;
		}

	QString str = QString::number(dblSByte, 'f', i == 0 ? 0 : 2);
	str.append(Suffix[i]);
	return str;
}

void StaticHelpers::dellDir(QString dirName)
{
	try
	{
		error_code ec;
		remove_all(dirName.toStdString(), ec);
	}
	catch(...)
	{
		qDebug() << " exception caught int deldir";
	}
}
QString StaticHelpers::filePriorityToString(int priority)
{
	static char* priority_str[] = { QT_TRANSLATE_NOOP("FileViewModel", "FILETAB_PRIORITY_ZERO"),
	                                QT_TRANSLATE_NOOP("FileViewModel", "FILETAB_PRIORITY_LOW"),
	                                QT_TRANSLATE_NOOP("FileViewModel", "FILETAB_PRIORITY_MEDIUM"),
	                                QT_TRANSLATE_NOOP("FileViewModel", "FILETAB_PRIORITY_HIGH")
	                              };

	if (priority == 0)
	{
		return qApp->translate("FileViewModel", priority_str[0]);
	}

	if (priority < 3)
	{
		return qApp->translate("FileViewModel", priority_str[1]);
	}

	if (priority >= 3 && priority < 6)
	{
		return qApp->translate("FileViewModel", priority_str[2]);
	}

	if (priority >= 6)
	{
		return qApp->translate("FileViewModel", priority_str[3]);
	}

	return "";
}

QString StaticHelpers::toTimeString(int seconds)
{
	if(seconds < 0)
	{
		return "";
	}

	int min = 0, hour = 0, day = 0, Week = 0;
	Week = seconds / (7 * 24 * 60 * 60);
	seconds -= Week * (7 * 24 * 60 * 60);
	day = seconds / (24 * 60 * 60);
	seconds -= day * (24 * 60 * 60);
	hour = seconds / (60 * 60);
	seconds -= hour * 60 * 60;
	min = seconds / 60;
	seconds -= min * 60;
	QString result, tmp;

	if(Week > 0)
	{
		result.append(QObject::tr("%1 WEEK ").arg(Week));
	}

	if(day > 0)
	{
		result.append(QObject::tr("%1 DAY ").arg(day));
	}

	if(hour >= 0)
	{
		result.append(tmp.sprintf("%02d:", hour));
	}

	if(min >= 0)
	{
		result.append(tmp.sprintf("%02d:", min));
	}

	result.append(tmp.sprintf("%02d", seconds));
	return result;
}

QString StaticHelpers::SchedulerTypeToString(SchedulerTask::TaskType type)
{
	switch(type)
	{
		case SchedulerTask::START_ALL :
			return "START_ALL";

		case SchedulerTask::PAUSE_ALL:
			return "PAUSE_ALL";

		case SchedulerTask::LIMIT_UPLOAD:
			return "LIMIT_UL";

		case SchedulerTask::LIMIT_DOWNLOAD:
			return "LIMIT_DL";
	}

	return "";
}

QString StaticHelpers::GetBaseSuffix(const libtorrent::file_storage& storrage)
{
	QString base_suffix;
	int maxSuffix = 0;
	QMap<QString, int> suffixesCount;

	for(int i = 0; i < storrage.num_files(); i++)
	{
		QFileInfo curfile(QString::fromUtf8(storrage.file_path(i).c_str()));

		if(StyleEngene::suffixes[StyleEngene::DISK].contains(curfile.suffix()))
		{
			base_suffix = curfile.suffix();
			break;
		}

		if(StyleEngene::suffixes[StyleEngene::VIDEO].contains(curfile.suffix()))
		{
			base_suffix = curfile.suffix();
			break;
		}

		if(!suffixesCount.contains(curfile.suffix()))
		{
			suffixesCount.insert(curfile.suffix(), 1);
		}
		else
		{
			suffixesCount[curfile.suffix()]++;
		}

		if(suffixesCount[curfile.suffix()] > maxSuffix)
		{
			maxSuffix = suffixesCount[curfile.suffix()];
			base_suffix = curfile.suffix();
		}
	}

	return base_suffix;
}

QString StaticHelpers::translateLibTorrentError(boost::system::error_code const& ec)
{
	if (ec.category() != libtorrent::get_libtorrent_category())
	{
		return QString::fromLocal8Bit(ec.message().c_str());
	}

	// the error is a libtorrent error
	int code = ec.value();
	static char const* msgs[] =
	{
		QT_TRANSLATE_NOOP("ErrorMsg", "no error"),
		QT_TRANSLATE_NOOP("ErrorMsg", "torrent file collides with file from another torrent"),
		QT_TRANSLATE_NOOP("ErrorMsg", "hash check failed"),
		QT_TRANSLATE_NOOP("ErrorMsg", "torrent file is not a dictionary"),
		QT_TRANSLATE_NOOP("ErrorMsg", "missing or invalid 'info' section in torrent file"),
		QT_TRANSLATE_NOOP("ErrorMsg", "'info' entry is not a dictionary"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid or missing 'piece length' entry in torrent file"),
		QT_TRANSLATE_NOOP("ErrorMsg", "missing name in torrent file"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid 'name' of torrent (possible exploit attempt)"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid length of torrent"),
		QT_TRANSLATE_NOOP("ErrorMsg", "failed to parse files from torrent file"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid or missing 'pieces' entry in torrent file"),
		QT_TRANSLATE_NOOP("ErrorMsg", "incorrect number of piece hashes in torrent file"),
		QT_TRANSLATE_NOOP("ErrorMsg", "too many pieces in torrent"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid metadata received from swarm"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid bencoding"),
		QT_TRANSLATE_NOOP("ErrorMsg", "no files in torrent"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid escaped string"),
		QT_TRANSLATE_NOOP("ErrorMsg", "session is closing"),
		QT_TRANSLATE_NOOP("ErrorMsg", "torrent already exists in session"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid torrent handle used"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid type requested from entry"),
		QT_TRANSLATE_NOOP("ErrorMsg", "missing info-hash from URI"),
		QT_TRANSLATE_NOOP("ErrorMsg", "file too short"),
		QT_TRANSLATE_NOOP("ErrorMsg", "unsupported URL protocol"),
		QT_TRANSLATE_NOOP("ErrorMsg", "failed to parse URL"),
		QT_TRANSLATE_NOOP("ErrorMsg", "peer sent 0 length piece"),
		QT_TRANSLATE_NOOP("ErrorMsg", "parse failed"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid file format tag"),
		QT_TRANSLATE_NOOP("ErrorMsg", "missing info-hash"),
		QT_TRANSLATE_NOOP("ErrorMsg", "mismatching info-hash"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid hostname"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid port"),
		QT_TRANSLATE_NOOP("ErrorMsg", "port blocked by port-filter"),
		QT_TRANSLATE_NOOP("ErrorMsg", "expected closing ] for address"),
		QT_TRANSLATE_NOOP("ErrorMsg", "destructing torrent"),
		QT_TRANSLATE_NOOP("ErrorMsg", "timed out"),
		QT_TRANSLATE_NOOP("ErrorMsg", "upload to upload connection"),
		QT_TRANSLATE_NOOP("ErrorMsg", "uninteresting upload-only peer"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid info-hash"),
		QT_TRANSLATE_NOOP("ErrorMsg", "torrent paused"),
		QT_TRANSLATE_NOOP("ErrorMsg", "'have'-message with higher index than the number of pieces"),
		QT_TRANSLATE_NOOP("ErrorMsg", "bitfield of invalid size"),
		QT_TRANSLATE_NOOP("ErrorMsg", "too many piece requests while choked"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid piece packet"),
		QT_TRANSLATE_NOOP("ErrorMsg", "out of memory"),
		QT_TRANSLATE_NOOP("ErrorMsg", "torrent aborted"),
		QT_TRANSLATE_NOOP("ErrorMsg", "connected to ourselves"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid piece size"),
		QT_TRANSLATE_NOOP("ErrorMsg", "timed out: no interest"),
		QT_TRANSLATE_NOOP("ErrorMsg", "timed out: inactivity"),
		QT_TRANSLATE_NOOP("ErrorMsg", "timed out: no handshake"),
		QT_TRANSLATE_NOOP("ErrorMsg", "timed out: no request"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid choke message"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid unchoke message"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid interested message"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid not-interested message"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid request message"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid hash list"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid hash piece message"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid cancel message"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid dht-port message"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid suggest piece message"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid have-all message"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid have-none message"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid reject message"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid allow-fast message"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid extended message"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid message"),
		QT_TRANSLATE_NOOP("ErrorMsg", "sync hash not found"),
		QT_TRANSLATE_NOOP("ErrorMsg", "unable to verify encryption constant"),
		QT_TRANSLATE_NOOP("ErrorMsg", "plaintext mode not provided"),
		QT_TRANSLATE_NOOP("ErrorMsg", "rc4 mode not provided"),
		QT_TRANSLATE_NOOP("ErrorMsg", "unsupported encryption mode"),
		QT_TRANSLATE_NOOP("ErrorMsg", "peer selected unsupported encryption mode"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid encryption pad size"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid encryption handshake"),
		QT_TRANSLATE_NOOP("ErrorMsg", "incoming encrypted connections disabled"),
		QT_TRANSLATE_NOOP("ErrorMsg", "incoming regular connections disabled"),
		QT_TRANSLATE_NOOP("ErrorMsg", "duplicate peer-id"),
		QT_TRANSLATE_NOOP("ErrorMsg", "torrent removed"),
		QT_TRANSLATE_NOOP("ErrorMsg", "packet too large"),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", "HTTP error"),
		QT_TRANSLATE_NOOP("ErrorMsg", "missing location header"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid redirection"),
		QT_TRANSLATE_NOOP("ErrorMsg", "redirecting"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid HTTP range"),
		QT_TRANSLATE_NOOP("ErrorMsg", "missing content-length"),
		QT_TRANSLATE_NOOP("ErrorMsg", "banned by IP filter"),
		QT_TRANSLATE_NOOP("ErrorMsg", "too many connections"),
		QT_TRANSLATE_NOOP("ErrorMsg", "peer banned"),
		QT_TRANSLATE_NOOP("ErrorMsg", "stopping torrent"),
		QT_TRANSLATE_NOOP("ErrorMsg", "too many corrupt pieces"),
		QT_TRANSLATE_NOOP("ErrorMsg", "torrent is not ready to accept peers"),
		QT_TRANSLATE_NOOP("ErrorMsg", "peer is not properly constructed"),
		QT_TRANSLATE_NOOP("ErrorMsg", "session is closing"),
		QT_TRANSLATE_NOOP("ErrorMsg", "optimistic disconnect"),
		QT_TRANSLATE_NOOP("ErrorMsg", "torrent finished"),
		QT_TRANSLATE_NOOP("ErrorMsg", "no router found"),
		QT_TRANSLATE_NOOP("ErrorMsg", "metadata too large"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid metadata request"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid metadata size"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid metadata offset"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid metadata message"),
		QT_TRANSLATE_NOOP("ErrorMsg", "pex message too large"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid pex message"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid lt_tracker message"),
		QT_TRANSLATE_NOOP("ErrorMsg", "pex messages sent too frequent (possible attack)"),
		QT_TRANSLATE_NOOP("ErrorMsg", "torrent has no metadata"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid dont-have message"),
		QT_TRANSLATE_NOOP("ErrorMsg", "SSL connection required"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid SSL certificate"),
		QT_TRANSLATE_NOOP("ErrorMsg", "not an SSL torrent"),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),

		// natpmp errors
		QT_TRANSLATE_NOOP("ErrorMsg", "unsupported protocol version"),
		QT_TRANSLATE_NOOP("ErrorMsg", "not authorized to create port map (enable NAT-PMP on your router)"),
		QT_TRANSLATE_NOOP("ErrorMsg", "network failure"),
		QT_TRANSLATE_NOOP("ErrorMsg", "out of resources"),
		QT_TRANSLATE_NOOP("ErrorMsg", "unsupported opcode"),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),

		// fastresume errors
		QT_TRANSLATE_NOOP("ErrorMsg", "missing or invalid 'file sizes' entry"),
		QT_TRANSLATE_NOOP("ErrorMsg", "no files in resume data"),
		QT_TRANSLATE_NOOP("ErrorMsg", "missing 'slots' and 'pieces' entry"),
		QT_TRANSLATE_NOOP("ErrorMsg", "mismatching number of files"),
		QT_TRANSLATE_NOOP("ErrorMsg", "mismatching file size"),
		QT_TRANSLATE_NOOP("ErrorMsg", "mismatching file timestamp"),
		QT_TRANSLATE_NOOP("ErrorMsg", "not a dictionary"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid 'blocks per piece' entry"),
		QT_TRANSLATE_NOOP("ErrorMsg", "missing slots list"),
		QT_TRANSLATE_NOOP("ErrorMsg", "file has more slots than torrent"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid entry type in slot list"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid piece index in slot list"),
		QT_TRANSLATE_NOOP("ErrorMsg", "pieces needs to be reordered"),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),

		// HTTP errors
		QT_TRANSLATE_NOOP("ErrorMsg", "Invalid HTTP header"),
		QT_TRANSLATE_NOOP("ErrorMsg", "missing Location header in HTTP redirect"),
		QT_TRANSLATE_NOOP("ErrorMsg", "failed to decompress HTTP response"),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),

		// i2p errors
		QT_TRANSLATE_NOOP("ErrorMsg", "no i2p router is set up"),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),

		// tracker errors
		QT_TRANSLATE_NOOP("ErrorMsg", "scrape not available on tracker"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid tracker response"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid peer dictionary entry"),
		QT_TRANSLATE_NOOP("ErrorMsg", "tracker sent a failure message"),
		QT_TRANSLATE_NOOP("ErrorMsg", "missing or invalid 'files' entry"),
		QT_TRANSLATE_NOOP("ErrorMsg", "missing or invalid 'hash' entry"),
		QT_TRANSLATE_NOOP("ErrorMsg", "missing or invalid 'peers' and 'peers6' entry"),
		QT_TRANSLATE_NOOP("ErrorMsg", "udp tracker response packet has invalid size"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid transaction id in udp tracker response"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid action field in udp tracker response"),

		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),
		QT_TRANSLATE_NOOP("ErrorMsg", ""),

		// bdecode errors
		QT_TRANSLATE_NOOP("ErrorMsg", "expected string in bencoded string"),
		QT_TRANSLATE_NOOP("ErrorMsg", "expected colon in bencoded string"),
		QT_TRANSLATE_NOOP("ErrorMsg", "unexpected end of file in bencoded string"),
		QT_TRANSLATE_NOOP("ErrorMsg", "expected value (list, dict, int or string) in bencoded string"),
		QT_TRANSLATE_NOOP("ErrorMsg", "bencoded nesting depth exceeded"),
		QT_TRANSLATE_NOOP("ErrorMsg", "bencoded item count limit exceeded"),
		QT_TRANSLATE_NOOP("ErrorMsg", "integer overflow"),

	};

	if (code < 0 || code >= sizeof(msgs) / sizeof(msgs[0]))
	{
		return QString::fromLocal8Bit(ec.message().c_str());
	}

	return qApp->translate("ErrorMsg", msgs[code]);
}

QString StaticHelpers::CombinePathes(QString path, QString suffix)
{
	return QDir::toNativeSeparators(QDir::cleanPath(path + QDir::separator() + suffix));
}

NetworkDiskCache* StaticHelpers::GetGLobalWebCache()
{
	if (m_pDiskCache == NULL)
	{
		m_pDiskCache = new NetworkDiskCache();
		m_pDiskCache->setCacheDirectory(QDesktopServices::storageLocation(QDesktopServices::CacheLocation) + "/WebCache");
		m_pDiskCache->setMaximumCacheSize(50 * KbInt * KbInt);
		qDebug() << "RssManager  cache path:" << m_pDiskCache->cacheDirectory() << " max size:" << m_pDiskCache->maximumCacheSize() / KbInt / KbInt << "MB";
	}

	return m_pDiskCache;
}

NetworkDiskCache* StaticHelpers::m_pDiskCache = NULL;


