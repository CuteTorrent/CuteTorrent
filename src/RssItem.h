#ifndef _RSS_ITEM_INCLUDED_
#define _RSS_ITEM_INCLUDED_
#include <QString>
#include <QDateTime>
#include <QMetaType>
class RssItem : public QObject
{
	friend QDataStream& operator <<(QDataStream& out, const RssItem& any);
	friend QDataStream& operator >>(QDataStream& out, RssItem& any);
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
	quint64 m_size;
	quint32 m_peers;
	quint32 m_seeds;
	bool m_unread;
public:

	RssItem(const RssItem& other);;
	RssItem() : QObject(), m_size(0), m_peers(0), m_seeds(0), m_unread(true) {}
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


};

Q_DECLARE_METATYPE(RssItem*)
#endif
