#ifndef _RSS_ITEM_H_
#define _RSS_ITEM_H_

#include <libtorrent/config.hpp>
#include <libtorrent/rss.hpp>
#include <QtCore>
#include "RssFeedItem.h"
class TorrentManager;
class RssFeed
{
public:
	RssFeed()
	{}
	RssFeed(libtorrent::feed_handle rssFeed);
	void Update();
	RssFeedItemList GetFeedItems();
	libtorrent::feed_settings GetSettings();
	void SetSettinfs(libtorrent::feed_settings);
private:
	void BuildFeedItemsList();
	libtorrent::feed_handle m_hRssFeed;
	RssFeedItemList m_pFeedItems;
	friend TorrentManager;
};
typedef QList<RssFeed*> RssFeedList;
Q_DECLARE_METATYPE(RssFeed*)
#endif // !_RSS_ITEM_H_
