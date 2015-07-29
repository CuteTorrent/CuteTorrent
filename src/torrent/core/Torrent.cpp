/*
CuteTorrent BitTorrenttClient with dht support, userfriendly interface
and some additional features which make it more convenient.
Copyright (C) <year>  <name of author>

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
#include <QDateTime>
#include <QDebug>
#include <QFileInfo>
#include <QStringList>
#include <QTimer>

#include "Torrent.h"
#include "TorrentManager.h"
#include <libtorrent/magnet_uri.hpp>
#include <libtorrent/peer_info.hpp>
#include "QApplicationSettings.h"
#include "StyleEngene.h"
#include "StaticHelpers.h"


Torrent::Torrent(torrent_status hTorrent, QString group)
	: QObject(NULL)
	, m_hasMedia(false)
	, m_isPrevSeed(false)
	, size(0)
	, mountable(false)
	, isMovingFileStorrage(false)
{
	m_hTorrent = hTorrent;
	
	if (!m_hTorrent.handle.is_valid())
	{
		qCritical() << "Invalid Torrent recived...";
		return;
	}

	file_storage storrgae =
#if LIBTORRENT_VERSION_NUM >= 10000
		m_hTorrent.handle.torrent_file()->files();
#else
		m_hTorrent.handle.get_torrent_info().files();
#endif
	this->group = group;
	std::vector<size_type> progress;
	m_hTorrent.handle.file_progress(progress);

	for (int i = 0; i < storrgae.num_files(); i++)
	{
		QFileInfo curfile(QString::fromUtf8(storrgae.file_path(i).c_str()));
		QString currentSuffix = curfile.suffix().toLower();
		bool fileReady = storrgae.file_size(i) == progress[i];
		QSet<QString> diskSufixes = StyleEngene::suffixes[StyleEngene::DISK];
		bool mayMount = ((fileReady || m_hTorrent.handle.file_priority(i) > 0) && diskSufixes.contains(currentSuffix));

		if (mayMount)
		{
			std::string save_path =
#if LIBTORRENT_VERSION_NUM >= 10000
				m_hTorrent.save_path;
#else
				m_hTorrent.handle.save_path();
#endif
			imageFiles << QString::fromUtf8(save_path.c_str()) + QString::fromUtf8(storrgae.file_path(i).c_str());
		}

		if (fileReady || m_hTorrent.handle.file_priority(i) > 0)
		{
			QSet<QString> videoSuffix = StyleEngene::suffixes[StyleEngene::VIDEO];

			if (videoSuffix.contains(currentSuffix) || StyleEngene::suffixes[StyleEngene::AUDIO].contains(currentSuffix))
			{
				m_hasMedia = true;
			}

			size += storrgae.file_size(i);
		}
	}

	base_suffix = StaticHelpers::GetBaseSuffix(storrgae);

	if (!base_suffix.isEmpty())
	{
		icon = StyleEngene::getInstance()->guessMimeIcon(base_suffix, type);
	}
	else
	{
		icon = QIcon(":/icons/my-folder.ico");
		type = "folder";
	}
}

bool Torrent::hasError() const
{
	if(m_hTorrent.handle.is_valid())
	{
//       QString errorString="";
		bool hasErr = false;

		if(m_hTorrent.error.length() > 0)
		{
			hasErr = true;
		}

		return hasErr;
	}

	return false;
}

bool Torrent::isActive() const
{
	return (m_hTorrent.download_rate > 2 * KbInt ||
			m_hTorrent.upload_rate > 2 * KbInt ||
			m_hTorrent.state == torrent_status::checking_files);
}
std::vector<peer_info> Torrent::GetPeerInfo()
{
	std::vector<peer_info> pInfo;

	if(m_hTorrent.handle.is_valid())
	{
		m_hTorrent.handle.get_peer_info(pInfo);
	}

	return pInfo;
}
std::vector<announce_entry> Torrent::GetTrackerInfo()
{
	std::vector<announce_entry> trackers;

	if(m_hTorrent.handle.is_valid())
	{
		trackers = m_hTorrent.handle.trackers();
	}

	return trackers;
}
QString Torrent::GetErrorMessage() const
{
	if(m_hTorrent.handle.is_valid())
	{
		QString errorString = "";

		if(m_hTorrent.error.length() > 0)
		{
			errorString.append(QString::fromUtf8(m_hTorrent.error.c_str()));
			errorString.append("\n");
		}

		/*     const std::vector<announce_entry> trackers=cur_torrent.trackers();
		for (int i=0;i<trackers.size();i++)
		  {
		      if (trackers[i].message.length()>0)
		      {
		          errorString.append(QString::fromLocal8Bit(trackers[i].message.c_str()));
		          errorString.append("\n");
		      }
		  }*/
		return errorString;
	}

	return "";
}
bool Torrent::hasMetadata() const
{
	if(m_hTorrent.handle.is_valid())
	{
		return m_hTorrent.has_metadata;
	}

	return false;
}
bool Torrent::isDownloading() const
{
	if(m_hTorrent.handle.is_valid())
	{
		return m_hTorrent.state >= torrent_status::downloading_metadata && m_hTorrent.state <= torrent_status::downloading && !m_hTorrent.paused;
	}

	return false;
}
bool Torrent::isPaused() const
{
	if(m_hTorrent.handle.is_valid())
	{
		return m_hTorrent.paused;
	}

	return false;
}
bool Torrent::isSeeding() const
{
	if(m_hTorrent.handle.is_valid())
	{
		if (m_hTorrent.state == torrent_status::finished)
		{
			return true;
		}
		return m_hTorrent.is_seeding;
	}

	return false;
}
float Torrent::GetProgress() const
{
	if(m_hTorrent.handle.is_valid())
	{
		return m_hTorrent.progress_ppm / 10000.f;
	}

	return 0;
}
QString Torrent::GetSuffix()
{
	return base_suffix;
}

