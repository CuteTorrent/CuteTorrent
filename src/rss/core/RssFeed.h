#ifndef _RSS_FEED_INCLUDED_
#define _RSS_FEED_INCLUDED_
#include <QUuid>
#include <QUrl>
#include <QTime>
#include <QMetaType>
#include "qnetworkcookie.h"
#include <helpers/DT_mounter.h>
class RssParser;
class QNetworkAccessManager;
class NetworkDiskCache;
class QNetworkReply;
class QTimer;
class QDataStream;
class RssItem;
class RssParser;

class RssFeed : public QObject
{
	Q_OBJECT

	friend RssParser;
	friend QDataStream& operator <<(QDataStream& out, const RssFeed& any);
	friend QDataStream& operator >>(QDataStream& out, RssFeed& any);

	QTimer* m_pUpdateTimer;
	QUuid m_uid;
	QUrl m_url;
	QNetworkAccessManager* m_pNetManager;
	QTime m_elapsedTime;
	QString m_title, m_link, m_description, m_errorString, m_customDisplayName;
	bool m_isUpdating;
	QList<RssItem*> m_rssItemsByDate;
	QHash<QString, RssItem*> m_rssItems;
	QHash<QString, QString> m_coookies;
	int m_ttl, m_unreadCount, m_customTtl;
	QList<QNetworkCookie> buildCookies() const;
	signals:
	void FeedChanged(QUuid);
public:
	RssFeed(QUrl url, QUuid uid);
	void UpdateUnreadCount();
	QString displayName(bool noUnreadCount = false);
	void setDisplayName(QString value);
	QList<QNetworkCookie> coookies() const;
	void setCoookies(const QHash<QString, QString>& coookiesValue);
	QUrl url();
	QUuid uid();
	QString title();
	QString description() const;
	int ttl();
	void setTll(int value);
	int unreadCount() const;
	bool isUpdating() const;
	QString error() const;
	int nextUpdate();
	QList<RssItem*> GetFeedItems();
	void setItemUnread(bool unreadValue, QString guid);
	RssItem* GetFeedItem(QString guid);
public slots:
	void Update();
private slots:
	void resourceLoaded(QNetworkReply*);
};

Q_DECLARE_METATYPE(RssFeed*)
#endif

