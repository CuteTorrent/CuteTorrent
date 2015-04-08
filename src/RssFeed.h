#ifndef _RSS_FEED_INCLUDED_
#define _RSS_FEED_INCLUDED_
#include <QUuid>
#include <Qurl>
#include "RssCommon.h"
#include <QTime>
class QNetworkAccessManager;
class NetworkDiskCache;
class QNetworkReply;
class QTimer;
class QDataStream;
class RssFeed : public QObject
{
	friend RssParser;
	friend QDataStream& operator <<(QDataStream& out, const RssFeed& any);
	friend QDataStream& operator >>(QDataStream& out, RssFeed& any);
	Q_OBJECT
private:
	QTimer* m_pUpdateTimer;
	QUuid m_uid;
	QUrl m_url;
	QNetworkAccessManager* m_pNetManager;
	QTime m_elapsedTime;
	QString m_title, m_link, m_description, m_errorString, m_customDisplayName;
	bool m_isUpdating;
	QList<RssItem> m_rssItemsByDate;
	QHash<QString,RssItem> m_rssItems;
	int m_ttl, m_unreadCount;
	void calculateUnreadCount();
signals:
	void FeedChanged(QUuid);
public:
	RssFeed(QUrl url, QUuid uid);
	QString displayName(bool noUnreadCount = false);
	void setDisplayName(QString value);
	QUrl url();
	QUuid uid();
	QString title();
	QString description() const;
	int ttl();
	int unreadCount() const { return m_unreadCount; }
	bool isUpdating() const { return m_isUpdating; }
	QString error() const { return m_errorString; }
	int next_update();
	QList<RssItem> GetFeedItems();
	void setItemUnread(bool param1, QString guid);
	RssItem GetFeedItem(QString guid);
public slots:
	void Update();
private slots:
	void resourceLoaded(QNetworkReply*);
	
};
Q_DECLARE_METATYPE(RssFeed*)
#endif