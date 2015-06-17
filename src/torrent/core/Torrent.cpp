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
bool Torrent::hasError() const
{
	if(m_hTorrent.is_valid())
	{
//       QString errorString="";
		bool hasErr = false;

		if(m_hTorrent.status(torrent_handle::query_accurate_download_counters).error.length() > 0)
		{
			hasErr = true;
		}

		return hasErr;
	}

	return false;
}

bool Torrent::isActive() const
{
	torrent_status status = m_hTorrent.status();
	return (status.download_rate > 2 * KbInt ||
	        status.upload_rate > 2 * KbInt ||
	        status.state == torrent_status::checking_files);
}
std::vector<peer_info> Torrent::GetPeerInfo()
{
	std::vector<peer_info> pInfo;

	if(m_hTorrent.is_valid())
	{
		m_hTorrent.get_peer_info(pInfo);
	}

	return pInfo;
}
std::vector<announce_entry> Torrent::GetTrackerInfo()
{
	std::vector<announce_entry> trackers;

	if(m_hTorrent.is_valid())
	{
		trackers = m_hTorrent.trackers();
	}

	return trackers;
}
QString Torrent::GetErrorMessage() const
{
	if(m_hTorrent.is_valid())
	{
		QString errorString = "";

		if(m_hTorrent.status().error.length() > 0)
		{
			errorString.append(QString::fromUtf8(m_hTorrent.status().error.c_str()));
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
	if(m_hTorrent.is_valid())
	{
		return m_hTorrent.status().has_metadata;
	}

	return false;
}
bool Torrent::isDownloading() const
{
	if(m_hTorrent.is_valid())
	{
		return m_hTorrent.status().state >= 2 && m_hTorrent.status().state <= 3;
	}

	return false;
}
bool Torrent::isPaused() const
{
	if(m_hTorrent.is_valid())
	{
		return m_hTorrent.status().paused;
	}

	return false;
}
bool Torrent::isSeeding() const
{
	if(m_hTorrent.is_valid())
	{
		if(m_hTorrent.status().state == 4)
		{
			return true;
		}

		return m_hTorrent.status().is_seeding;
	}

	return false;
}
float Torrent::GetProgress() const
{
	if(m_hTorrent.is_valid())
	{
		return m_hTorrent.status().progress_ppm / 10000.f;
	}

	return 0;
}
QString Torrent::GetSuffix()
{
	return base_suffix;
}

void Torrent::UpdateImageFiles()
{
	imageFiles.clear();

	if(m_hTorrent.is_valid())
	{
        file_storage storrgae =
        #if LIBTORRENT_VERSION_NUM >= 10000
                m_hTorrent.torrent_file()->files();
        #else
                m_hTorrent.get_torrent_info().files();
        #endif

		std::vector<size_type> progress;
		m_hTorrent.file_progress(progress);

		for (int i = 0; i < storrgae.num_files(); ++i)
		{
			QFileInfo curfile(QString::fromUtf8(storrgae.file_path(i).c_str()));
			QString currentSuffix = curfile.suffix().toLower();
			bool fileReady = storrgae.file_size(i) == progress[i];
			bool mayMount = (fileReady && StyleEngene::suffixes[StyleEngene::DISK].contains(currentSuffix));

			if (mayMount)
			{
                std::string save_path =
#if LIBTORRENT_VERSION_NUM >= 10000
                        m_hTorrent.status(torrent_handle::query_save_path).save_path;
#else
                        m_hTorrent.save_path();
#endif
                imageFiles << QString::fromUtf8(save_path.c_str()) + QString::fromUtf8(storrgae.file_path(i).c_str());
			}
		}
	}
}

QStringList& Torrent::GetImageFiles()
{
	UpdateImageFiles();
	return imageFiles;
}
Torrent::Torrent(torrent_handle* hTorrent, QString group)
	: QObject(NULL), m_hasMedia(false), m_isPrevSeed(false), size(0), mountable(false)
{
	m_hTorrent = *hTorrent;
	m_hTorrent.set_share_mode(false);

	if (!m_hTorrent.is_valid())
	{
		qCritical() << "Invalid Torrent recived...";
		return;
	}

    file_storage storrgae =
        #if LIBTORRENT_VERSION_NUM >= 10000
                m_hTorrent.torrent_file()->files();
        #else
                m_hTorrent.get_torrent_info().files();
        #endif
	this->group = group;
	std::vector<size_type> progress;
	m_hTorrent.file_progress(progress);

	for(int i = 0; i < storrgae.num_files(); i++)
	{
		QFileInfo curfile(QString::fromUtf8(storrgae.file_path(i).c_str()));
		QString currentSuffix = curfile.suffix().toLower();
		bool fileReady = storrgae.file_size(i) == progress[i];
		QSet<QString> diskSufixes = StyleEngene::suffixes[StyleEngene::DISK];
		bool mayMount = ((fileReady || m_hTorrent.file_priority(i) > 0) && diskSufixes.contains(currentSuffix));

		if (mayMount)
		{
            std::string save_path =
#if LIBTORRENT_VERSION_NUM >= 10000
                    m_hTorrent.status(torrent_handle::query_save_path).save_path;
#else
                    m_hTorrent.save_path();
#endif
            imageFiles << QString::fromUtf8(save_path.c_str()) + QString::fromUtf8(storrgae.file_path(i).c_str());
		}

		if (fileReady || m_hTorrent.file_priority(i) > 0)
		{
			QSet<QString> videoSuffix = StyleEngene::suffixes[StyleEngene::VIDEO];

			if(videoSuffix.contains(currentSuffix) || StyleEngene::suffixes[StyleEngene::AUDIO].contains(currentSuffix))
			{
				m_hasMedia = true;
			}

			size += storrgae.file_size(i);
		}
	}

	base_suffix = StaticHelpers::GetBaseSuffix(storrgae);

	if(!base_suffix.isEmpty())
	{
		icon = StyleEngene::getInstance()->guessMimeIcon(base_suffix, type);
	}
	else
	{
		icon = QIcon(":/icons/my-folder.ico");
		type = "folder";
	}
}

bool Torrent::isDaemonToolsMountable()
{
	UpdateImageFiles();
	return imageFiles.length() > 0;
}
QString Torrent::GetProgresString() const
{
	if(m_hTorrent.is_valid())
	{
		return QString::number(m_hTorrent.status().progress_ppm / 10000.f , 'f', 2) + "%";
	}

	return "";
}
QString Torrent::GetStatusString() const
{
	static QStringList state_str = QStringList() << (QT_TR_NOOP("STATE_FILE_CHEACKING (q)"))
	                               << (QT_TR_NOOP("STATE_FILE_CHEACKING"))
	                               << (QT_TR_NOOP("STATE_DOWNLOADING"))
	                               << (QT_TR_NOOP("STATE_DOWNLOADING"))
	                               << (QT_TR_NOOP("STATE_SEEDING"))
	                               << (QT_TR_NOOP("STATE_SEEDING"))
	                               << (QT_TR_NOOP("STATE_PREPARING"))
	                               << (QT_TR_NOOP("STATE_FILE_CHEACKING (r)"));

	if(m_hTorrent.is_valid())
	{
		return tr(state_str.at(m_hTorrent.status().state).toLatin1().data()) + (m_hTorrent.status().sequential_download ? (" [S]") : "");
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
	if(m_hTorrent.is_valid())
	{
        return QString::fromUtf8(
            #if LIBTORRENT_VERSION_NUM >= 10000
                                 m_hTorrent.status(torrent_handle::query_name).name.c_str()
            #else
                                m_hTorrent.name().c_str()
            #endif

                    );
	}

	return "";
}
QString Torrent::GetDwonloadSpeed()
{
	if(m_hTorrent.is_valid())
	{
		return (m_hTorrent.status().download_rate > 2 * KbInt) ? StaticHelpers::toKbMbGb(m_hTorrent.status().download_rate) + "\\s" : "";
	}

	return "";
}
QString Torrent::GetDwonloadSpeed() const
{
	if(m_hTorrent.is_valid())
	{
		return (m_hTorrent.status().download_rate > 2 * KbInt) ? StaticHelpers::toKbMbGb(m_hTorrent.status().download_rate) + "\\s" : "";
	}

	return "";
}
QString Torrent::GetUploadSpeed()
{
	if(m_hTorrent.is_valid())
	{
		return (m_hTorrent.status().upload_rate > 2 * KbInt) ? StaticHelpers::toKbMbGb(m_hTorrent.status().upload_rate) + "\\s" : "";
	}

	return "";
}

QString Torrent::GetUploadSpeed() const
{
	if(m_hTorrent.is_valid())
	{
		return (m_hTorrent.status().upload_rate > 2 * KbInt) ? StaticHelpers::toKbMbGb(m_hTorrent.status().upload_rate) + "\\s" : "";
	}

	return "";
}
int Torrent::GetPeerCount()
{
	if(m_hTorrent.is_valid())
	{
		return m_hTorrent.status().num_peers;
	}

	return 0;
}
int Torrent::GetActivePeerCount()
{
	if(m_hTorrent.is_valid())
	{
		return m_hTorrent.status().num_seeds;
	}

	return 0;
}
QIcon Torrent::GetMimeTypeIcon() const
{
	return StyleEngene::getInstance()->guessMimeIcon(base_suffix);
}
//-V659
QIcon Torrent::GetMimeTypeIcon()
{
	return StyleEngene::getInstance()->guessMimeIcon(base_suffix, type);
}

void Torrent::announceRehash()
{
	if(m_hTorrent.is_valid())
	{
		m_hTorrent.force_recheck();
	}
}
bool Torrent::isSquential() const
{
	if(m_hTorrent.is_valid())
	{
		return m_hTorrent.status().sequential_download;
	}

	return false;
}
void Torrent::seqensialDownload()
{
	if(m_hTorrent.is_valid())
	{
		m_hTorrent.set_sequential_download(!m_hTorrent.status().sequential_download);
	}
}

void Torrent::pause()
{
	if(m_hTorrent.is_valid())
	{
		m_hTorrent.auto_managed(false);
		m_hTorrent.pause();
	}
}
void Torrent::resume()
{
	if(m_hTorrent.is_valid())
	{
		m_hTorrent.resume();
	}
}
QString Torrent::GetSavePath()
{
	if(m_hTorrent.is_valid())
	{
        return QDir::toNativeSeparators(
                    QString::fromUtf8(
                #if LIBTORRENT_VERSION_NUM >= 10000
                                        m_hTorrent.status(torrent_handle::query_save_path).save_path.c_str()
                #else
                                        m_hTorrent.save_path().c_str()
                #endif
                                            ));
	}

	return "";
}
QString Torrent::GetTotalUploaded() const
{
	if(m_hTorrent.is_valid())
	{
		return StaticHelpers::toKbMbGb(m_hTorrent.status(torrent_handle::query_accurate_download_counters).all_time_upload);
	}

	return "";
}
QString Torrent::GetTotalDownloaded() const
{
	if(m_hTorrent.is_valid())
	{
		return StaticHelpers::toKbMbGb(m_hTorrent.status(torrent_handle::query_accurate_download_counters).all_time_download);
	}

	return "";
}
QString Torrent::GetActiveTime()
{
	if(m_hTorrent.is_valid())
	{
		return StaticHelpers::toTimeString(m_hTorrent.status(torrent_handle::query_accurate_download_counters).active_time);
	}

	return "";
}

QString Torrent::GetTotalSize() const
{
	return StaticHelpers::toKbMbGb(size);
}
QString Torrent::GetSeedString()
{
	if(m_hTorrent.is_valid())
	{
		torrent_status status = m_hTorrent.status(torrent_handle::query_accurate_download_counters);
		return QObject::tr("CT_CONNECTED %1 CT_FROM %2").arg(status.num_seeds).arg(status.list_seeds);
	}

	return "";
}
QString Torrent::GetPeerString()
{
	if(m_hTorrent.is_valid())
	{
		torrent_status status = m_hTorrent.status(torrent_handle::query_accurate_download_counters);
		return QObject::tr("CT_CONNECTED %1 CT_FROM %2").arg(status.num_peers).arg(status.list_peers);
	}

	return "";
}

QString Torrent::GetRemainingTime()
{
	QString res;

	if(m_hTorrent.is_valid())
	{
		if(isSeeding() || isPaused())
		{
			res.append(QChar(8734));
			return res;
		}

		torrent_status status = m_hTorrent.status(torrent_handle::query_accurate_download_counters);

		if(status.download_rate < KbInt * 10)
		{
			res.append(QChar(8734));
		}
		else
		{
			int time = size * (1.0f - status.progress_ppm / 1000000.f) / status.download_rate;
			res = StaticHelpers::toTimeString(time);
		}
	}

	return res;
}

files_info Torrent::GetFileDownloadInfo()
{
	files_info filesInfo;
	filesInfo.infoHash = GetInfoHash();

	if(m_hTorrent.is_valid())
	{
        filesInfo.storrage =
        #if LIBTORRENT_VERSION_NUM >= 10000
                m_hTorrent.torrent_file()->files();
        #else
                m_hTorrent.get_torrent_info().files();
        #endif
		std::vector<libtorrent::size_type> downloadedSizes;
		m_hTorrent.file_progress(downloadedSizes);
		filesInfo.progresses.reserve(downloadedSizes.size());

		for (int i = 0; i < filesInfo.storrage.num_files(); i++)
		{
			filesInfo.progresses.push_back(downloadedSizes[i] * 1.0 / filesInfo.storrage.file_size(i));
		}

		filesInfo.priorities = m_hTorrent.file_priorities();
	}

	return filesInfo;
}

bool Torrent::isSingleFile()
{
    #if LIBTORRENT_VERSION_NUM >= 10000
            return m_hTorrent.torrent_file()->files().num_files() == 1;
    #else
            return m_hTorrent.get_torrent_info().files().num_files() == 1;
    #endif
}

void Torrent::SetFilePriority(int index, int prioryty)
{
	if(m_hTorrent.is_valid())
	{
		m_hTorrent.file_priority(index, prioryty);
	}
}

void Torrent::updateTrackers()
{
	if(m_hTorrent.is_valid())
	{
		m_hTorrent.force_reannounce();
	}
}

void Torrent::MoveStorrage(QString path)
{
	if(m_hTorrent.is_valid())
	{
		m_hTorrent.move_storage(path.toStdString());
	}
}

int Torrent::GetPieceCount()
{
	if(m_hTorrent.is_valid())
	{
#if LIBTORRENT_VERSION_NUM >= 10000
        return m_hTorrent.torrent_file()->files().num_files() == 1;
#else
        return m_hTorrent.get_torrent_info().files().num_pieces();
#endif
	}

	return 0;
}

QVector<int> Torrent::GetDownloadedPieces()
{
	QVector<int> res;
	libtorrent::bitfield data;

	if(m_hTorrent.is_valid())
	{
        int max_num = GetPieceCount();
		data.resize(max_num);
		data.clear_all();

		for(int i = 0; i < max_num; i++)
		{
			if(m_hTorrent.have_piece(i))
			{
				data.set_bit(i);
				res.append(i);
			}
		}
	}

	return res;
}

QVector<int> Torrent::GetDownloadingPieces()
{
	QVector<int> res;
	libtorrent::bitfield data;

	if(m_hTorrent.is_valid())
	{
		std::vector<partial_piece_info> pieces;
		m_hTorrent.get_download_queue(pieces);
        int max_num = GetPieceCount();
		data.resize(max_num);
		data.clear_all();

		for(std::vector<partial_piece_info>::iterator i = pieces.begin(); i != pieces.end(); ++i)
		{
			if(i->finished + i->writing > 0)
			{
				data.set_bit(i->piece_index);
				res.append(i->piece_index);
			}
		}
	}

	return res;
}

QString Torrent::GetDiscribtion()
{
	if(m_hTorrent.is_valid())
	{
        return QString::fromUtf8(
            #if LIBTORRENT_VERSION_NUM >= 10000
                    m_hTorrent.torrent_file()->comment().c_str()
            #else
                    m_hTorrent.get_torrent_info().comment().c_str()
            #endif
                    );
	}

	return "";
}

void Torrent::SetUlLimit(int val)
{
	if(m_hTorrent.is_valid())
	{
		m_hTorrent.set_upload_limit(val);
	}
}

void Torrent::SetDlLimit(int val)
{
	if(m_hTorrent.is_valid())
	{
		m_hTorrent.set_download_limit(val);
	}
}

int Torrent::GetDownloadLimit()
{
	if(m_hTorrent.is_valid())
	{
		return m_hTorrent.download_limit();
	}

	return 0;
}

int Torrent::GetUploadLimit()
{
	if(m_hTorrent.is_valid())
	{
		return m_hTorrent.upload_limit();
	}

	return 0;
}
bool Torrent::hasMediaFiles()
{
	return m_hasMedia;
}

void Torrent::AddPeer(QHostAddress adr, short port)
{
	if(m_hTorrent.is_valid())
	{
		tcp::endpoint ep(boost::asio::ip::address_v4(adr.toIPv4Address()), port);
		m_hTorrent.connect_peer(ep);
	}
}

void Torrent::AddTracker(QString url)
{
	if(m_hTorrent.is_valid())
	{
		m_hTorrent.add_tracker(url.toStdString());
	}
}

void Torrent::stop()
{
	if(m_hTorrent.is_valid())
	{
		m_hTorrent.auto_managed(false);
		m_hTorrent.pause(torrent_handle::graceful_pause);
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

void Torrent::SuperSeed()
{
	if(m_hTorrent.is_valid())
	{
		m_hTorrent.super_seeding(!m_hTorrent.status().super_seeding);
	}
}

bool Torrent::isSuperSeed()
{
	if(m_hTorrent.is_valid())
	{
		return m_hTorrent.status().super_seeding;
	}

	return false;
}

QString Torrent::GetInfoHash()
{
	if(m_hTorrent.is_valid())
	{
		return QString::fromStdString(to_hex(m_hTorrent.info_hash().to_string()));
	}

	return "";
}

QString Torrent::GetType()
{
	return type;
}

int Torrent::GetStatus()
{
	if(m_hTorrent.is_valid())
	{
		return m_hTorrent.status().state;
	}

	return 0;
}

QString Torrent::generateMagnetLink()
{
	return QString::fromStdString(libtorrent::make_magnet_uri(m_hTorrent));
}

QString Torrent::GetGroup()
{
	return group;
}

void Torrent::setGroup(QString newGroup)
{
	group = newGroup;
	m_hTorrent.save_resume_data();
}

torrent_handle& Torrent::GetInternalHandle()
{
	return m_hTorrent;
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
	if (m_hTorrent.is_valid())
	{
		std::string tracker2remove = url.toStdString();
		std::vector<announce_entry> currenttTrackers = m_hTorrent.trackers();

		for (std::vector<announce_entry>::iterator i = currenttTrackers.begin(); i != currenttTrackers.end(); i++)
		{
			if (i->url.compare(tracker2remove) == 0)
			{
				currenttTrackers.erase(i);
				break;
			}
		}

		m_hTorrent.replace_trackers(currenttTrackers);
		m_hTorrent.force_reannounce();
	}
}

void Torrent::RemoveTrackers(QStringList trackers)
{
	if (m_hTorrent.is_valid())
	{
		std::vector<announce_entry> currenttTrackers = m_hTorrent.trackers();

        for (int i = 0 ; i< trackers.size();i++)
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

		m_hTorrent.replace_trackers(currenttTrackers);
		m_hTorrent.force_reannounce();
	}
}

void Torrent::ReplaceTracker(QString original, QString changed)
{
	if (m_hTorrent.is_valid())
	{
		std::string tracker2remove = original.toStdString();
		std::vector<announce_entry> currenttTrackers = m_hTorrent.trackers();

		for (std::vector<announce_entry>::iterator i = currenttTrackers.begin(); i != currenttTrackers.end(); i++)
		{
			if (i->url.compare(tracker2remove) == 0)
			{
				currenttTrackers.erase(i);
				break;
			}
		}

		currenttTrackers.push_back(announce_entry(changed.toStdString()));
		m_hTorrent.replace_trackers(currenttTrackers);
		m_hTorrent.force_reannounce();
	}
}

void Torrent::addWebSeed(QString webSeedUrl)
{
	if (m_hTorrent.is_valid())
	{
		m_hTorrent.add_http_seed(webSeedUrl.toStdString());
	}
}


