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
#include <libtorrent/gzip.hpp>
#include <QApplication>
#include <libtorrent/upnp.hpp>
#include <libtorrent/i2p_stream.hpp>
#include <libtorrent/socks5_stream.hpp>
#include "StyleEngene.h"
#include <zlib.h>
#include <float.h>
QString StaticHelpers::toKbMbGb(size_type size, bool isSpped)
{
	float val = size;
	char* SizeSuffix[] =
	{
		QT_TRANSLATE_NOOP("Torrent", " B"),
		QT_TRANSLATE_NOOP("Torrent", " Kb"),
		QT_TRANSLATE_NOOP("Torrent", " Mb"),
		QT_TRANSLATE_NOOP("Torrent", " Gb"),
		QT_TRANSLATE_NOOP("Torrent", " Tb"),
		QT_TRANSLATE_NOOP("Torrent", " Pb"),
		QT_TRANSLATE_NOOP("Torrent", " Eb"),
		QT_TRANSLATE_NOOP("Torrent", " Zb")
	};
	char* SpeedSuffix[] =
	{
		QT_TRANSLATE_NOOP("Torrent", " B\\s"),
		QT_TRANSLATE_NOOP("Torrent", " Kb\\s"),
		QT_TRANSLATE_NOOP("Torrent", " Mb\\s"),
		QT_TRANSLATE_NOOP("Torrent", " Gb\\s"),
		QT_TRANSLATE_NOOP("Torrent", " Tb\\s"),
		QT_TRANSLATE_NOOP("Torrent", " Pb\\s"),
		QT_TRANSLATE_NOOP("Torrent", " Eb\\s"),
		QT_TRANSLATE_NOOP("Torrent", " Zb\\s")
	};
	int i = 0;
	float dblSByte = val;

	if (size > KbInt)
	{
		for (i; size_type(val / KbInt) > 0; i++, val /= KbInt)
		{
			dblSByte = val / KbFloat;
		}
	}

	float fractpart, intpart;
	fractpart = modff(dblSByte, &intpart);
	QString str;

	if (fractpart < FLT_EPSILON)
	{
		str = QString::number(int(dblSByte));
	}
	else
	{
		str = QString::number(dblSByte, 'f', i == 0 ? 0 : 2);
	}

	if (isSpped)
	{
		str.append(qApp->translate("Torrent", SpeedSuffix[i]));
	}
	else
	{
		str.append(qApp->translate("Torrent", SizeSuffix[i]));
	}

	return str;
}

QString StaticHelpers::filePriorityToString(int priority)
{
	static char* priority_str[] = { QT_TRANSLATE_NOOP("FileViewModel", "FILETAB_PRIORITY_ZERO"),
	                                QT_TRANSLATE_NOOP("FileViewModel", "FILETAB_PRIORITY_LOW"),
	                                QT_TRANSLATE_NOOP("FileViewModel", "FILETAB_PRIORITY_MEDIUM"),
	                                QT_TRANSLATE_NOOP("FileViewModel", "FILETAB_PRIORITY_HIGH")
	                              };

	switch (priority)
	{
		case 0:
		{
			return qApp->translate("FileViewModel", priority_str[0]);
		}

		case 1:
		case 2:
		{
			return qApp->translate("FileViewModel", priority_str[1]);
		}

		case 3:
		case 4:
		case 5:
		{
			return qApp->translate("FileViewModel", priority_str[2]);
		}

		case 6:
		case 7:
		{
			return qApp->translate("FileViewModel", priority_str[3]);
		}

		default:
		{
			return "";
		}
	}
}

QString StaticHelpers::toTimeString(int seconds)
{
	if(seconds < 0)
	{
		return "";
	}

	int min, hour, day, Week;
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
		result.append(qApp->translate("DateTime", "%1 WEEK ").arg(Week));
	}

	if(day > 0)
	{
		result.append(qApp->translate("DateTime", "%1 DAY ").arg(day));
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

		case SchedulerTask::UNKNOWN:
			break;

		default:
			break;
	}

	return "";
}

