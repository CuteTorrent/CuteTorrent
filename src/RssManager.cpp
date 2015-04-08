#include "RssManager.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkDiskCache>
#include <QDesktopServices>
#include <QDebug>
#include <QTimer>
#include "QApplicationSettings.h"
#include "RssFeed.h"
#include "StaticHelpers.h"
#include "messagebox.h"
RssManager::RssManager(QObject* parent) : QObject(parent)
{
	
	m_pSettings = QApplicationSettings::getInstance();
	QTimer::singleShot(1000, this, SLOT(LoadFeeds()));
}

RssFeed* RssManager::addFeed(QUrl url, bool& isNew)
{
	isNew = true;
	foreach (RssFeed* pFeed, m_pFeeds)
	{
		if (pFeed->url() == url)
		{
			isNew = false;
			return pFeed;
		}
	}
	RssFeed* pFeed = new RssFeed(url, QUuid::createUuid());
	m_pFeeds.append(pFeed);
	connect(pFeed, SIGNAL(FeedChanged(QUuid)), SIGNAL(FeedChanged(QUuid)));
	return pFeed;
}

RssManager::~RssManager()
{
	qDebug() << "RssManager::~RssManager";
	QApplicationSettings::FreeInstance();
}

QList<RssFeed*> RssManager::feeds()
{
	return m_pFeeds;
}

RssManagerPtr RssManager::getInstance()
{
	RssManagerPtr instance = m_sInstrance.lock();
	if (!instance)
	{
		instance.reset(new RssManager());
		m_sInstrance = instance;
	}
	return instance;
}

void RssManager::SaveFeeds()
{
	QString dataDir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
	QString feedsPath = StaticHelpers::CombinePathes(dataDir, "feeds.dat");
	QFile feedsDat(feedsPath);
	if (feedsDat.open(QIODevice::WriteOnly))
	{
		QDataStream dataStream(&feedsDat);
		dataStream.setVersion(QDataStream::Qt_4_8);
		dataStream << m_pFeeds.size();
		for each (RssFeed* pFeed in m_pFeeds)
		{
			dataStream << *pFeed;
		}
		feedsDat.flush();
		feedsDat.close();
	}
	
}

void RssManager::LoadFeeds()
{
	QString dataDir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
	QString feedsPath = StaticHelpers::CombinePathes(dataDir, "feeds.dat");
	QFile feedsDat(feedsPath);
	if (feedsDat.open(QIODevice::ReadOnly))
	{
		QDataStream dataStream(&feedsDat);
		dataStream.setVersion(QDataStream::Qt_4_8);
		int numFeeds;
		dataStream >> numFeeds;
		QUuid tempUid = QUuid::createUuid();
		for (int i = 0; i < numFeeds; i++)
		{
			RssFeed* tempFeed = new RssFeed(QUrl(""), tempUid);
			dataStream >> *tempFeed;
			m_pFeeds.append(tempFeed);
			tempFeed->Update();
			connect(tempFeed, SIGNAL(FeedChanged(QUuid)), SIGNAL(FeedChanged(QUuid)));
		}
		feedsDat.close();
	}
}

void RssManager::removeFeed(QUuid uid)
{
	int index = 0;
	foreach(RssFeed* pFeed, m_pFeeds)
	{
		if (pFeed->uid() == uid)
		{
			break;
		}
		index++;
	}
	RssFeed* pFeed2Remove = m_pFeeds.at(index);
	m_pFeeds.removeAt(index);
	disconnect(pFeed2Remove, SIGNAL(FeedChanged(QUuid)), this, SIGNAL(FeedChanged(QUuid)));
	pFeed2Remove->deleteLater();
	pFeed2Remove = NULL;
	emit FeedChanged(uid);
}

boost::weak_ptr<RssManager> RssManager::m_sInstrance;

