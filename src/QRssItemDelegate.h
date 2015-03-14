#pragma once
#include <QtCore>
#include <QtGui>
class RssFeedTreeItem;
class RssFeedItemTreeItem;
class FaviconDownloader;
class QRssItemDelegate :
	public QStyledItemDelegate
{
	Q_OBJECT
public:
	QRssItemDelegate();
	~QRssItemDelegate();
	QSize margin(const QStyle& style) const;
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	
private:
	FaviconDownloader* m_pFaviconDownloader;
	void drawFeed(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex &index, RssFeedTreeItem* pFeed) const;
	void drawFeedItem(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex &index, RssFeedItemTreeItem* pFeed) const;
	QSize feedSizeHint(const QStyleOptionViewItem & option, RssFeedTreeItem* pFeed) const;
	QSize feedItemSizeHint(const QStyleOptionViewItemV4 & option, RssFeedItemTreeItem* pFeedItem) const;
};

