#include "RssDownloadManager.h"

RssDownloadManager::RssDownloadManager()
{
	m_pTorrentManager = TorrentManager::getInstance();
}

RssDownloadManager::~RssDownloadManager()
{
	TorrentManager::freeInstance();
}

void RssDownloadManager::onNewRssItem(feed_item item)
{

}

void RssDownloadManager::StartDownload()
{

}
