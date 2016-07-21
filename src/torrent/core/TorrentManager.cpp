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
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QApplication>
#include <QDebug>
#include <QDir>

#include <deque>
#include <exception>
#include <iosfwd>
#include <iterator>
#include <utility>
#include "StaticHelpers.h"
#include "QApplicationSettings.h"
#include "TorrentManager.h"
#include "TorrentStorrage.h"
#include "Torrent.h"
#include "defs.h"
#include <libtorrent/add_torrent_params.hpp>
#include <libtorrent/address.hpp>
#include <libtorrent/alert.hpp>
#include <libtorrent/alert_types.hpp>
#include <libtorrent/bencode.hpp>
#include <libtorrent/config.hpp>
#include <libtorrent/create_torrent.hpp>
#include <libtorrent/entry.hpp>
#include <libtorrent/file.hpp>
#include <libtorrent/file_storage.hpp>
#include <libtorrent/ip_filter.hpp>
#include <libtorrent/lazy_entry.hpp>
#include <libtorrent/session_status.hpp>
#if LIBTORRENT_VERSION_NUM >= 10000
#include <libtorrent/sha1_hash.hpp>
#endif
#include <libtorrent/storage_defs.hpp>
#include <libtorrent/thread.hpp>
#include <libtorrent/torrent_info.hpp>
#include <libtorrent/extensions/ut_pex.hpp>
#include <libtorrent/extensions/ut_metadata.hpp>
#include <libtorrent/extensions/smart_ban.hpp>
#include <libtorrent/magnet_uri.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include "Version.h"
#include "messagebox.h"
#include "NotificationSystem.h"
#include "filedownloader.h"
#include <TorrentGroup.h>
#include <TorrentGroupsManager.h>
#include <avaliableSpacePlugin/avaliable_space_verifier_plugin.h>
#include <PowerManagement.h>
using namespace libtorrent;
namespace fs = boost::filesystem;
TorrentManager::TorrentManager()
	: m_pFileDownloader(FileDownloader::getNewInstance())
	  , m_bIsSaveSessionInitiated(false)
	  , m_pNotificationSys(NotificationSystem::getInstance())
	  , m_powerManagement(PowerManagement::getInstance())
	  , m_lastActiveTime(0)
	  , m_shouldRate(false)
#if LIBTORRENT_VERSION_NUM < 10000
	, m_pUpnp(NULL)
#endif
{
	m_pTorrentStorrage = TorrentStorrage::getInstance();
	m_pTorrentSessionSettings = QApplicationSettings::getInstance();
	m_pTorrentSession = new session(fingerprint("cT", Version::Major(), Version::Minor(), Version::Revision(), Version::Build())
	                                , 0
	                                , alert::error_notification | alert::storage_notification | alert::status_notification | alert::tracker_notification | alert::port_mapping_notification);
	error_code ec;
	std::vector<char> in;
	QString dataDir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
	QString oldStyleDirPath = StaticHelpers::CombinePathes(QApplication::applicationDirPath(), "CT_DATA");
	QDir oldStyleDir(oldStyleDirPath);

	QString bstSessionDir = StaticHelpers::CombinePathes(dataDir, "BtSessionData");
	if (oldStyleDir.exists())
	{
		QString newStyleDirPath = bstSessionDir;

		if (!MoveFiles(oldStyleDirPath, newStyleDirPath))
		{
			CustomMessageBox::warning(NULL, "Move dir failed", QString("Failed to move %1 to %2!").arg(oldStyleDirPath, newStyleDirPath));
		}
	}


	QString stateFileName = StaticHelpers::CombinePathes(dataDir, "BtSessionData/actual.state");

	if (StaticHelpers::LoadFile(stateFileName, in))
	{
		entry e = bdecode(&in[0], &in[0] + in.size());
		if (entry* i = e.find_key("active_session_time"))
		{
			m_lastActiveTime = i->integer();
		}
		m_pTorrentSession->load_state(e);
	}
	m_shouldRate = (m_lastActiveTime > 360000);
	m_elapsedTimer.start();
	session_settings s_settings = readSettings();
	// upnp
#if LIBTORRENT_VERSION_NUM >= 10000
	m_pTorrentSession->start_upnp();
#else
	m_pUpnp = m_pTorrentSession->start_upnp();
#endif
	connect(m_pFileDownloader.get(), SIGNAL(DownloadReady(QUrl, QTemporaryFile*)), SLOT(OnDownloadReady(QUrl, QTemporaryFile*)));
/*	QString geoIpPath = StaticHelpers::CombinePathes(dataDir, "GeoIP.dat");
	QUrl geoIpUrl = QUrl("http://geolite.maxmind.com/download/geoip/database/GeoLiteCountry/GeoIP.dat.gz");

	if (QFile::exists(geoIpPath))
	{
		QFileInfo fileInfo(geoIpPath);

		if (QDateTime::currentDateTime() >= fileInfo.lastModified().addMonths(1))
		{
			m_pFileDownloader->download(geoIpUrl);
		}
		else
		{
			m_pTorrentSession->load_country_db(geoIpPath.toUtf8().data());
		}
	}
	else
	{
		m_pFileDownloader->download(geoIpUrl);
	}
	*/
	m_pTorrentSession->start_natpmp();
	m_pTorrentSession->add_extension(create_avaliable_space_verifier_plugin());
	m_pTorrentSession->add_extension(&create_ut_metadata_plugin);
	m_pTorrentSession->add_extension(&create_smart_ban_plugin);

	if (m_pTorrentSessionSettings->valueBool("Torrent", "use_pex", true))
	{
		m_pTorrentSession->add_extension(&create_ut_pex_plugin);
	}
	std::string data_dir = bstSessionDir.toUtf8().data();
	if (!QDir(bstSessionDir).exists())
	{
		create_directories(data_dir, ec);
		if (ec)
		{
			CustomMessageBox::critical(NULL, "ERROR", StaticHelpers::translateLibTorrentError(ec));
			return;
		}
	}


	RefreshExternalPeerSettings();
	m_pTorrentSession->listen_on(std::make_pair(listen_port, listen_port + 20)
	                             , ec);

	if (ec)
	{
		CustomMessageBox::critical(NULL, "ERROR", tr("LISTENING ON PORT %1 FAILED").arg(listen_port));
		return;
	}

	m_pTorrentSession->set_settings(s_settings);
	
	connect(this, SIGNAL(Notify(int, QString, QVariant)), m_pNotificationSys.get(), SLOT(OnNewNotification(int, QString, QVariant)));
	m_updateTimerID = startTimer(500);
}

bool TorrentManager::MoveFiles(QString oldStyleDirPath, QString newStyleDirPath) const
{
	QDir oldDir(oldStyleDirPath);
	QDir newDir(newStyleDirPath);
	QStringList entryList = oldDir.entryList();
	bool res = true;

	for (int i = 0; i < entryList.length(); i++)
	{
		QString fileName = entryList[i];
		res = QFile::copy(oldDir.absoluteFilePath(fileName), newDir.absoluteFilePath(fileName)) && res;
		res = QFile::remove(oldDir.absoluteFilePath(fileName)) && res;
	}

	oldDir.rmdir(oldStyleDirPath);
	return res;
}

void TorrentManager::timerEvent(QTimerEvent* timerEvent)
{
	if (timerEvent->timerId() == m_updateTimerID)
	{
		m_pTorrentSession->post_torrent_updates();
		dispatchPendingAlerts();
		if (GetSessionDwonloadRate() < 5 * KbInt)
		{
			m_powerManagement->pefromActionOnIdle(PowerManagement::IDLE_DOWNLOAD);
			if (GetSessionUploadRate() < 5 * KbInt)
			{
				m_powerManagement->pefromActionOnIdle(PowerManagement::IDLE_ALL);
			}
		}
	}
}

