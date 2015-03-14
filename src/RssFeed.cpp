
#include "RssFeed.h"
#include "TorrentManager.h"
#include "StaticHelpers.h"
RssFeed::RssFeed(libtorrent::feed_handle rssFeed)
{
	m_hRssFeed = rssFeed;
	BuildFeedItemsList();
}

void RssFeed::Update()
{
	m_hRssFeed.update_feed();
}

libtorrent::feed_settings RssFeed::GetSettings()
{
	return m_hRssFeed.settings();
}

void RssFeed::SetSettinfs(libtorrent::feed_settings settings)
{
	m_hRssFeed.set_settings(settings);
}

RssFeedItemList RssFeed::GetFeedItems()
{
	return m_pFeedItems;
}

void RssFeed::BuildFeedItemsList()
{
	std::vector<feed_item> feedItems = m_hRssFeed.get_feed_status().items;
	for each (feed_item feedItem in feedItems)
	{
		RssFeedItem item(feedItem);
		m_pFeedItems.append(item);
	}
}

QString RssFeed::title()
{
	return QString::fromUtf8(m_hRssFeed.get_feed_status().title.c_str());
}

int RssFeed::next_update()
{
	return m_hRssFeed.get_feed_status().next_update;
}

bool RssFeed::isUpdating()
{
	return m_hRssFeed.get_feed_status().updating;
}

QString RssFeed::error()
{
	error_code ec = m_hRssFeed.get_feed_status().error;
	if (ec)
	{
		return StaticHelpers::translateLibTorrentError(ec);
	}
	return "";
}

QString RssFeed::url()
{
	return QString::fromStdString(m_hRssFeed.get_feed_status().url);
}