void Torrent::UpdateDiskImageFiles()
{
	imageFiles.clear();

	if(m_hTorrent.handle.is_valid())
	{
		file_storage storrgae =
#if LIBTORRENT_VERSION_NUM >= 10000
			m_hTorrent.handle.torrent_file()->files();
#else
			m_hTorrent.handle.get_torrent_info().files();
#endif
		std::vector<size_type> progress;
		m_hTorrent.handle.file_progress(progress);

		for (int i = 0; i < storrgae.num_files(); ++i)
		{
			QString fileSubPath = QString::fromUtf8(storrgae.file_path(i).c_str());
			QFileInfo curfile(fileSubPath);
			QString currentSuffix = curfile.suffix().toLower();
			bool fileReady = storrgae.file_size(i) == progress[i];
			bool mayMount = (fileReady && StyleEngene::suffixes[StyleEngene::DISK].contains(currentSuffix));

			if (mayMount)
			{
				std::string save_path =
#if LIBTORRENT_VERSION_NUM >= 10000
				    m_hTorrent.save_path;
#else
					m_hTorrent.handle.save_path();
#endif
				imageFiles << StaticHelpers::CombinePathes(QString::fromUtf8(save_path.c_str()), fileSubPath);
			}
		}
	}
}

QStringList& Torrent::GetImageFiles()
{
	UpdateDiskImageFiles();
	return imageFiles;
}

bool Torrent::isDaemonToolsMountable()
{
	UpdateDiskImageFiles();
	return imageFiles.length() > 0 && (isSeeding() || isPaused());
}
QString Torrent::GetProgresString() const
{
	if(m_hTorrent.handle.is_valid())
	{
		return QString::number(m_hTorrent.progress_ppm / 10000.f , 'f', 2) + "%";
	}

	return "";
}
QString Torrent::GetStatusString() const
{
	static const char* state_str[] =
	{
		QT_TR_NOOP("STATE_FILE_CHECKING (q)"),
		QT_TR_NOOP("STATE_FILE_CHECKING"),
		QT_TR_NOOP("STATE_DOWNLOADING_METADATA"),
		QT_TR_NOOP("STATE_DOWNLOADING"),
		QT_TR_NOOP("STATE_FINISHED"),
		QT_TR_NOOP("STATE_SEEDING"),
		QT_TR_NOOP("STATE_ALLOCATING_SPACE"),
		QT_TR_NOOP("STATE_RESUMING"),
		QT_TR_NOOP("STATE_MOVING_FILE_STORRAGE")
	};

	if(m_hTorrent.handle.is_valid())
	{
		int state = m_hTorrent.state;

		if (isMovingFileStorrage)
		{
			state = 8;
		}

		return tr(state_str[state]) + (m_hTorrent.sequential_download ? (" [S]") : "");
	}

	return "";
}/*
QString Torrent::GetHashString() const
{


    try
    {
        return to_hex(cur_torrent.info_hash().to_string()).c_str();
    }
    catch (...)
    {

    }
    return "";
}*/

