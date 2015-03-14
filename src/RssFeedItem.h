#pragma once
#include <QtCore>
#include <libtorrent/rss.hpp>
using namespace libtorrent;
class RssFeedItem{
private:
	feed_item m_feedItem;
public:
	RssFeedItem(feed_item feed);
	QString Title();
	QString Description();
	QString URL();
	QString Coment();
	quint64 Size();
	QString Category();
	QString InfoHash();
};

typedef QList<RssFeedItem> RssFeedItemList;