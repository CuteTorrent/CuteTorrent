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

#include "libtorrent/alert_types.hpp"
#include "libtorrent/session.hpp"
#include <TorrentGroup.h>

class QTemporaryFile;
using namespace libtorrent;


#include "ServiceCommon.h"
#include "Singleton.h"
#include "TorrentCommon.h"
#include <QStringList>
#include <QVariant>
#include <QVector>
#include <QMap>
#include <QSet>
#include <QTime>
#include <QUrl>
#include "defs.h"
class QApplicationSettings;
class QTorrentDisplayModel;
class Torrent;

namespace libtorrent
{
	class alert;
} // namespace libtorrent

class TorrentManager : public QObject, public Singleton<TorrentManager>
{
	friend class Singleton<TorrentManager>;
	Q_OBJECT
signals:
	void TorrentAdded(Torrent*);
	void Notify(int, QString, QVariant);
	void TorrentRemoved(QString);
	void TorrentsChanged(QSet<QString> infohashes);
	void FileRenameCompleted();
protected:
	bool MoveFiles(QString oldStyleDirPath, QString newStyleDirPath) const;
	void timerEvent(QTimerEvent*) override;
	TorrentManager();

private:
	uint64_t counters_values[counters::num_counters];
	bool m_shouldRate;
	int m_lastActiveTime;
	QTime m_elapsedTimer;
#if LIBTORRENT_VERSION_NUM < 10000
	upnp* m_pUpnp;
#endif
	int num_outstanding_resume_data;
	bool m_bIsSaveSessionInitiated;
	int m_updateTimerID;
	void handle_alert(alert*);
	void writeSettings();
	TorrentStorragePtr m_pTorrentStorrage;
	session* m_pTorrentSession;
	QApplicationSettingsPtr m_pTorrentSessionSettings;
	int save_file(std::string const& filename, std::vector<char>& v);
	int max_connections_per_torrent;
	int listen_port;
	QString ipFilterFileName;
	bool useProxy;
	aux::proxy_settings ps;
	NotificationSystemPtr m_pNotificationSys;
	QMutex m_alertMutex;
	QWaitCondition m_alertsWaitCondition;
	PowerManagementPtr m_powerManagement;
	QHash<int, QHash<QString, qint64>> m_handeledAlerts;
private slots:
	void dispatchPendingAlerts();
public slots:
	void RemoveTorrent(QString InfoHash, bool delFiles = false);
public:
	bool shouldRate();
	int GetSessionDwonloadRate();

	enum AddTorrentFlag
	{
		SEQUENTIAL_MODE = 1,
		SEED_MODE = 2,
		PAUSED_MODE = 4,
		SUPER_SEED_MODE = 8,
		SKIP_ROOT_DIR = 16
	};

	Q_DECLARE_FLAGS(AddTorrentFlags, AddTorrentFlag)
	void SaveSession();
	void RefreshExternalPeerSettings();
	void InitSession(boost::function<void(int, QString)>);
	settings_pack readSettings();
	
	
	void updateSettings(const settings_pack& settings);
	void updateMaxConnectionsPerTorrent();
	QString GetSessionDownloadSpeed();
	int GetSessionUploadRate();
	QString GetSessionUploadSpeed();
	QString GetSessionDownloaded();
	QString GetSessionUploaded();
	QString GetSessionDHTstate();
	std::vector<torrent_status> GetTorrents();
	opentorrent_info* GetTorrentInfo(QString filename, error_code& ec);
	openmagnet_info* GetTorrentInfo(const torrent_handle& handle);

	bool AddMagnet(torrent_handle h, QString& SavePath, TorrentGroup* group = NULL, QMap<QString, quint8> filepriorities = QMap<QString, quint8>(), AddTorrentFlags flags = 0);
	Torrent* AddTorrent(QString& path, QString& save_path, error_code& ec, QString name = "", QMap<QString, quint8> filepriorities = QMap<QString, quint8>(), TorrentGroup* group = NULL,
	                    AddTorrentFlags flags = 0);
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
Q_DECLARE_METATYPE(QSet<QString>)
#endif