void TorrentManager::InitSession(boost::function<void(int proggres, QString item)> progressCallback)
{
	QString dataDir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
	QDir dir(StaticHelpers::CombinePathes(dataDir, "BtSessionData"));
	QStringList filter;
	filter << "*.torrent";
	QStringList torrentFiles = dir.entryList(filter);
	//QMessageBox::warning(NULL,QApplication::applicationDirPath(),QApplication::applicationDirPath());
	QList<QString>::iterator begin = torrentFiles.begin();
	int size = torrentFiles.size();

	for (QStringList::iterator i = begin; i != torrentFiles.end(); ++i)
	{
		error_code ec;
		QString filePath = dir.filePath(*i);
		torrent_info ti(filePath.toUtf8().data(), ec);
		QString infoHash = QString::fromStdString(to_hex(ti.info_hash().to_string()));
		QString savePath = "";


		Torrent* pTorrent = AddTorrent(filePath, savePath, ec);

		if (ec)
		{
			qCritical() << StaticHelpers::translateLibTorrentError(ec);
		}

		progressCallback((i - begin) * 100 / size, pTorrent->GetName());
	}
}

bool yes(torrent_status const&)
{
	return true;
}

std::vector<torrent_status> TorrentManager::GetTorrents()
{
	std::vector<torrent_status> result;
	m_pTorrentSession->get_torrent_status(&result, yes);
	return result;
}