QString StaticHelpers::GetBaseSuffix(const file_storage& storrage)
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


QString StaticHelpers::translateLibTorrentError(error_code const& ec)
{
#if LIBTORRENT_VERSION_NUM >= 10000

	if (ec.category() == get_libtorrent_category())
	{
		return translateSessionError(ec);
	}

	if (ec.category() == get_bdecode_category())
	{
		return translateBEncodeError(ec);
	}

	if (ec.category() == get_gzip_category())
	{
		return translateGzipError(ec);
	}

	if (ec.category() == get_i2p_category())
	{
		return translateI2PError(ec);
	}

	if (ec.category() == get_socks_category())
	{
		return translateSocksError(ec);
	}

	if (ec.category() == get_upnp_category())
	{
		return translateUpnpError(ec);
	}

#endif
	return QString::fromLocal8Bit(ec.message().c_str());
}

QString StaticHelpers::translateSessionError(error_code const& ec)
{
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
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid action field in udp tracker response")
	};
	return translateError(ec, const_cast<char**>(msgs), sizeof(msgs) / sizeof(msgs[0]));
}

QString StaticHelpers::translateBEncodeError(error_code const& ec)
{
	static char const* msgs[] =
	{
		QT_TRANSLATE_NOOP("ErrorMsg", "no error"),
		QT_TRANSLATE_NOOP("ErrorMsg", "expected string in bencoded string"),
		QT_TRANSLATE_NOOP("ErrorMsg", "expected colon in bencoded string"),
		QT_TRANSLATE_NOOP("ErrorMsg", "unexpected end of file in bencoded string"),
		QT_TRANSLATE_NOOP("ErrorMsg", "expected value (list, dict, int or string) in bencoded string"),
		QT_TRANSLATE_NOOP("ErrorMsg", "bencoded nesting depth exceeded"),
		QT_TRANSLATE_NOOP("ErrorMsg", "bencoded item count limit exceeded"),
		QT_TRANSLATE_NOOP("ErrorMsg", "integer overflow")
	};
	return translateError(ec, const_cast<char**>(msgs), sizeof(msgs) / sizeof(msgs[0]));
}

