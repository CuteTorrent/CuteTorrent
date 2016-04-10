#pragma once
#include <QtCore>
#include <QtGui>
#include "ServiceCommon.h"
#include "RssCommon.h"
class RssFeedTreeItem;
class RssFeedItemTreeItem;
class RssItem;
class RssFeed;

class QRssItemDelegate :
	public QStyledItemDelegate
{
	Q_OBJECT
public:
	QRssItemDelegate(QObject* parent = 0);
	~QRssItemDelegate();
	QSize margin(const QStyle& style) const;
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
	FaviconDownloaderPtr m_pFaviconDownloader;
	void drawFeed(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index, RssFeed* pRssFeed) const;
	void drawFeedItem(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index, RssItem* pFeedItem) const;
	QSize feedSizeHint(const QStyleOptionViewItem& option, RssFeed* pRssFeed) const;
	QSize feedItemSizeHint(const QStyleOptionViewItemV4& option, RssItem* pFeedItem) const;
};