void TorrentManager::handle_alert(alert* a)
{
	QString dataDir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
	TORRENT_TRY
	{
		switch (a->type())
		{
			case tracker_reply_alert::alert_type:
			{
				tracker_reply_alert* p = alert_cast<tracker_reply_alert>(a);
				Torrent* pTorrent = m_pTorrentStorrage->getTorrent(QString::fromStdString(to_hex(p->handle.info_hash().to_string())));

				if (pTorrent != NULL)
				{
					pTorrent->SetTrackerPeersCnt(QString::fromStdString(p->url), p->num_peers);
				}

				break;
			}


			case torrent_finished_alert::alert_type:
			{
				torrent_finished_alert* p = alert_cast<torrent_finished_alert>(a);
				p->handle.set_max_connections(max_connections_per_torrent / 2);
				torrent_handle h = p->handle;
				QString infoHash = QString::fromStdString(to_hex(h.info_hash().to_string()));
				Torrent* pTorrent = m_pTorrentStorrage->getTorrent(infoHash);

				if (pTorrent != NULL)
				{
					if (pTorrent->isSquential())
					{
						pTorrent->seqensialDownload();
					}

					if (!pTorrent->isPrevioslySeeded())
					{
						QString name = pTorrent->GetName();
						emit Notify(NotificationSystem::TORRENT_COMPLETED, tr("TORRENT_COMPLETED %1").arg(name), StaticHelpers::CombinePathes(pTorrent->GetSavePath(), name));
					}
				}

				h.save_resume_data();
				break;
			}

			case save_resume_data_failed_alert::alert_type:
			{
				save_resume_data_failed_alert* p = alert_cast<save_resume_data_failed_alert>(a);
				p->handle.save_resume_data();
				break;
			}

			case save_resume_data_alert::alert_type:
			{
				save_resume_data_alert* p = alert_cast<save_resume_data_alert>(a);
				torrent_handle h = p->handle;

				if (p->resume_data)
				{
					std::vector<char> out;
					entry e = *p->resume_data;
					QString info_hash = QString::fromStdString(to_hex(h.info_hash().to_string()));
					Torrent* torrent = m_pTorrentStorrage->getTorrent(info_hash);

					if (torrent != NULL)
					{
						bool isSeed = torrent->isSeeding();
						e["torrent_group_id"] = torrent->GetGroupUid().toString().toUtf8().data();
						e["torrent_name"] =
#if LIBTORRENT_VERSION_NUM >= 10000
							h.status(torrent_handle::query_name).name;
#else
							h.name();
#endif
						e["is_previous_seed"] = isSeed ? 1 : 0;
						torrent->setIsPrevioslySeeded(isSeed);
					}

					bencode(back_inserter(out), e);
					save_file(StaticHelpers::CombinePathes(dataDir, "BtSessionData", info_hash + ".resume").toUtf8().data(), out);

					if (m_bIsSaveSessionInitiated)
					{
						num_outstanding_resume_data--;
					}
				}

				break;
			}

			case tracker_error_alert::alert_type:
			{
				tracker_error_alert* p = alert_cast<tracker_error_alert>(a);
				QString infoHash = QString::fromStdString(to_hex(p->handle.info_hash().to_string()));
				Torrent* pTorrent = m_pTorrentStorrage->getTorrent(infoHash);

				if (pTorrent != NULL)
				{
					emit Notify(NotificationSystem::TRACKER_ERROR, tr("TORRENT %1 HAS TRACKER ERROR %2").arg(pTorrent->GetName(), StaticHelpers::translateLibTorrentError(p->error)), QVariant());
				}

				break;
			}

			case storage_moved_alert::alert_type:
			{
				storage_moved_alert* p = alert_cast<storage_moved_alert>(a);
				torrent_handle h = p->handle;

				if (h.is_valid())
				{
					h.save_resume_data();
				}

				QString infoHash = QString::fromStdString(to_hex(h.info_hash().to_string()));
				Torrent* pTorrent = m_pTorrentStorrage->getTorrent(infoHash);

				if (pTorrent != NULL)
				{
					emit Notify(NotificationSystem::TORRENT_INFO, tr("MOVE_STORRAGE_COMPLETED_TO:\n%1 %2").arg(pTorrent->GetName(), pTorrent->GetSavePath()), pTorrent->GetSavePath());
					pTorrent->CompliteMoveStorrage();
				}

				break;
			}

			case torrent_error_alert::alert_type:
			{
				torrent_error_alert* p = alert_cast<torrent_error_alert>(a);
				QString infoHash = QString::fromStdString(to_hex(p->handle.info_hash().to_string()));
				Torrent* pTorrent = m_pTorrentStorrage->getTorrent(infoHash);

				if (pTorrent != NULL)
				{
					if (m_handeledAlerts.contains(a->type()) && m_handeledAlerts[a->type()].contains(infoHash))
					{
						qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch(), lastHandeledTime = m_handeledAlerts[a->type()][infoHash];

						if (currentTime - lastHandeledTime > 60000)
						{
							emit Notify(NotificationSystem::TORRENT_ERROR, tr("TORRENT %1 HAS ERROR %2").arg(pTorrent->GetName(), StaticHelpers::translateLibTorrentError(p->error)), QVariant());
							m_handeledAlerts[a->type()][infoHash] = QDateTime::currentDateTime().toMSecsSinceEpoch();
						}
					}
					else
					{
						emit Notify(NotificationSystem::TORRENT_ERROR, tr("TORRENT %1 HAS ERROR %2").arg(pTorrent->GetName(), StaticHelpers::translateLibTorrentError(p->error)), QVariant());
						m_handeledAlerts[a->type()][infoHash] = QDateTime::currentDateTime().toMSecsSinceEpoch();
					}
					pTorrent->pause();
				}

				break;
			}

			case fastresume_rejected_alert::alert_type:
			{
				fastresume_rejected_alert* p = alert_cast<fastresume_rejected_alert>(a);
				torrent_handle h = p->handle;
				h.auto_managed(false);
				h.pause();
				QString infoHash = QString::fromStdString(to_hex(h.info_hash().to_string()));
				Torrent* pTorrent = m_pTorrentStorrage->getTorrent(infoHash);

				if (pTorrent != NULL)
				{
					emit Notify(NotificationSystem::TORRENT_ERROR, tr("TORRENT %1 HAS ERROR %2").arg(pTorrent->GetName(), StaticHelpers::translateLibTorrentError(p->error)), QVariant());
				}

				break;
			}

			case metadata_received_alert::alert_type:
			{
				metadata_received_alert* p = alert_cast<metadata_received_alert>(a);
				torrent_handle h = p->handle;

				if (h.is_valid())
				{
					try
					{
#if LIBTORRENT_VERSION_NUM >= 10000
						boost::shared_ptr<torrent_info const> ti = h.torrent_file();

						if (ti != NULL)
						{
							create_torrent ct(*ti.get());
							QString infoHash = QString::fromStdString(to_hex(ti->info_hash().to_string()));
#else
						const torrent_info ti = h.get_torrent_info();
						{
							create_torrent ct(ti);
							QString infoHash = QString::fromStdString(to_hex(ti.info_hash().to_string()));
#endif
							std::ofstream out(StaticHelpers::CombinePathes(dataDir, "BtSessionData", infoHash + ".torrent").toUtf8().data(), std::ios_base::binary);
							bencode(std::ostream_iterator<char>(out), ct.generate());
						}
					}
					catch (...)
					{
						qCritical() << "Exception in metadata_received_alert";
					}
				}

				break;
			}

			case peer_error_alert::alert_type:
			{
				peer_error_alert* p = alert_cast<peer_error_alert>(a);
				QString infoHash = QString::fromStdString(to_hex(p->handle.info_hash().to_string()));
				Torrent* pTorrent = m_pTorrentStorrage->getTorrent(infoHash);

				if (pTorrent != NULL)
				{
					emit Notify(NotificationSystem::TORRENT_ERROR, tr("TORRENT %1 HAS ERROR %2").arg(pTorrent->GetName(), StaticHelpers::translateLibTorrentError(p->error)), QVariant());
				}

				break;
			}

			case portmap_error_alert::alert_type:
			{
				portmap_error_alert* alert = alert_cast<portmap_error_alert>(a);
				QString infoHash = "Non-torrent";
				if (m_handeledAlerts.contains(a->type()) && m_handeledAlerts[a->type()].contains(infoHash))
				{
					if (QDateTime::currentDateTime().toMSecsSinceEpoch() - m_handeledAlerts[a->type()][infoHash] > 60000)
					{
						emit Notify(NotificationSystem::SYSTEM_ERROR, StaticHelpers::translateLibTorrentError(alert->error), QVariant());
						m_handeledAlerts[a->type()][infoHash] = QDateTime::currentDateTime().toMSecsSinceEpoch();
					}
				}
				else
				{
					emit Notify(NotificationSystem::SYSTEM_ERROR, StaticHelpers::translateLibTorrentError(alert->error), QVariant());
					m_handeledAlerts[a->type()][infoHash] = QDateTime::currentDateTime().toMSecsSinceEpoch();
				}
				emit Notify(NotificationSystem::SYSTEM_ERROR, StaticHelpers::translateLibTorrentError(alert->error), QVariant());
				break;
			}

			case dht_error_alert::alert_type:
			{
				dht_error_alert* p = alert_cast<dht_error_alert>(a);
				emit Notify(NotificationSystem::SYSTEM_ERROR, StaticHelpers::translateLibTorrentError(p->error), QVariant());
				break;
			}

			case state_update_alert::alert_type:
			{
				state_update_alert* p = alert_cast<state_update_alert>(a);
				size_t size = p->status.size();
				QSet<QString> torrentUpdates;
				for (int i = 0; i < size; i++)
				{
					torrent_status status = p->status[i];
					QString infoHash = QString::fromStdString(to_hex(status.info_hash.to_string()));
					torrentUpdates.insert(infoHash);
					Torrent* torrent = m_pTorrentStorrage->getTorrent(infoHash);

					if (torrent != NULL)
					{
						torrent->UpdateStatus(status);
					}
				}
				emit TorrentsChanged(torrentUpdates);
				break;
			}
			case file_error_alert::alert_type:
			case lsd_peer_alert::alert_type:
			case incoming_connection_alert::alert_type:
			case portmap_log_alert::alert_type:
			case portmap_alert::alert_type:
			case peer_unsnubbed_alert::alert_type:
			case peer_snubbed_alert::alert_type:
			case peer_disconnected_alert::alert_type:
			case peer_connect_alert::alert_type:
			case performance_alert::alert_type:
			case add_torrent_alert::alert_type:
			case listen_succeeded_alert::alert_type:
			case state_changed_alert::alert_type:
			case torrent_added_alert::alert_type:
			case torrent_checked_alert::alert_type:
			case torrent_resumed_alert::alert_type:
			case torrent_paused_alert::alert_type:
			case torrent_removed_alert::alert_type:
			case cache_flushed_alert::alert_type:
			case torrent_deleted_alert::alert_type:
			case external_ip_alert::alert_type:
			case udp_error_alert::alert_type:
			case dht_reply_alert::alert_type:
			case tracker_announce_alert::alert_type:
				break;

			default:
			{
				QString information = QString::fromUtf8(a->message().c_str());

				if ((a->category() & alert::error_notification) == alert::error_notification)
				{
					emit Notify(NotificationSystem::ERRORS, information, QVariant());
				}
				else
				{
					emit Notify(NotificationSystem::TORRENT_INFO, information, QVariant());
				}

				break;
			}
		}
	}
	TORRENT_CATCH(std::exception & e)
	{
		qCritical() << "exception in handle_alert" << e.what();
	}
}


Torrent* TorrentManager::AddTorrent(QString& path, QString& save_path, error_code& ec, QString name, QMap<QString, quint8> filePriorities, TorrentGroup* group, AddTorrentFlags flags)

{
	boost::shared_ptr<torrent_info> t;
	t.reset(new torrent_info(path.toUtf8().data(), ec));

	if (ec != 0)
	{
		return NULL;
	}

	add_torrent_params p;
	QString dataDir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
	std::string infoHash = to_hex(t->info_hash().to_string());
	QString qInfoHash = QString::fromStdString(infoHash);
	qDebug() << "Data dir: " << dataDir;
	QString btDataDir = QString("BtSessionData");
	QString resumeFileName = StaticHelpers::CombinePathes(dataDir, "BtSessionData", qInfoHash + ".resume");
	std::vector<char> buf;

	bool isPreviousSeed = false;
	qDebug() << "Resume file Name:" << resumeFileName;
	if (StaticHelpers::LoadFile(resumeFileName, buf))
	{
#if LIBTORRENT_VERSION_NUM >= 10000
		p.resume_data = buf;
#else
		p.resume_data = &buf;
#endif
		entry e = bdecode(buf.begin(), buf.end());

		if (entry* i = e.find_key("torrent_name"))
		{
			std::string stdName = i->string();
			name = QString::fromUtf8(stdName.c_str());
		}

		QString groupName;
		if (entry* i = e.find_key("torrent_group"))
		{
			std::string string = i->string();
			groupName = QString::fromUtf8(string.c_str());
		}
		if (!groupName.isEmpty())
		{
			group = TorrentGroupsManager::getInstance()->GetGroupByName(groupName);
		}
		else
		{
			QUuid groupUid;
			if (entry* i = e.find_key("torrent_group_id"))
			{
				std::string string = i->string();
				groupUid = QString::fromUtf8(string.c_str());
			}
			group = TorrentGroupsManager::getInstance()->GetGroup(groupUid);
		}


		if (entry* i = e.find_key("save_path"))
		{
			save_path = QString::fromUtf8(i->string().c_str());
		}

		if (entry* i = e.find_key("is_previous_seed"))
		{
			isPreviousSeed = (i->integer() == 1);
		}
	}

	if (!filePriorities.isEmpty())
	{
		std::vector<uint8_t> filepriorities;
		file_storage storrage = t->files();
		int filesCount = storrage.num_files();

		for (int i = 0; i < filesCount; i++)
		{
			file_entry file = storrage.at(i);
			QString filePath = QDir::toNativeSeparators(QString::fromUtf8(file.path.c_str()));

			if (filePriorities.contains(filePath))
			{
				filepriorities.push_back(filePriorities[filePath]);
			}
			else
			{
				filepriorities.push_back(4);
			}
		}

#if LIBTORRENT_VERSION_NUM >= 10000
		p.file_priorities = filepriorities;
#else
		p.file_priorities = &filepriorities;
#endif
	}

	if (!name.isEmpty())
	{
		p.name = name.toUtf8().data();
		file_storage fs = t->files();
		fs.set_name(p.name);
		t->remap_files(fs);
	}

	p.ti = t;
	p.save_path = std::string(save_path.toUtf8().data());
	p.storage_mode = storage_mode_t(m_pTorrentSessionSettings->valueInt("Torrent", "file_allocation_mode", storage_mode_sparse));
	p.flags = add_torrent_params::flag_duplicate_is_error | add_torrent_params::flag_update_subscribe | add_torrent_params::flag_auto_managed;

	if (flags.testFlag(PAUSED_MODE))
	{
		p.flags |= add_torrent_params::flag_paused;
	}

	if (flags.testFlag(SEED_MODE))
	{
		p.flags |= add_torrent_params::flag_seed_mode;
	}

	if (flags.testFlag(SUPER_SEED_MODE))
	{
		p.flags |= add_torrent_params::flag_super_seeding;
	}

#if LIBTORRENT_VERSION_NUM >= 10000

	if (flags.testFlag(SEQUENTIAL_MODE))
	{
		p.flags |= add_torrent_params::flag_sequential_download;
	}

#else
	p.flags |= add_torrent_params::flag_paused;
#endif
	p.userdata = static_cast<void*>(strdup(path.toLatin1().data()));
	torrent_handle h = m_pTorrentSession->add_torrent(p, ec);

	if (ec || !h.is_valid())
	{
		//	QMessageBox::warning(0,"Error",ec.message().c_str());
		return NULL;
	}

#if LIBTORRENT_VERSION_NUM < 10000

	if (flags.testFlag(SEQUENTIAL_MODE))
	{
		h.set_sequential_download(true);
	}

	h.resume();
#endif
	torrent_status status = h.status();
	Torrent* current = new Torrent(status, group);
	current->setIsPrevioslySeeded(isPreviousSeed);
	m_pTorrentStorrage->append(current);
	qDebug() << "emit TorrentAdded";
	emit TorrentAdded(current);
	h.set_max_connections(max_connections_per_torrent);
	QFile::copy(path, StaticHelpers::CombinePathes(dataDir, "BtSessionData", qInfoHash + ".torrent"));
	return current;
}

session_settings TorrentManager::readSettings()
{
	session_settings s_settings = m_pTorrentSession->settings();
	s_settings.smooth_connects = false;
	s_settings.half_open_limit = m_pTorrentSessionSettings->valueInt("Torrent", "half_open_limit", 0x7fffffff);
	s_settings.allow_multiple_connections_per_ip = m_pTorrentSessionSettings->valueBool("Torrent", "allow_multiple_connections_per_ip", true);
	if (m_pTorrentSessionSettings->valueBool("Torrent", "random_listen_port", false))
	{
		listen_port = rand() % 65535;
		m_pTorrentSessionSettings->setValue("Torrent", "listen_port", listen_port);
	}
	else
	{
		listen_port = m_pTorrentSessionSettings->valueInt("Torrent", "listen_port", 6103);
	}

	s_settings.use_disk_read_ahead = m_pTorrentSessionSettings->valueBool("Torrent", "use_disk_read_ahead", true);
	s_settings.disable_hash_checks = m_pTorrentSessionSettings->valueBool("Torrent", "disable_hash_checks", false);
	s_settings.peer_timeout = m_pTorrentSessionSettings->valueInt("Torrent", "peer_timeout", 120);
	s_settings.announce_to_all_tiers = m_pTorrentSessionSettings->valueBool("Torrent", "announce_to_all_tiers", true);
	s_settings.announce_to_all_trackers = m_pTorrentSessionSettings->valueBool("Torrent", "announce_to_all_trackers", true);
	s_settings.download_rate_limit = m_pTorrentSessionSettings->valueInt("Torrent", "download_rate_limit", 0);
	s_settings.upload_rate_limit = m_pTorrentSessionSettings->valueInt("Torrent", "upload_rate_limit", 0);
	s_settings.dht_upload_rate_limit = m_pTorrentSessionSettings->valueInt("Torrent", "dht_upload_rate_limit", 0);
	s_settings.ignore_limits_on_local_network = m_pTorrentSessionSettings->valueBool("Torrent", "ignore_limits_on_local_network", false);
	s_settings.local_upload_rate_limit = m_pTorrentSessionSettings->valueInt("Torrent", "local_upload_rate_limit", 0);
	s_settings.local_download_rate_limit = m_pTorrentSessionSettings->valueInt("Torrent", "local_download_rate_limit", 0);
	s_settings.rate_limit_utp = m_pTorrentSessionSettings->valueBool("Torrent", "rate_limit_utp", true);
	s_settings.torrent_connect_boost = 50;
	s_settings.unchoke_slots_limit = m_pTorrentSessionSettings->valueInt("Torrent", "unchoke_slots_limit", 8);
	s_settings.urlseed_wait_retry = m_pTorrentSessionSettings->valueInt("Torrent", "urlseed_wait_retry", 30);
	s_settings.listen_queue_size = m_pTorrentSessionSettings->valueInt("Torrent", "listen_queue_size", 30);
	s_settings.mixed_mode_algorithm = session_settings::peer_proportional;
	s_settings.max_peerlist_size = m_pTorrentSessionSettings->valueInt("Torrent", "max_peerlist_size", 0);
	s_settings.connections_limit = m_pTorrentSessionSettings->valueInt("Torrent", "connections_limit", 2000);
	s_settings.max_paused_peerlist_size = m_pTorrentSessionSettings->valueInt("Torrent", "max_paused_peerlist_size", 400);
	s_settings.seed_time_limit = m_pTorrentSessionSettings->valueInt("Torrent", "seed_time_limit", 0);
	s_settings.share_ratio_limit = m_pTorrentSessionSettings->valueFloat("Torrent", "share_ratio_limit");
	m_pTorrentSessionSettings->valueInt("Torrent", "file_allocation_mode", storage_mode_sparse);
	ipFilterFileName = m_pTorrentSessionSettings->valueString("Torrent", "ip_filter_filename", "");
	FILE* filter = fopen(ipFilterFileName.toLatin1().data(), "r");

	if (filter)
	{
		ip_filter fil;
		unsigned int a, b, c, d, e, f, g, h, flags;

		while (fscanf(filter, "%u.%u.%u.%u - %u.%u.%u.%u %u\n", &a, &b, &c, &d, &e, &f, &g, &h, &flags) == 9)
		{
			address_v4 start((a << 24) + (b << 16) + (c << 8) + d);
			address_v4 last((e << 24) + (f << 16) + (g << 8) + h);

			if (flags <= 127)
			{
				flags = ip_filter::blocked;
			}
			else
			{
				flags = 0;
			}

			fil.add_rule(start, last, flags);
		}

		m_pTorrentSession->set_ip_filter(fil);
		fclose(filter);
	}

	max_connections_per_torrent = m_pTorrentSessionSettings->valueInt("Torrent", "max_connections_per_torrent", 50);
	useProxy = m_pTorrentSessionSettings->valueBool("Torrent", "useProxy", false);

	if (useProxy)
	{
		ps.hostname = m_pTorrentSessionSettings->valueString("Torrent", "proxy_hostname").toUtf8().constData();
		ps.port = m_pTorrentSessionSettings->valueInt("Torrent", "proxy_port");
		ps.type = static_cast<proxy_settings::proxy_type>(m_pTorrentSessionSettings->valueInt("Torrent", "proxy_type"));
		ps.username = m_pTorrentSessionSettings->valueString("Torrent", "proxy_username").toUtf8().constData();
		ps.password = m_pTorrentSessionSettings->valueString("Torrent", "proxy_password").toUtf8().constData();
		m_pTorrentSession->set_proxy(ps);
	}

	s_settings.lock_files = m_pTorrentSessionSettings->valueBool("Torrent", "lock_files", false);
	s_settings.disk_io_read_mode = m_pTorrentSessionSettings->valueInt("Torrent", "disk_io_write_mode", 0);
	s_settings.disk_io_write_mode = m_pTorrentSessionSettings->valueInt("Torrent", "disk_io_write_mode", 0);
	s_settings.low_prio_disk = m_pTorrentSessionSettings->valueBool("Torrent", "low_prio_disk", false);
	s_settings.cache_size = m_pTorrentSessionSettings->valueInt("Torrent", "cache_size", 2048);
	s_settings.use_read_cache = m_pTorrentSessionSettings->valueBool("Torrent", "use_read_cache", s_settings.cache_size > 0);
	s_settings.cache_buffer_chunk_size = m_pTorrentSessionSettings->valueInt("Torrent", "cache_buffer_chunk_size", s_settings.cache_size / 100);
	s_settings.allowed_fast_set_size = m_pTorrentSessionSettings->valueInt("Torrent", "allowed_fast_set_size", 10);
	s_settings.read_cache_line_size = m_pTorrentSessionSettings->valueInt("Torrent", "read_cache_line_size", 40);
	s_settings.allow_reordered_disk_operations = m_pTorrentSessionSettings->valueBool("Torrent", "allow_reordered_disk_operations", true);
	s_settings.active_downloads = m_pTorrentSessionSettings->valueInt("Torrent", "active_downloads", -1);
	s_settings.active_limit = m_pTorrentSessionSettings->valueInt("Torrent", "active_limit", -1);
	s_settings.active_seeds = m_pTorrentSessionSettings->valueInt("Torrent", "active_seeds", -1);
	s_settings.choking_algorithm = session_settings::auto_expand_choker;
	s_settings.seed_choking_algorithm = session_settings::fastest_upload;
	s_settings.disk_cache_algorithm = session_settings::avoid_readback;
	s_settings.user_agent = "CuteTorrent ";
	s_settings.user_agent.append(Version::getVersionStr());
	s_settings.announce_double_nat = true;
	return s_settings;
}

pe_settings TorrentManager::readEncSettings()
{
	return m_pTorrentSession->get_pe_settings();
}

void TorrentManager::updateEncSettings(const pe_settings& settings)
{
	m_pTorrentSession->set_pe_settings(settings);
}

void TorrentManager::updateSettings(const session_settings& settings)
{
	m_pTorrentSession->set_settings(settings);
}

void TorrentManager::updateMaxConnectionsPerTorrent()
{
	int newValue = m_pTorrentSessionSettings->valueInt("Torrent", "max_connections_per_torrent");

	if (max_connections_per_torrent != newValue)
	{
		max_connections_per_torrent = newValue;
		int length = m_pTorrentStorrage->length();

		for (int i = 0; i < length; i++)
		{
			(*m_pTorrentStorrage.get()).at(i)->GetInternalHandle().set_max_connections(max_connections_per_torrent);
		}
	}
}

void TorrentManager::writeSettings()
{
	session_settings s_settings = m_pTorrentSession->settings();
	m_pTorrentSessionSettings->setValue("Torrent", "half_open_limit", s_settings.half_open_limit);
	m_pTorrentSessionSettings->setValue("Torrent", "allow_multiple_connections_per_ip", s_settings.allow_multiple_connections_per_ip);
	m_pTorrentSessionSettings->setValue("Torrent", "use_disk_read_ahead", s_settings.use_disk_read_ahead);
	m_pTorrentSessionSettings->setValue("Torrent", "disable_hash_checks", s_settings.disable_hash_checks);;
	m_pTorrentSessionSettings->setValue("Torrent", "peer_timeout", s_settings.peer_timeout);
	m_pTorrentSessionSettings->setValue("Torrent", "announce_to_all_tiers", s_settings.announce_to_all_tiers);
	m_pTorrentSessionSettings->setValue("Torrent", "download_rate_limit", s_settings.download_rate_limit);
	m_pTorrentSessionSettings->setValue("Torrent", "dht_upload_rate_limit", s_settings.dht_upload_rate_limit);
	m_pTorrentSessionSettings->setValue("Torrent", "ignore_limits_on_local_network", s_settings.ignore_limits_on_local_network);
	m_pTorrentSessionSettings->setValue("Torrent", "local_upload_rate_limit", s_settings.local_upload_rate_limit);
	m_pTorrentSessionSettings->setValue("Torrent", "local_download_rate_limit", s_settings.local_download_rate_limit);
	m_pTorrentSessionSettings->setValue("Torrent", "rate_limit_utp", s_settings.rate_limit_utp);
	m_pTorrentSessionSettings->setValue("Torrent", "seed_time_limit", s_settings.seed_time_limit);
	m_pTorrentSessionSettings->setValue("Torrent", "share_ratio_limit", QString::number(s_settings.share_ratio_limit));
	m_pTorrentSessionSettings->setValue("Torrent", "upload_rate_limit", s_settings.upload_rate_limit);
	m_pTorrentSessionSettings->setValue("Torrent", "unchoke_slots_limit", s_settings.unchoke_slots_limit);
	m_pTorrentSessionSettings->setValue("Torrent", "urlseed_wait_retry", s_settings.urlseed_wait_retry);
	m_pTorrentSessionSettings->setValue("Torrent", "listen_queue_size", s_settings.listen_queue_size);
	m_pTorrentSessionSettings->setValue("Torrent", "max_peerlist_size", s_settings.max_peerlist_size);
	m_pTorrentSessionSettings->setValue("Torrent", "max_paused_peerlist_size", s_settings.max_paused_peerlist_size);
	m_pTorrentSessionSettings->setValue("Torrent", "max_connections_per_torrent", max_connections_per_torrent);
	m_pTorrentSessionSettings->setValue("Torrent", "ip_filter_filename", ipFilterFileName);
	m_pTorrentSessionSettings->setValue("Torrent", "useProxy", useProxy);
	m_pTorrentSessionSettings->setValue("Torrent", "proxy_hostname", ps.hostname.c_str());
	m_pTorrentSessionSettings->setValue("Torrent", "proxy_port", ps.port);
	m_pTorrentSessionSettings->setValue("Torrent", "proxy_type", ps.type);
	m_pTorrentSessionSettings->setValue("Torrent", "proxy_username", ps.username.c_str());
	m_pTorrentSessionSettings->setValue("Torrent", "proxy_password", ps.password.c_str());
	m_pTorrentSessionSettings->setValue("Torrent", "lock_files", s_settings.lock_files);
	m_pTorrentSessionSettings->setValue("Torrent", "disk_io_read_mode", s_settings.disk_io_read_mode);
	m_pTorrentSessionSettings->setValue("Torrent", "disk_io_write_mode", s_settings.disk_io_write_mode);
	m_pTorrentSessionSettings->setValue("Torrent", "low_prio_disk", s_settings.low_prio_disk);
	m_pTorrentSessionSettings->setValue("Torrent", "cache_size", s_settings.cache_size);
	m_pTorrentSessionSettings->setValue("Torrent", "use_read_cache", s_settings.use_read_cache);
	m_pTorrentSessionSettings->setValue("Torrent", "allow_reordered_disk_operations", s_settings.allow_reordered_disk_operations);
	m_pTorrentSessionSettings->setValue("Torrent", "cache_buffer_chunk_size", s_settings.cache_buffer_chunk_size);
	m_pTorrentSessionSettings->setValue("Torrent", "allowed_fast_set_size", s_settings.allowed_fast_set_size);
	m_pTorrentSessionSettings->setValue("Torrent", "read_cache_line_size", s_settings.read_cache_line_size);
	m_pTorrentSessionSettings->setValue("Torrent", "active_downloads", s_settings.active_downloads);
	m_pTorrentSessionSettings->setValue("Torrent", "active_limit", s_settings.active_limit);
	m_pTorrentSessionSettings->setValue("Torrent", "active_seeds", s_settings.active_seeds);
	pe_settings enc_settings = m_pTorrentSession->get_pe_settings();
	m_pTorrentSessionSettings->setValue("Torrent", "in_enc_policy", enc_settings.in_enc_policy);
	m_pTorrentSessionSettings->setValue("Torrent", "out_enc_policy", enc_settings.out_enc_policy);
	m_pTorrentSessionSettings->setValue("Torrent", "allowed_enc_level", enc_settings.allowed_enc_level - 1);
	m_pTorrentSessionSettings->setValue("Torrent", "prefer_rc4", enc_settings.prefer_rc4);
}

void TorrentManager::SaveSession()
{
	if (!m_bIsSaveSessionInitiated)
	{
		killTimer(m_updateTimerID);
		//m_pTorrentSession->set_alert_dispatch(NULL);
		m_bIsSaveSessionInitiated = true;
		writeSettings();
		num_outstanding_resume_data = 0;
		std::vector<torrent_status> temp;
		m_pTorrentSession->pause();
		m_pTorrentSession->get_torrent_status(&temp, &yes, 0xffffffff);

		for (std::vector<torrent_status>::iterator i = temp.begin();
		     i != temp.end(); ++i)
		{
			torrent_status& st = *i;

			if (!st.handle.is_valid())
			{
				continue;
			}

			if (!st.has_metadata)
			{
				continue;
			}

			/*if (!st.handle.need_save_resume_data())
			{
				continue;
			}*/
			st.handle.save_resume_data();
			++num_outstanding_resume_data;
		}

		QString dataDir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);

		while (num_outstanding_resume_data > 0)
		{
			std::deque<alert*> alerts;
			m_pTorrentSession->pop_alerts(&alerts);

			for (std::deque<alert*>::iterator i = alerts.begin()
				     , end(alerts.end()); i != end; ++i)
			{
				boost::scoped_ptr<alert> a(*i);

				if (alert_cast<save_resume_data_failed_alert>(a.get()))
				{
					--num_outstanding_resume_data;
					continue;
				}

				save_resume_data_alert const* rd = alert_cast<save_resume_data_alert>(*i);

				if (!rd)
				{
					continue;
				}

				--num_outstanding_resume_data;

				if (!rd->resume_data)
				{
					continue;
				}

				torrent_handle h = rd->handle;
				std::vector<char> out;
				entry& e = *rd->resume_data;
				QString info_hash = QString::fromStdString(to_hex(h.info_hash().to_string()));
				Torrent* torrent = m_pTorrentStorrage->getTorrent(info_hash);

				if (torrent != NULL)
				{
					e["torrent_group_id"] = torrent->GetGroupUid().toString().toUtf8().data();
					e["torrent_name"] =
#if LIBTORRENT_VERSION_NUM >= 10000
						h.status(torrent_handle::query_name).name;
#else
					    h.name();
#endif
					bool isSeeding = torrent->isSeeding();
					e["is_previous_seed"] = isSeeding ? 1 : 0;
				}

				bencode(back_inserter(out), e);

				if (save_file((StaticHelpers::CombinePathes(dataDir, "BtSessionData", info_hash + ".resume").toUtf8().data()), out) < 0)
				{
					qDebug() << "save_file failed";
				}
			}
		}

		{
			entry session_state;
			m_pTorrentSession->save_state(session_state);
			int activeSessionTime = m_lastActiveTime + m_elapsedTimer.elapsed() / 1000;
			if (activeSessionTime < 360000)
			{
				
				session_state["active_session_time"] = activeSessionTime;
			}
			else
			{
				session_state["active_session_time"] = 0;
			}
			std::vector<char> out;
			bencode(back_inserter(out), session_state);
			QString path = StaticHelpers::CombinePathes(dataDir, "BtSessionData", "actual.state");
			save_file(path.toUtf8().data(), out);
		}

		m_pTorrentSession->abort();
	}
}