QString StaticHelpers::translateGzipError(error_code const& ec)
{
	static char const* msgs[] =
	{
		QT_TRANSLATE_NOOP("ErrorMsg", "no error"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid gzip header"),
		QT_TRANSLATE_NOOP("ErrorMsg", "inflated data too large"),
		QT_TRANSLATE_NOOP("ErrorMsg", "available inflate data did not terminate"),
		QT_TRANSLATE_NOOP("ErrorMsg", "output space exhausted before completing inflate"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid block type (type == 3)"),
		QT_TRANSLATE_NOOP("ErrorMsg", "stored block length did not match one's complement"),
		QT_TRANSLATE_NOOP("ErrorMsg", "dynamic block code description: too many length or distance codes"),
		QT_TRANSLATE_NOOP("ErrorMsg", "dynamic block code description: code lengths codes incomplete"),
		QT_TRANSLATE_NOOP("ErrorMsg", "dynamic block code description: repeat lengths with no first length"),
		QT_TRANSLATE_NOOP("ErrorMsg", "dynamic block code description: repeat more than specified lengths"),
		QT_TRANSLATE_NOOP("ErrorMsg", "dynamic block code description: invalid literal/length code lengths"),
		QT_TRANSLATE_NOOP("ErrorMsg", "dynamic block code description: invalid distance code lengths"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid literal/length or distance code in fixed or dynamic block"),
		QT_TRANSLATE_NOOP("ErrorMsg", "distance is too far back in fixed or dynamic block"),
		QT_TRANSLATE_NOOP("ErrorMsg", "unknown gzip error")
	};
	return translateError(ec, const_cast<char**>(msgs), sizeof(msgs) / sizeof(msgs[0]));
}

QString StaticHelpers::translateI2PError(error_code const& ec)
{
	static char const* msgs[] =
	{
		QT_TRANSLATE_NOOP("ErrorMsg", "no error"),
		QT_TRANSLATE_NOOP("ErrorMsg", "parse failed"),
		QT_TRANSLATE_NOOP("ErrorMsg", "cannot reach peer"),
		QT_TRANSLATE_NOOP("ErrorMsg", "i2p error"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid key"),
		QT_TRANSLATE_NOOP("ErrorMsg", "invalid id"),
		QT_TRANSLATE_NOOP("ErrorMsg", "timeout"),
		QT_TRANSLATE_NOOP("ErrorMsg", "key not found"),
		QT_TRANSLATE_NOOP("ErrorMsg", "duplicated id")
	};
	return translateError(ec, const_cast<char**>(msgs), sizeof(msgs) / sizeof(msgs[0]));
}

QString StaticHelpers::translateSocksError(error_code const& ec)
{
	static char const* msgs[] =
	{
		QT_TRANSLATE_NOOP("ErrorMsg", "SOCKS no error"),
		QT_TRANSLATE_NOOP("ErrorMsg", "SOCKS unsupported version"),
		QT_TRANSLATE_NOOP("ErrorMsg", "SOCKS unsupported authentication method"),
		QT_TRANSLATE_NOOP("ErrorMsg", "SOCKS unsupported authentication version"),
		QT_TRANSLATE_NOOP("ErrorMsg", "SOCKS authentication error"),
		QT_TRANSLATE_NOOP("ErrorMsg", "SOCKS username required"),
		QT_TRANSLATE_NOOP("ErrorMsg", "SOCKS general failure"),
		QT_TRANSLATE_NOOP("ErrorMsg", "SOCKS command not supported"),
		QT_TRANSLATE_NOOP("ErrorMsg", "SOCKS no identd running"),
		QT_TRANSLATE_NOOP("ErrorMsg", "SOCKS identd could not identify username")

	};
	return translateError(ec, const_cast<char**>(msgs), sizeof(msgs) / sizeof(msgs[0]));
}
struct error_code_t
{
	int code;
	char const* msg;
};

error_code_t error_codes[] =
{
	{ 0, QT_TRANSLATE_NOOP("ErrorMsg", "no error") }
	, { 402, QT_TRANSLATE_NOOP("ErrorMsg", "Invalid Arguments") }
	, { 501, QT_TRANSLATE_NOOP("ErrorMsg", "Action Failed") }
	, { 714, QT_TRANSLATE_NOOP("ErrorMsg", "The specified value does not exist in the array") }
	, { 715, QT_TRANSLATE_NOOP("ErrorMsg", "The source IP address cannot be wild-carded") }
	, { 716, QT_TRANSLATE_NOOP("ErrorMsg", "The external port cannot be wild-carded") }
	, {
		718, QT_TRANSLATE_NOOP("ErrorMsg", "The port mapping entry specified conflicts with "
		"a mapping assigned previously to another client")
	}
	, { 724, QT_TRANSLATE_NOOP("ErrorMsg", "Internal and External port values must be the same") }
	, {
		725, QT_TRANSLATE_NOOP("ErrorMsg", "The NAT implementation only supports permanent "
		"lease times on port mappings")
	}
	, {
		726, QT_TRANSLATE_NOOP("ErrorMsg", "RemoteHost must be a wildcard and cannot be a "
		"specific IP address or DNS name")
	}
	, { 727, QT_TRANSLATE_NOOP("ErrorMsg", "ExternalPort must be a wildcard and cannot be a specific port ") }
};
QString StaticHelpers::translateUpnpError(error_code const& ec)
{
	int ev = ec.value();
	int num_errors = sizeof(error_codes) / sizeof(error_codes[0]);
	error_code_t* end = error_codes + num_errors;
	error_code_t tmp = { ev, 0 };
	error_code_t* e = std::lower_bound(error_codes, end, tmp
	                                   , boost::bind(&error_code_t::code, _1) < boost::bind(&error_code_t::code, _2));

	if (e != end && e->code == ev)
	{
		return e->msg;
	}

	char msg[500];
	snprintf(msg, sizeof(msg), "unknown UPnP error (%d)", ev);
	return msg;
}

QString StaticHelpers::translateError(error_code const& ec, char* msgs[], int msgs_len)
{
	int code = ec.value();

	if (code < 0 || code >= msgs_len)
	{
		return QString::fromStdString(ec.message());
	}

	return qApp->translate("ErrorMsg", msgs[code]);
}

#ifdef Q_WS_X11
void StaticHelpers::OpenFolderNautilus(QString& file)
{
	QProcess* nautilus = new QProcess();
	QStringList arguments;
	arguments << "--browser" << file;
	nautilus->startDetached("nautilus", arguments);
	nautilus->deleteLater();
}
#endif

#ifdef Q_WS_WIN
#include <Shlobj.h>
void StaticHelpers::OpenFileInExplorer(QString& file)
{
	file = QDir::toNativeSeparators(file);
	wchar_t wPath[MAX_PATH] = { 0 };
	file.toWCharArray(wPath);
	ITEMIDLIST* pItem = ILCreateFromPathW(wPath);
	SHOpenFolderAndSelectItems(pItem, 0, NULL, 0);
	ILFree(pItem);
}

typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);

LPFN_ISWOW64PROCESS fnIsWow64Process;

bool StaticHelpers::IsWow64()
{
	BOOL bIsWow64 = FALSE;
	//IsWow64Process is not available on all supported versions of Windows.
	//Use GetModuleHandle to get a handle to the DLL that contains the function
	//and GetProcAddress to get a pointer to the function if available.
	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
	                       GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

	if (NULL != fnIsWow64Process)
	{
		if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
		{
			//handle error
		}
	}

	return bIsWow64;
}
#endif

NetworkDiskCache* StaticHelpers::GetGLobalWebCache()
{
	if (m_pDiskCache == NULL)
	{
		m_pDiskCache = new NetworkDiskCache();
		m_pDiskCache->setCacheDirectory(QDesktopServices::storageLocation(QDesktopServices::CacheLocation) + "/WebCache");
		m_pDiskCache->setMaximumCacheSize(50 * KbInt * KbInt);
		qDebug() << "NetworkDiskCache  cache path:" << m_pDiskCache->cacheDirectory() << " max size:" << m_pDiskCache->maximumCacheSize() / KbInt / KbInt << "MB";
	}

	return m_pDiskCache;
}

QByteArray StaticHelpers::gUncompress(QByteArray data)
{
	if (data.size() <= 4)
	{
		qWarning("gUncompress: Input data is truncated");
		return QByteArray();
	}

	QByteArray result;
	int ret;
	z_stream strm;
	static const int CHUNK_SIZE = 1024;
	char out[CHUNK_SIZE];
	/* allocate inflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = data.size();
	strm.next_in = (Bytef*)(data.data());
	ret = inflateInit2(&strm, 15 + 32); // gzip decoding

	if (ret != Z_OK)
	{
		return QByteArray();
	}

	// run inflate()
	do
	{
		strm.avail_out = CHUNK_SIZE;
		strm.next_out = (Bytef*)(out);
		ret = inflate(&strm, Z_NO_FLUSH);
		Q_ASSERT(ret != Z_STREAM_ERROR);  // state not clobbered

		switch (ret)
		{
			case Z_NEED_DICT:
				ret = Z_DATA_ERROR;     // and fall through

			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				(void)inflateEnd(&strm);
				return QByteArray();
		}

		result.append(out, CHUNK_SIZE - strm.avail_out);
	}
	while (strm.avail_out == 0);

	// clean up and return
	inflateEnd(&strm);
	return result;
}

QTime StaticHelpers::SecsToQTime(int secs)
{
	int h = 0, m = 0, s = 0;
	h = secs / 3600;
	secs -= h * 3600;
	m = secs / 60;
	secs -= m * 60;
	s = secs;
	return QTime(h, m, s);
}

int StaticHelpers::QTimeToSecs(const QTime& time)
{
	return QTime(0, 0, 0).secsTo(time);
}

NetworkDiskCache* StaticHelpers::m_pDiskCache = NULL;


