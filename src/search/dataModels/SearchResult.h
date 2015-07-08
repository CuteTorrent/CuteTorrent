#pragma once
#include <QtCore>
#include <helpers/StaticHelpers.h>

class SearchResult : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ Name WRITE setName)
    Q_PROPERTY(QString torrentFileUrl READ TorrentFileUrl WRITE setTorrentFileUrl)
    Q_PROPERTY(QString torrentDescUrl READ TorrentDescUrl WRITE setTorrentDescUrl)
    Q_PROPERTY(QString engine READ Engine WRITE setEngine)
	Q_PROPERTY(QString leechers READ Leechers WRITE setLeechers)
	Q_PROPERTY(QString seeders READ Seeders WRITE setSeeders)
	Q_PROPERTY(QString size READ Size WRITE setSize)
public:
    QString Name() const
    {
        return m_name;
    }

    void setName(QString val)
    {
        m_name = val;
    }

    QString TorrentFileUrl() const
    {
        return m_torrentFileUrl;
    }

    void setTorrentFileUrl(QString val)
    {
        m_torrentFileUrl = val;
    }

    QString TorrentDescUrl() const
    {
        return m_torrentDescUrl;
    }

    void setTorrentDescUrl(QString val)
    {
        m_torrentDescUrl = val;
    }

    QString Engine() const
    {
        return m_engine;
    }

    void setEngine(QString val)
    {
        m_engine = val;
    }

	QString Leechers() const
    {
        return m_leechers;
    }

	void setLeechers(QString val)
    {
        m_leechers = val;
    }

	QString Seeders() const
    {
        return m_seeders;
    }

	void setSeeders(QString val)
    {
        m_seeders = val;
    }

	QString Size() const
    {
        return m_size;
    }

	void setSize(QString val)
    {
		bool ok;
		uint64_t valL = val.toULongLong(&ok);
		if (ok)
		{
			m_size = StaticHelpers::toKbMbGb(valL);
		}
		else
		{
			m_size = val;
		}
        
    }

    bool operator == (SearchResult other)
	{
        if(m_name != other.m_name)
		{
			return false;
		}

        if(m_torrentFileUrl != other.m_torrentFileUrl)
		{
			return false;
		}

        if(m_engine != other.m_engine)
		{
			return false;
		}

        if(m_leechers != other.m_leechers)
		{
			return false;
		}

        if(m_seeders != other.m_seeders)
		{
			return false;
		}

        if(m_size != other.m_size)
		{
			return false;
		}

		return true;
	}
 private:
    QString m_name;
    QString m_torrentFileUrl;
    QString m_torrentDescUrl;
    QString m_engine;
	QString m_leechers;
	QString m_seeders;
	QString m_size;
};


Q_DECLARE_METATYPE(SearchResult*)
Q_DECLARE_METATYPE(QList<SearchResult*>);