int TorrentManager::save_file(std::string const& filename, std::vector<char>& v)
{
	using namespace libtorrent;
	file f;
	error_code ec;

	if (!f.open(filename, file::write_only, ec))
	{
		qCritical() << "file.open failed" << QString::fromStdString(ec.message()) << qPrintable(filename.c_str());
		return -1;
	}

	if (ec)
	{
		qCritical() << "file.open failed" << QString::fromStdString(ec.message()) << qPrintable(filename.c_str());
		return -1;
	}

	file::iovec_t b = {&v[0], v.size()};
	int64_t written = f.writev(0, &b, 1, ec);

	if (written != int(v.size()))
	{
		return -3;
	}

	if (ec)
	{
		return -3;
	}

	return 0;
}

/*
void TorrentManager::UpdatePathResumeAndLinks()
{
    QString dataDir;
#ifdef Q_WS_MAC
    dataDir="/Library/CuteTorrent/";
#else
    dataDir = QApplication::applicationDirPath()+QDir::separator();
#endif
    QFile pathDataFile(dataDir+"CT_DATA/path.resume");
    if (pathDataFile.open(QFile::WriteOnly))
    {
        for (QMap<QString,QPair<QString,QString> >::iterator i=save_path_data.begin();i!=save_path_data.end();++i)
        {
            pathDataFile.write((i.key()+"|"+i.value().first+"|"+i.value().second+"\n").toUtf8());
        }
        pathDataFile.close();
    }
    else
    {
        CustomMessageBox::critical(0,"Error","CT_DATA/path.resume couldn't be opened");
    }


}
*/