QString Torrent::GetName() const
{
	if(m_hTorrent.handle.is_valid())
	{
		return QString::fromUtf8(
#if LIBTORRENT_VERSION_NUM >= 10000
					m_hTorrent.name.c_str()
#else
					m_hTorrent.handle.name().c_str()
#endif
		       );
	}

	return "";
}

QString Torrent::GetDwonloadSpeed()
{
	if(m_hTorrent.handle.is_valid())
	{
		return (m_hTorrent.download_rate > 2 * KbInt) ? StaticHelpers::toKbMbGb(m_hTorrent.download_rate, true) : "";
	}

	return "";
}
QString Torrent::GetDwonloadSpeed() const
{
	if(m_hTorrent.handle.is_valid())
	{
		return (m_hTorrent.download_rate > 2 * KbInt) ? StaticHelpers::toKbMbGb(m_hTorrent.download_rate, true) : "";
	}

	return "";
}
QString Torrent::GetUploadSpeed()
{
	if(m_hTorrent.handle.is_valid())
	{
		return (m_hTorrent.upload_rate > 2 * KbInt) ? StaticHelpers::toKbMbGb(m_hTorrent.upload_rate, true) : "";
	}

	return "";
}

QString Torrent::GetUploadSpeed() const
{
	if(m_hTorrent.handle.is_valid())
	{
		return (m_hTorrent.upload_rate > 2 * KbInt) ? StaticHelpers::toKbMbGb(m_hTorrent.upload_rate, true) : "";
	}

	return "";
}
int Torrent::GetPeerCount()
{
	if (m_hTorrent.handle.is_valid())
	{
		return m_hTorrent.num_peers;
	}

	return 0;
}
int Torrent::GetActivePeerCount()
{
	if (m_hTorrent.handle.is_valid())
	{
		return m_hTorrent.num_seeds;
	}

	return 0;
}
QIcon Torrent::GetMimeTypeIcon() const
{
	return StyleEngene::getInstance()->guessMimeIcon(base_suffix);
}

void Torrent::announceRehash()
{
	if(m_hTorrent.handle.is_valid())
	{
		m_hTorrent.handle.force_recheck();
	}
}
bool Torrent::isSquential() const
{
	if(m_hTorrent.handle.is_valid())
	{
		return m_hTorrent.sequential_download;
	}

	return false;
}
void Torrent::seqensialDownload()
{
	if(m_hTorrent.handle.is_valid())
	{
		m_hTorrent.handle.set_sequential_download(!m_hTorrent.sequential_download);
	}
}

void Torrent::pause()
{
	if(m_hTorrent.handle.is_valid())
	{
		m_hTorrent.handle.auto_managed(false);
		m_hTorrent.handle.pause();
	}
}
void Torrent::resume()
{
	if(m_hTorrent.handle.is_valid())
	{
		m_hTorrent.handle.auto_managed(true);
		m_hTorrent.handle.resume();
	}
}
QString Torrent::GetSavePath()
{
	if(m_hTorrent.handle.is_valid())
	{
		return QDir::toNativeSeparators(
		           QString::fromUtf8(
#if LIBTORRENT_VERSION_NUM >= 10000
		               m_hTorrent.save_path.c_str()
#else
					   m_hTorrent.handle.save_path().c_str()
#endif
		           ));
	}

	return "";
}
QString Torrent::GetTotalUploaded() const
{
	if(m_hTorrent.handle.is_valid())
	{
		return StaticHelpers::toKbMbGb(m_hTorrent.all_time_upload);
	}

	return "";
}
QString Torrent::GetTotalDownloaded() const
{
	if(m_hTorrent.handle.is_valid())
	{
		return StaticHelpers::toKbMbGb(m_hTorrent.all_time_download);
	}

	return "";
}
QString Torrent::GetActiveTime()
{
	if(m_hTorrent.handle.is_valid())
	{
		return StaticHelpers::toTimeString(m_hTorrent.active_time);
	}

	return "";
}

