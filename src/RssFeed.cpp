
#include "RssFeed.h"
#include "TorrentManager.h"
RssFeed::RssFeed(libtorrent::feed_handle rssFeed)
{
	m_hRssFeed = rssFeed;
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