TorrentManager::~TorrentManager()
{
	SaveSession();
}

opentorrent_info* TorrentManager::GetTorrentInfo(QString filename, error_code& ec)
{
	boost::shared_ptr<torrent_info> ti(new torrent_info(filename.toUtf8().data(), ec));

	if (ec != 0)
	{
		return NULL;
	}

	torrent_handle foundTorrent = m_pTorrentSession->find_torrent(ti->info_hash());
	opentorrent_info* info = new opentorrent_info;
	info->size = ti->total_size();
	info->name = QString::fromUtf8(ti->name().c_str());
	info->describtion = QString::fromUtf8(ti->comment().c_str());
	info->torrentInfo = ti;
	info->baseSuffix = StaticHelpers::GetBaseSuffix(ti->files());
	info->infoHash = QString::fromStdString(to_hex(ti->info_hash().to_string()));

	if (foundTorrent.is_valid())
	{
		ec.assign(errors::duplicate_torrent, get_libtorrent_category());
	}

	return info;
}

openmagnet_info* TorrentManager::GetTorrentInfo(const torrent_handle& handle)
{
	if (handle.is_valid())
	{
		openmagnet_info* info = new openmagnet_info;
#if LIBTORRENT_VERSION_NUM >= 10000
		boost::shared_ptr<torrent_info const> ti = handle.torrent_file();
		info->size = ti->total_size();
		info->name = QString::fromUtf8(ti->name().c_str());
		info->describtion = QString::fromUtf8(ti->comment().c_str());
		info->torrentInfo = ti;
		info->infoHash = QString::fromStdString(to_hex(ti->info_hash().to_string()));
#else
		const torrent_info ti = handle.get_torrent_info();
		info->size = ti.total_size();
		info->name = QString::fromUtf8(ti.name().c_str());
		info->describtion = QString::fromUtf8(ti.comment().c_str());
		info->files = ti.files();
		info->infoHash = QString::fromStdString(to_hex(ti.info_hash().to_string()));
#endif
		info->handle = handle;
		info->baseSuffix = StaticHelpers::GetBaseSuffix(ti->files());
		return info;
	}

	return NULL;
}