QString Torrent::GetTotalSize() const
{
	return StaticHelpers::toKbMbGb(size);
}
QString Torrent::GetSeedString()
{
	if(m_hTorrent.handle.is_valid())
	{
		return qApp->translate("Torrent","CT_CONNECTED %1 CT_FROM %2").arg(m_hTorrent.num_seeds).arg(m_hTorrent.list_seeds);
	}

	return "";
}
QString Torrent::GetPeerString()
{
	if(m_hTorrent.handle.is_valid())
	{
		return  qApp->translate("Torrent", "CT_CONNECTED %1 CT_FROM %2").arg(m_hTorrent.num_peers).arg(m_hTorrent.list_peers);
	}

	return "";
}

QString Torrent::GetRemainingTime()
{
	QString res;

	if(m_hTorrent.handle.is_valid())
	{
		if(isSeeding() || isPaused())
		{
			res.append(QChar(8734));
			return res;
		}

		if (m_hTorrent.download_rate < KbInt * 10)
		{
			res.append(QChar(8734));
		}
		else
		{
			int time = size * (1.0f - m_hTorrent.progress_ppm / 1000000.f) / m_hTorrent.download_rate;
			res = StaticHelpers::toTimeString(time);
		}
	}

	return res;
}

files_info Torrent::GetFileDownloadInfo()
{
	files_info filesInfo;
	filesInfo.infoHash = GetInfoHash();

	if(m_hTorrent.handle.is_valid())
	{
		filesInfo.storrage =
#if LIBTORRENT_VERSION_NUM >= 10000
		    m_hTorrent.handle.torrent_file()->files();
#else
			m_hTorrent.handle.get_torrent_info().files();
#endif
		std::vector<libtorrent::size_type> downloadedSizes;
		m_hTorrent.handle.file_progress(downloadedSizes);
		filesInfo.progresses.reserve(downloadedSizes.size());

		for (int i = 0; i < filesInfo.storrage.num_files(); i++)
		{
			filesInfo.progresses.push_back(downloadedSizes[i] * 1.0 / filesInfo.storrage.file_size(i));
		}

		filesInfo.priorities = m_hTorrent.handle.file_priorities();
	}

	return filesInfo;
}

bool Torrent::isSingleFile()
{
#if LIBTORRENT_VERSION_NUM >= 10000
	return m_hTorrent.handle.torrent_file()->files().num_files() == 1;
#else
	return m_hTorrent.handle.get_torrent_info().files().num_files() == 1;
#endif
}

void Torrent::UpdateStatus(torrent_status newVal)
{
	m_hTorrent = newVal;
	float ratioLimit = QApplicationSettings::getInstance()->valueFloat("Torrent", "share_ratio_limit");

	if (ratioLimit > 0.0f)
	{
		float currentRatio = float(m_hTorrent.all_time_upload) / m_hTorrent.all_time_download;
		if (currentRatio > ratioLimit)
		{
			pause();
		}
	}
}

void Torrent::SetFilePriority(int index, int priority)
{
	if(m_hTorrent.handle.is_valid())
	{
		if (m_hTorrent.handle.file_priority(index) != priority)
		{
			m_hTorrent.handle.file_priority(index, priority);
			if (priority > 0)
			{
				size += m_hTorrent.torrent_file->file_at(index).size;
			}
			else
			{
				size -= m_hTorrent.torrent_file->file_at(index).size;
			}
		}
		
	}
}


void Torrent::CompliteMoveStorrage()
{
	isMovingFileStorrage = false;
}

void Torrent::UpdateTrackers()
{
	if(m_hTorrent.handle.is_valid())
	{
		m_hTorrent.handle.force_reannounce();
	}
}

