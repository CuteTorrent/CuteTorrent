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
	Q_PROPERTY(int leechers READ Leechers WRITE setLeechers)
	Q_PROPERTY(int seeders READ Seeders WRITE setSeeders)
	Q_PROPERTY(quint64 size READ Size WRITE setSize)
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

	int Leechers() const
	{
		return m_leechers;
	}

	void setLeechers(int val)
	{
		m_leechers = val;
	}

	int Seeders() const
	{
		return m_seeders;
	}

	void setSeeders(int val)
	{
		m_seeders = val;
	}

	quint64 Size() const
	{
		return m_size;
	}

	void setSize(quint64 val)
	{
		m_size = val;
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
	int m_leechers;
	int m_seeders;
	quint64 m_size;
};


Q_DECLARE_METATYPE(SearchResult*)
Q_DECLARE_METATYPE(QList<SearchResult*>);