void TorrentManager::dispatchPendingAlerts()
{
	std::vector<alert*> alerts;
	m_pTorrentSession->pop_alerts(&alerts);

	foreach(alert* const a, alerts)
	{
		handle_alert(a);
	}
}

void TorrentManager::RemoveTorrent(QString infoHash, bool delFiles)
{
	m_pTorrentStorrage->remove(infoHash);
	emit TorrentRemoved(infoHash);
	try
	{
		char out[20] = {0};
		from_hex(infoHash.toUtf8().data(), infoHash.length(), out);
		sha1_hash hash(out);
		torrent_handle h = m_pTorrentSession->find_torrent(hash);

		if (h.is_valid())
		{
			m_pTorrentSession->remove_torrent(h, delFiles ? session::delete_files : 0);
		}
	}
	catch (libtorrent_exception e)
	{
		qCritical() << e.what();
	}
	catch (...)
	{
		qCritical() << "Not a libtorrent exception caught";
	}
	QString dataDir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
	QString resumePath = StaticHelpers::CombinePathes(dataDir, "BtSessionData", infoHash + ".resume");
	QString torrentPath = StaticHelpers::CombinePathes(dataDir, "BtSessionData", infoHash + ".torrent");
	QFile resumeDataFile(resumePath);
	QFile torrentFile(torrentPath);


	if (resumeDataFile.exists())
	{
		resumeDataFile.setPermissions(QFile::ReadOther | QFile::WriteOther);
		if (!resumeDataFile.remove())
		{
			CustomMessageBox::critical(NULL, "Error", QString("Failed deleting file %1.\nError: %2").arg(torrentPath, resumeDataFile.errorString()));
		}
	}

	if (torrentFile.exists())
	{
		torrentFile.setPermissions(QFile::ReadOther | QFile::WriteOther);
		if (!torrentFile.remove())
		{
			CustomMessageBox::critical(NULL, "Error", QString("Failed deleting file %1.\nError: %2").arg(torrentPath, torrentFile.errorString()));
		}
	}
}