void Torrent::UpdateTracker(int index)
{
	if (m_hTorrent.handle.is_valid())
	{
		m_hTorrent.handle.force_reannounce(0, index);
	}
}

void Torrent::MoveStorrage(QString path)
{
	if(m_hTorrent.handle.is_valid())
	{
		m_hTorrent.handle.move_storage(path.toStdString());
		isMovingFileStorrage = true;
	}
}

int Torrent::GetPieceCount()
{
	if(m_hTorrent.handle.is_valid())
	{
#if LIBTORRENT_VERSION_NUM >= 10000
		return m_hTorrent.handle.torrent_file()->files().num_pieces();
#else
		return m_hTorrent.handle.get_torrent_info().files().num_pieces();
#endif
	}

	return 0;
}

QBitArray Torrent::GetDownloadedPieces()
{
	QBitArray res(GetPieceCount());
	bitfield data;

	if(m_hTorrent.handle.is_valid())
	{
		data = m_hTorrent.pieces;
		int max_num = data.size();

		for(int i = 0; i < max_num; i++)
		{
			if(data.get_bit(i))
			{
				res.setBit(i);
			}
		}
	}

	return res;
}

QBitArray Torrent::GetDownloadingPieces()
{
	QBitArray res(GetPieceCount());

	//m_hTorrent.piece_availability();
	if(m_hTorrent.handle.is_valid())
	{
		std::vector<partial_piece_info> pieces;
		m_hTorrent.handle.get_download_queue(pieces);

		for(std::vector<partial_piece_info>::iterator i = pieces.begin(); i != pieces.end(); ++i)
		{
			res.setBit(i->piece_index);
		}
	}

	return res;
}

QString Torrent::GetDiscribtion()
{
	if(m_hTorrent.handle.is_valid())
	{
		return QString::fromUtf8(
#if LIBTORRENT_VERSION_NUM >= 10000
					m_hTorrent.handle.torrent_file()->comment().c_str()
#else
					m_hTorrent.handle.get_torrent_info().comment().c_str()
#endif
		       );
	}

	return "";
}

void Torrent::GetPieceAvalibility(std::vector<int>& availibility)
{
	if (m_hTorrent.handle.is_valid())
	{
		m_hTorrent.handle.piece_availability(availibility);
	}
}

float Torrent::GetDistributedCopies()
{
	if (m_hTorrent.handle.is_valid())
	{
		return m_hTorrent.distributed_copies;
	}

	return 0.0f;
}

void Torrent::SetUlLimit(int val)
{
	if(m_hTorrent.handle.is_valid())
	{
		m_hTorrent.handle.set_upload_limit(val);
	}
}

void Torrent::SetDlLimit(int val)
{
	if(m_hTorrent.handle.is_valid())
	{
		m_hTorrent.handle.set_download_limit(val);
	}
}

int Torrent::GetDownloadLimit()
{
	if(m_hTorrent.handle.is_valid())
	{
		return m_hTorrent.handle.download_limit();
	}

	return 0;
}

int Torrent::GetUploadLimit()
{
	if(m_hTorrent.handle.is_valid())
	{
		return m_hTorrent.handle.upload_limit();
	}

	return 0;
}
bool Torrent::hasMediaFiles()
{
	return m_hasMedia;
}

void Torrent::AddPeer(QHostAddress adr, short port)
{
	if(m_hTorrent.handle.is_valid())
	{
		tcp::endpoint ep(boost::asio::ip::address_v4(adr.toIPv4Address()), port);
		m_hTorrent.handle.connect_peer(ep);
	}
}

void Torrent::AddTracker(QString url)
{
	if(m_hTorrent.handle.is_valid())
	{
		m_hTorrent.handle.add_tracker(url.toStdString());
	}
}

void Torrent::stop()
{
	if(m_hTorrent.handle.is_valid())
	{
		m_hTorrent.handle.auto_managed(false);
		m_hTorrent.handle.pause(torrent_handle::graceful_pause);
	}
}

bool Torrent::isStoped() const
{
	return false;//m_stoped;
}

