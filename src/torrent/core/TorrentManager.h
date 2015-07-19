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
#ifndef TOR_MANAGER_H
#define TOR_MANAGER_H
#include <vector>
#include <QMutex>
#include <QWaitCondition>
#include "libtorrent/error_code.hpp"
#include "libtorrent/session_settings.hpp"
#include "libtorrent/torrent_handle.hpp"
#include "libtorrent/version.hpp"
#include "libtorrent/upnp.hpp"
#ifdef Q_WS_WIN
#pragma warning (disable: 4005)
#pragma warning (disable: 4100)
#pragma warning (disable: 4267)
#endif
#include <boost/bind.hpp>

#include "libtorrent/alert_types.hpp"
#include "libtorrent/session.hpp"

class QTemporaryFile;
using boost::bind;
using namespace libtorrent;


#include "defs.h"
#include "ServiceCommon.h"
#include "Singleton.h"
#include "TorrentCommon.h"
#include <QVariant>
#include <QVector>
#include <QMap>
#include <QUrl>
class QApplicationSettings;
class QTorrentDisplayModel;
class Torrent;
namespace libtorrent
{
class alert;
}  // namespace libtorrent
struct openmagnet_info;
struct opentorrent_info;



class TorrentManager : public QObject, public Singleton<TorrentManager>
{
	friend class Singleton<TorrentManager>;
	Q_OBJECT
signals:
	void AddTorrentGui(Torrent*);
	void Notify(int, QString, QVariant);
	void initCompleted();
	void TorrentRemove(QString);
	void OnNewFeed();
	void OnFeedDeleted();
	void OnNewFeedItem();
	void OnFeedChanged();
protected:
	bool MoveFiles(QString oldStyleDirPath, QString newStyleDirPath);
	void timerEvent(QTimerEvent *) override;


private:
	TorrentManager();

#if LIBTORRENT_VERSION_NUM < 10000
	upnp* m_pUpnp;
#endif
	FileDownloaderPtr m_pFileDownloader;
	int num_outstanding_resume_data;
	bool m_bIsSaveSessionInitiated;
	void handle_alert(alert*);
	void writeSettings();
	TorrentStorragePtr m_pTorrentStorrage;
	session* m_pTorrentSession;
	QApplicationSettingsPtr m_pTorrentSessionSettings;
	int save_file(std::string const& filename, std::vector<char>& v);
	QString DTInstallPath;
	int max_connections_per_torrent;
	int listen_port;
	QString ipFilterFileName;
	bool useProxy;
	proxy_settings ps;
	NotificationSystemPtr m_pNotificationSys;
	QMutex m_alertMutex;
	QWaitCondition m_alertsWaitCondition;
	QVector<alert*> m_alerts;
	void collectAlerts(std::auto_ptr<alert> a);
	void getPendingAlerts(QVector<alert*>& out);
private slots:
	void dispatchPendingAlerts();
public slots:
	void RemoveTorrent(QString InfoHash, bool delFiles = false);
	void OnDownloadReady(QUrl, QTemporaryFile*);
public:
	void UpdateTorrents();
	enum AddTorrentFlag
	{
		SEQUENTIAL_MODE = 1,
		SEED_MODE = 2,
		PAUSED_MODE = 4,
		SUPER_SEED_MODE = 8
	};
	Q_DECLARE_FLAGS(AddTorrentFlags, AddTorrentFlag)
	void SaveSession();
	void RefreshExternalPeerSettings();
	void InitSession();
	session_settings readSettings();
	pe_settings readEncSettings();
	void updateEncSettings(const pe_settings& settings);
	void updateSettings(const session_settings& settings);
	void updateMaxConnectionsPerTorrent();
	QString GetSessionDownloadSpeed();
	QString GetSessionUploadSpeed();
	QString GetSessionDownloaded();
	QString GetSessionUploaded();
	QString GetSessionDHTstate();
	std::vector<torrent_status> GetTorrents();
	opentorrent_info* GetTorrentInfo(QString filename, error_code& ec);
	openmagnet_info* GetTorrentInfo(const torrent_handle& handle);

	bool AddMagnet(torrent_handle h, QString SavePath, QString group = "", QMap< QString, quint8> filepriorities = QMap<QString, quint8>(), AddTorrentFlags flags = 0);
	bool AddTorrent(QString path, QString name, QString save_path, error_code& ec, QMap<QString, quint8> filepriorities = QMap<QString, quint8>(), QString group = "", AddTorrentFlags flags = 0);
	~TorrentManager();
#if LIBTORRENT_VERSION_NUM >= 10000
	void AddPortMapping(session::protocol_type type, ushort sourcePoert, ushort destPort);
#else
	void AddPortMapping(upnp::protocol_type type, ushort sourcePoert, ushort destPort);
#endif
	torrent_handle ProcessMagnetLink(QString link, error_code& ec);
	void CancelMagnetLink(QString link);
	void StartAllTorrents();
	void PauseAllTorrents();
	void SetUlLimit(int val);
	void SetDlLimit(int val);
	int GetDownloadLimit();
	int GetUploadLimit();
	Torrent* GetTorrentByInfoHash(const sha1_hash& hash);
	Torrent* GetTorrentByInfoHash(QString hash);
	void RereshPortForwardingSettings();
};
Q_DECLARE_METATYPE(opentorrent_info)

#endif