void TorrentManager::OnDownloadReady(QUrl url, QTemporaryFile* pUnsafeFile)
{
	boost::scoped_ptr<QTemporaryFile> pFile(pUnsafeFile);

	if (!pFile->isOpen())
	{
		if (!pFile->open())
		{
			qCritical() << "Unable to open file" << pFile->fileName();
			return;
		}
	}

	QByteArray data = pFile->readAll();
	QByteArray uncompresssed = StaticHelpers::gUncompress(data);
	QString dataDir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
	QString geoIpPath = StaticHelpers::CombinePathes(dataDir, "GeoIP.dat");
	QFile file(geoIpPath);

	if (file.open(QFile::WriteOnly))
	{
		file.write(uncompresssed);
		file.close();
		m_pTorrentSession->load_country_db(geoIpPath.toUtf8().data());
	}
	else
	{
		qCritical() << "Unable to write GeoIP.dat" << geoIpPath;
	}
}

bool TorrentManager::shouldRate()
{
	return m_shouldRate;
}




int TorrentManager::GetSessionDwonloadRate()
{
	return m_pTorrentSession->status().download_rate;
}

QString TorrentManager::GetSessionDownloadSpeed()
{
	try
	{
		return StaticHelpers::toKbMbGb(GetSessionDwonloadRate(), true);
	}
	catch (...)
	{
		return "";
	}
}

int TorrentManager::GetSessionUploadRate()
{
	return m_pTorrentSession->status().upload_rate;
}

QString TorrentManager::GetSessionUploadSpeed()
{
	try
	{
		return StaticHelpers::toKbMbGb(GetSessionUploadRate(), true);
	}
	catch (...)
	{
		return "";
	}
}

QString TorrentManager::GetSessionDownloaded()
{
	try
	{
		return StaticHelpers::toKbMbGb(m_pTorrentSession->status().total_download);
	}
	catch (...)
	{
		return "";
	}
}

QString TorrentManager::GetSessionUploaded()
{
	try
	{
		return StaticHelpers::toKbMbGb(m_pTorrentSession->status().total_upload);
	}
	catch (...)
	{
		return "";
	}
}

torrent_handle TorrentManager::ProcessMagnetLink(QString link, error_code& ec)
{
	add_torrent_params add_info;
	torrent_handle h;
	parse_magnet_uri(link.toStdString(), add_info, ec);

	if (ec != NULL)
	{
		CustomMessageBox::critical(NULL, "ProcessMagnetLink Error", StaticHelpers::translateLibTorrentError(ec));
		return h;
	}

	QString dataDir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
	QString infoHash = QString::fromStdString(to_hex(add_info.info_hash.to_string()));
	QString filename = StaticHelpers::CombinePathes(dataDir, "BtSessionData", infoHash + ".resume");
	std::vector<char> buf;

	if (StaticHelpers::LoadFile(filename, buf))
	{
#if LIBTORRENT_VERSION_NUM >= 10000
		add_info.resume_data = buf;
#else
		add_info.resume_data = &buf;
#endif
	}

	add_info.flags = add_torrent_params::flag_duplicate_is_error | add_torrent_params::flag_update_subscribe | add_torrent_params::flag_auto_managed;
	h = m_pTorrentSession->add_torrent(add_info, ec);

	if (ec)
	{
		return h;
	}
	h.set_priority(255);
	while (!h.status().has_metadata)
	{
		sleep(100);
	}

	h.auto_managed(false);
	h.pause();
	return h;
}