bool Torrent::operator< (const Torrent& other) const
{
	return GetName() < other.GetName();
}

bool Torrent::operator< (Torrent* other)
{
	return other->GetName() < (GetName());
}

void Torrent::SwitchSuperSeed()
{
	if(m_hTorrent.handle.is_valid())
	{
		m_hTorrent.handle.super_seeding(!m_hTorrent.super_seeding);
	}
}

bool Torrent::isSuperSeed()
{
	if(m_hTorrent.handle.is_valid())
	{
		return m_hTorrent.super_seeding;
	}

	return false;
}

QString Torrent::GetInfoHash()
{
	if(m_hTorrent.handle.is_valid())
	{
		return QString::fromStdString(to_hex(m_hTorrent.info_hash.to_string()));
	}

	return "";
}

QString Torrent::GetType()
{
	return type;
}

int Torrent::GetStatus()
{
	if(m_hTorrent.handle.is_valid())
	{
		return m_hTorrent.state;
	}

	return 0;
}

QString Torrent::generateMagnetLink()
{
	return QString::fromStdString(libtorrent::make_magnet_uri(m_hTorrent.handle));
}

QString Torrent::GetGroup()
{
	return group;
}

void Torrent::setGroup(QString newGroup)
{
	group = newGroup;
	m_hTorrent.handle.save_resume_data();
}

torrent_handle& Torrent::GetInternalHandle()
{
	return m_hTorrent.handle;
}

void Torrent::SetTrackerPeersCnt(QString trackerUrl, int peerCount)
{
	trackerPeerCountMap[trackerUrl] = peerCount;
}

int Torrent::GetPeerCountForTracker(QString url)
{
	if (trackerPeerCountMap.contains(url))
	{
		return trackerPeerCountMap[url];
	}

	return 0;
}

void Torrent::RemoveTracker(QString url)
{
	if (m_hTorrent.handle.is_valid())
	{
		std::string tracker2remove = url.toStdString();
		std::vector<announce_entry> currenttTrackers = m_hTorrent.handle.trackers();

		for (std::vector<announce_entry>::iterator i = currenttTrackers.begin(); i != currenttTrackers.end(); i++)
		{
			if (i->url.compare(tracker2remove) == 0)
			{
				currenttTrackers.erase(i);
				break;
			}
		}

		m_hTorrent.handle.replace_trackers(currenttTrackers);
		m_hTorrent.handle.force_reannounce();
	}
}

void Torrent::RemoveTrackers(QStringList trackers)
{
	if (m_hTorrent.handle.is_valid())
	{
		std::vector<announce_entry> currenttTrackers = m_hTorrent.handle.trackers();

		for (int i = 0 ; i < trackers.size(); i++)
		{
			QString url = trackers[i];
			std::string tracker2remove = url.toStdString();

			for (std::vector<announce_entry>::iterator i = currenttTrackers.begin(); i != currenttTrackers.end(); i++)
			{
				if (i->url.compare(tracker2remove) == 0)
				{
					currenttTrackers.erase(i);
					break;
				}
			}
		}

		m_hTorrent.handle.replace_trackers(currenttTrackers);
		m_hTorrent.handle.force_reannounce();
	}
}

void Torrent::ReplaceTracker(QString original, QString changed)
{
	if (m_hTorrent.handle.is_valid())
	{
		std::string tracker2remove = original.toStdString();
		std::vector<announce_entry> currenttTrackers = m_hTorrent.handle.trackers();

		for (std::vector<announce_entry>::iterator i = currenttTrackers.begin(); i != currenttTrackers.end(); i++)
		{
			if (i->url.compare(tracker2remove) == 0)
			{
				currenttTrackers.erase(i);
				break;
			}
		}

		currenttTrackers.push_back(announce_entry(changed.toStdString()));
		m_hTorrent.handle.replace_trackers(currenttTrackers);
		m_hTorrent.handle.force_reannounce();
	}
}

void Torrent::addWebSeed(QString webSeedUrl)
{
	if (m_hTorrent.handle.is_valid())
	{
		m_hTorrent.handle.add_http_seed(webSeedUrl.toStdString());
	}
}


