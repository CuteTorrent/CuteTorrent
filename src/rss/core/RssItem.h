#ifndef _RSS_ITEM_INCLUDED_
#define _RSS_ITEM_INCLUDED_
#include <QString>
#include <QDateTime>
#include <QObject>
class RssFeed;

class RssItem : public QObject
{
	friend QDataStream& operator <<(QDataStream& out, const RssItem* any);
	friend QDataStream& operator >>(QDataStream& out, RssItem* any);
	Q_OBJECT
	QString m_title;
	QString m_description;
	QString m_category;
	QString m_descriptionLink;
	QString m_torrentUrl;
	QString m_magnetUrl;
	QString m_guid;
	QString m_infoHash;
	QString m_downloadingTorrent;
	QString m_author;
	QDateTime m_pubDate;
	RssFeed* m_pRssFeed;
	quint64 m_size;
	quint32 m_peers;
	quint32 m_seeds;
	bool m_unread;
//	Q_DISABLE_COPY(RssItem)
	RssItem(const RssItem& other);
public:

	RssItem(RssFeed* parent);
	RssFeed* rssFeed() const;
	QString title() const;
	void setTitle(const QString& value);
	QString description() const;
	void setDescription(const QString& value);
	QString category() const;
	void setCategory(const QString& value);
	QString describtionLink() const;
	void setDescribtionLink(const QString& value);
	QString torrentUrl() const;
	void setTorrentUrl(const QString& value);
	QString magnetUrl() const;
	void setMagnetUrl(const QString& value);
	QString guid() const;
	void setGuid(const QString& value);
	QString infoHash() const;
	void setInfoHash(const QString& value);
	QString downloadingTorrent() const;
	void setDownloadingTorrent(const QString& value);
	QDateTime pubDate() const;
	void setPubDate(const QDateTime& value);
	quint64 size() const;
	void setSize(quint64 value);
	quint32 peers() const;
	void setPeers(quint32 value);
	quint32 seeds() const;
	void setSeeds(quint32 value);
	bool unread() const;
	void setUread(bool value);
	QString author() const;
	void setAuthor(const QString& value);
	bool isValid();
	RssItem* clone();
};

Q_DECLARE_METATYPE(RssItem*)
#endif