void TorrentManager::CancelMagnetLink(QString link)
{
	add_torrent_params add_info;
	error_code ec;
	parse_magnet_uri(link.toStdString(), add_info, ec);

	if (ec)
	{
		CustomMessageBox::critical(NULL, "Error", StaticHelpers::translateLibTorrentError(ec));
		return;
	}

	torrent_handle h = m_pTorrentSession->find_torrent(add_info.info_hash);
	QString dataDir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
	QString infoHash = QString::fromStdString(to_hex(add_info.info_hash.to_string()));
	QString magnet_path = StaticHelpers::CombinePathes(dataDir, "BtSessionData", infoHash + ".torrent");

	if (QFile::exists(magnet_path))
	{
		QFile::remove(magnet_path);
	}

	if (h.is_valid())
	{
		m_pTorrentSession->remove_torrent(h, session::delete_files);
	}
}

bool TorrentManager::AddMagnet(torrent_handle h, QString& SavePath, TorrentGroup* group, QMap<QString, quint8> filePriorities, AddTorrentFlags flags)
{
	if (!filePriorities.isEmpty())
	{
		std::vector<int> filepriorities;
		file_storage storrage =
#if LIBTORRENT_VERSION_NUM >= 10000
			h.torrent_file()->files();
#else
		    h.get_torrent_info().files();
#endif

		for (int i = 0; i < storrage.num_files(); i++)
		{
			file_entry file = storrage.at(i);
			QString filePath = QDir::toNativeSeparators(QString::fromUtf8(file.path.c_str()));

			if (filePriorities.contains(filePath))
			{
				filepriorities.push_back(filePriorities[filePath]);
			}
			else
			{
				filepriorities.push_back(4);
			}
		}

		h.prioritize_files(filepriorities);
	}

	std::string save_path =
#if LIBTORRENT_VERSION_NUM >= 10000
		h.status(torrent_handle::query_save_path).save_path;
#else
	    h.save_path();
#endif

	if (SavePath != QString::fromUtf8(save_path.c_str()))
	{
		h.move_storage(SavePath.toUtf8().data());
	}
	h.set_priority(128);
	if (!flags.testFlag(PAUSED_MODE))
	{
		h.resume();
	}

	if (flags.testFlag(SUPER_SEED_MODE))
	{
		h.super_seeding(true);
	}

	if (flags.testFlag(SEQUENTIAL_MODE))
	{
		h.set_sequential_download(true);
	}

	Torrent* current = new Torrent(h.status(), group);
	m_pTorrentStorrage->append(current);

	if (h.is_valid())
	{
		emit TorrentAdded(current);
	}

	return true;
}

void TorrentManager::StartAllTorrents()
{
	std::vector<torrent_handle> torrent_handels = m_pTorrentSession->get_torrents();

	for (int i = 0; i < torrent_handels.size(); i++)
	{
		torrent_handle hTor = torrent_handels.at(i);

		if (hTor.is_valid() && hTor.status().paused)
		{
			hTor.resume();
		}
	}
}

void TorrentManager::PauseAllTorrents()
{
	std::vector<torrent_handle> torrent_handels = m_pTorrentSession->get_torrents();

	for (int i = 0; i < torrent_handels.size(); i++)
	{
		torrent_handle tHandle = torrent_handels.at(i);

		if (tHandle.is_valid() && !tHandle.status().paused)
		{
			tHandle.auto_managed(false);
			tHandle.pause();
		}
	}
}

QString TorrentManager::GetSessionDHTstate()
{
	session_status ses_state = m_pTorrentSession->status();

	if (m_pTorrentSession->is_dht_running())
	{
		return QString::number(ses_state.dht_nodes);
	}
	else
	{
		return "Disabled";
	}
}

void TorrentManager::SetUlLimit(int val)
{
	session_settings settings = m_pTorrentSession->settings();
	settings.upload_rate_limit = val;
	m_pTorrentSession->set_settings(settings);
}

void TorrentManager::SetDlLimit(int val)
{
	session_settings settings = m_pTorrentSession->settings();
	settings.download_rate_limit = val;
	m_pTorrentSession->set_settings(settings);
}

int TorrentManager::GetDownloadLimit()
{
	session_settings settings = m_pTorrentSession->settings();
	return settings.download_rate_limit;
}

int TorrentManager::GetUploadLimit()
{
	session_settings settings = m_pTorrentSession->settings();
	return settings.upload_rate_limit;
}

Torrent* TorrentManager::GetTorrentByInfoHash(const sha1_hash& hash)
{
	QString infoHash = QString::fromStdString(to_hex(hash.to_string()));

	if (m_pTorrentStorrage->hasTorrent(infoHash))
	{
		return (*m_pTorrentStorrage)[infoHash];
	}

	return NULL;
}

Torrent* TorrentManager::GetTorrentByInfoHash(QString infoHash)
{
	if (m_pTorrentStorrage->hasTorrent(infoHash))
	{
		return (*m_pTorrentStorrage)[infoHash];
	}

	return NULL;
}
#if LIBTORRENT_VERSION_NUM >= 10000
void TorrentManager::AddPortMapping(session::protocol_type type, ushort external_port, ushort local_port)
{
	m_pTorrentSession->add_port_mapping(type, external_port, local_port);
}
#else
void TorrentManager::AddPortMapping(upnp::protocol_type type, ushort external_port, ushort local_port)
{
	if (m_pUpnp != NULL)
	{
		m_pUpnp->add_mapping(type, external_port, local_port);
	}
}

#endif

void TorrentManager::RereshPortForwardingSettings()
{
	if (m_pTorrentSessionSettings->valueBool("Torrent", "use_port_forwarding", true))
	{
#if LIBTORRENT_VERSION_NUM >= 10000
		m_pTorrentSession->start_upnp();
#else
		m_pUpnp = m_pTorrentSession->start_upnp();
#endif
		m_pTorrentSession->start_natpmp();
	}
	else
	{
		m_pTorrentSession->stop_upnp();
		m_pTorrentSession->stop_natpmp();
	}
}

void TorrentManager::RefreshExternalPeerSettings()
{
	if (m_pTorrentSessionSettings->valueBool("Torrent", "use_lsd", true))
	{
		m_pTorrentSession->start_lsd();
	}
	else
	{
		m_pTorrentSession->stop_lsd();
	}

	if (m_pTorrentSessionSettings->valueBool("Torrent", "use_dht", true))
	{
		m_pTorrentSession->add_dht_router(make_pair(
			std::string("router.bittorrent.com"), 8991));
		m_pTorrentSession->add_dht_router(make_pair(
			std::string("router.utorrent.com"), 6881));
		m_pTorrentSession->add_dht_router(make_pair(
			std::string("dht.transmissionbt.com"), 6881));
		m_pTorrentSession->add_dht_router(make_pair(
			std::string("dht.aelitis.com"), 6881));
		m_pTorrentSession->add_dht_router(make_pair(
			std::string("dht.cutetorrent.info"), 6881));
		/*if (m_pTorrentSessionSettings->valueBool("Torrent", "use_special_dht_port", false))
		{
			dht_settings defaultSettings;
			defaultSettings.service_port = m_pTorrentSessionSettings->valueInt("Torrent", "special_dht_port", 6881);
			m_pTorrentSession->set_dht_settings(defaultSettings);;
		}*/
		m_pTorrentSession->start_dht();
	}
	else if (m_pTorrentSession->is_dht_running())
	{
		m_pTorrentSession->stop_dht();
	}
}

