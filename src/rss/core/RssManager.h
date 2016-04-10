#ifndef _RSS_MANAGER_INCLUDED_
#define _RSS_MANAGER_INCLUDED_

#include <QHash>
#include <QUrl>
#include <QUuid>
#include "RssDownloadRule.h"
#include "ServiceCommon.h"
#include "defs.h"
#include "Singleton.h"
class RssFeed;
class QApplicationSettings;
class RssItem;

class RssManager : public QObject, public Singleton<RssManager>
{
	friend class Singleton<RssManager>;
	Q_OBJECT
public:

	QList<RssFeed*> feeds();
	RssFeed* findFeed(const QUuid& uid);
	void addDownloadRule(RssDownloadRule* rule);
	void removeDownloadRule(const QUuid& uid);
	RssDownloadRule* findDownloadRule(const QUuid& uid);
	void updateDownloadRule(RssDownloadRule* rule);
	QList<RssDownloadRule*> downloadRules();
	~RssManager();
	void SaveFeeds();
	void SaveDownloadRules();
private:
	struct TorrentDownloadInfo
	{
		QUrl torrentUrl;
		RssDownloadRule* downloadRule;
		QUuid rssFeedId;
		QString rssItemId;
	};

	RssManager(QObject* parent = NULL);
	FileDownloaderPtr m_pTorrentDownloader;
	NotificationSystemPtr m_pNotificationSystem;
	QList<RssFeed*> m_pFeeds;
	QHash<QUrl, TorrentDownloadInfo> m_activeTorrentDownloads;
	QHash<QUuid, RssDownloadRule*> m_downloadRules;
	QApplicationSettingsPtr m_pSettings;
	void downloadRssItem(RssItem* rssItem, RssFeed* pFeed, RssDownloadRule* rule);
	QString gessSavePath(RssDownloadRule* downloadRule, QString base_suffix);
	QMap<QString, quint8> getFilePriorities(TorrentDownloadInfo downloadInfo, file_storage fileStorage);
public slots:
	void removeFeed(const QUuid& uid);
	RssFeed* addFeed(QUrl url, bool& isNew);
	void LoadFeeds();
	void LoadDownloadRules();
private slots:
	void onFeedChanged(QUuid);

	void onTorrentDownloaded(QUrl url, QTemporaryFile* pFile);
	void onTorrentDownloadError(QUrl url, QString error);
	void onMagnetError(QString);
	void onDownloadMetadataCompleted(openmagnet_info);
	signals:
	void Notify(int, QString, QVariant);
	void FeedChanged(QUuid);
	void FeedRemoved(QUuid);
};
#endif

