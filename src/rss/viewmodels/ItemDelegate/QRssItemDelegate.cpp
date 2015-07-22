#include "QRssItemDelegate.h"
#include "RssFeed.h"
#include "RssFeedTreeItem.h"
#include "RssFeedItemTreeItem.h"
#include "StaticHelpers.h"
#include "FaviconDownloader.h"
#include "QRssDisplayModel.h"
#include "RssItem.h"
enum
{
	GUI_PAD = 6
};

namespace
{
int MAX3(int a, int b, int c)
{
	const int ab(a > b ? a : b);
	return ab > c ? ab : c;
}
}

QRssItemDelegate::QRssItemDelegate(QObject* parent) : QStyledItemDelegate(parent)
{
	m_pFaviconDownloader = FaviconDownloader::getInstance();
}


QRssItemDelegate::~QRssItemDelegate()
{
}

QSize QRssItemDelegate::margin(const QStyle& style) const
{
	Q_UNUSED(style);
	return QSize(4, 4);
}

void QRssItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	/*try
	{*/
		QVariant data = index.data(QRssDisplayModel::RssFeedRole);

		if (data.isValid())
		{
			return drawFeed(painter, option, index, data.value<RssFeed*>());
		}
		else
		{
			data = index.data(QRssDisplayModel::RssItemRole);

			if (data.isValid())
			{
				return drawFeedItem(painter, option, index, data.value<RssItem*>());
			}
		}
/*	}
	catch (...)
	{
	}*/
}

QSize QRssItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
/*	try
	{*/
		QVariant data = index.data(QRssDisplayModel::RssFeedRole);

		if (data.isValid())
		{
			return feedSizeHint(option, data.value<RssFeed*>());
		}
		else
		{
			data = index.data(QRssDisplayModel::RssItemRole);

			if (data.isValid())
			{
				return feedItemSizeHint(option, data.value<RssItem*>());
			}
		}
	/*}
	catch (...)
	{
	}*/

	return QSize();
}

void QRssItemDelegate::drawFeed(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index, RssFeed* pRssFeed) const
{
	QStyleOptionViewItemV4 opt = option;
	initStyleOption(&opt, index);
	QStyle* style;

	if (opt.widget != NULL)
	{
		style = opt.widget->style();
	}
	else
	{
		style = QApplication::style();
	}

	const int iconSize(style->pixelMetric(QStyle::PM_SmallIconSize));
	QFont nameFont(option.font);
	nameFont.setWeight(QFont::Bold);
	const QFontMetrics nameFM(nameFont);
	const QIcon favicon = m_pFaviconDownloader->getFavicon(pRssFeed->url().toString());
	QString nameStr = pRssFeed->displayName();
	QSize nameSize(nameFM.size(0, nameStr));
	QFont statusFont(option.font);
	statusFont.setPointSize(int(option.font.pointSize() * 0.9));
	const QFontMetrics statusFM(statusFont);
	QString statusStr;
	QString errorStr = pRssFeed->error();

	if (errorStr.isEmpty())
	{
		if (pRssFeed->isUpdating())
		{
			statusStr = tr("STATUS_UPDATING");
		}
		else
		{
			statusStr = tr("STATUS_OK");
		}
	}
	else
	{
		statusStr = errorStr;
	}

	QFont updateTimeFont(statusFont);
	const QFontMetrics updateTimeFM(updateTimeFont);
	const QString updateTimeStr(StaticHelpers::toTimeString(pRssFeed->nextUpdate()));
	painter->save();
	painter->setRenderHint(QPainter::Antialiasing);
	style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, opt.widget);
	QIcon::Mode im = QIcon::Normal;
	QIcon::State qs = QIcon::On;
	QPalette::ColorGroup cg = QPalette::Normal;
	QPalette::ColorRole cr;
	cr = QPalette::Text;
	// layout
	const QSize m(margin(*style));
	QRect fillArea(option.rect);
	fillArea.adjust(m.width(), m.height(), -m.width(), -m.height());
	QRect iconArea(fillArea.x(), fillArea.y() + (fillArea.height() - iconSize) / 2, iconSize, iconSize);
	QRect nameArea(iconArea.x() + iconArea.width() + GUI_PAD, fillArea.y(),
	               fillArea.width() - GUI_PAD - iconArea.width(), nameSize.height());

	if (nameArea.x() + nameArea.width() > opt.rect.width())
	{
		nameArea.setWidth(opt.rect.width() - nameArea.x());
	}

	QRect updateTimeArea(nameArea);
	updateTimeArea.moveTop(nameArea.y() + statusFM.lineSpacing() + GUI_PAD / 2);
	updateTimeArea.setWidth(nameArea.width() / 2);
	QRect statusArea(nameArea);
	statusArea.moveTop(nameArea.y() + statusFM.lineSpacing() + GUI_PAD / 2);
	/*statusArea.moveLeft(nameArea.width() / 2 + iconArea.width() + GUI_PAD);
	statusArea.setWidth(nameArea.width() / 2);
	statusArea.setHeight(nameSize.height());
	*/
	painter->setPen(opt.palette.color(cg, cr));
	favicon.paint(painter, iconArea, Qt::AlignCenter, im, qs);
	painter->setFont(nameFont);
	nameStr = nameFM.elidedText(nameStr, Qt::ElideRight, nameArea.width());
	style->drawItemText(painter, nameArea, Qt::AlignLeft, opt.palette, option.state & QStyle::State_Enabled, nameStr);
	painter->setFont(statusFont);
	style->drawItemText(painter, statusArea, Qt::AlignRight, opt.palette, option.state & QStyle::State_Enabled, statusStr);
	painter->setFont(updateTimeFont);
	style->drawItemText(painter, updateTimeArea, Qt::AlignLeft, opt.palette, option.state & QStyle::State_Enabled, updateTimeStr);
	painter->restore();
}

void QRssItemDelegate::drawFeedItem(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index, RssItem* pFeedItem) const
{
	QStyleOptionViewItemV4 opt = option;
	initStyleOption(&opt, index);
	QStyle* style;

	if (opt.widget != NULL)
	{
		style = opt.widget->style();
	}
	else
	{
		style = QApplication::style();
	}

	QFont nameFont(option.font);

	if (pFeedItem->unread())
	{
		nameFont.setWeight(QFont::Bold);
	}

	const QFontMetrics nameFM(nameFont);
	QString nameStr(pFeedItem->title());
	QSize nameSize(nameFM.size(0, nameStr));
	QFont sizeFont(option.font);

	if (pFeedItem->unread())
	{
		sizeFont.setWeight(QFont::Bold);
	}

	const QFontMetrics sizeFM(sizeFont);
	qint64 size = pFeedItem->size();
	QString sizeStr;

	if (size > 0)
	{
		sizeStr = StaticHelpers::toKbMbGb(size);
	}
	else
	{
		sizeStr = tr("- Mb");
	}

	QSize sizeSize(sizeFM.size(0, sizeStr));
	QFont categoryFont(option.font);

	if (pFeedItem->unread())
	{
		categoryFont.setWeight(QFont::Bold);
	}

	const QFontMetrics categoryFM(categoryFont);
	QString categoryStr(pFeedItem->category());

	if (categoryStr.isEmpty())
	{
		categoryStr = "-";
	}

	QSize categorySize(categoryFM.size(0, categoryStr));
	const QSize m(margin(*style));
	QRect fillArea(option.rect);
	fillArea.adjust(m.width(), m.height(), -m.width(), -m.height());
	QRect nameArea(fillArea.x(), fillArea.y(), fillArea.width(), nameSize.height());
	QRect sizeArea(fillArea.x(), fillArea.y() + sizeFM.lineSpacing() + GUI_PAD / 2, fillArea.width() / 2, nameSize.height());
	QRect categoryArea(nameArea);
	categoryArea.moveTop(nameArea.y() + categoryFM.lineSpacing() + GUI_PAD / 2);
	QPalette::ColorGroup cg = QPalette::Normal;
	QPalette::ColorRole cr = QPalette::Text;
	painter->save();
	painter->setRenderHint(QPainter::Antialiasing);
	style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, opt.widget);
	painter->setPen(opt.palette.color(cg, cr));
	painter->setFont(nameFont);
	nameStr = nameFM.elidedText(nameStr, Qt::ElideRight, nameArea.width());
	style->drawItemText(painter, nameArea, Qt::AlignLeft, opt.palette, option.state & QStyle::State_Enabled, nameStr);
	painter->setFont(sizeFont);
	style->drawItemText(painter, sizeArea, Qt::AlignLeft, opt.palette, option.state & QStyle::State_Enabled, sizeStr);
	painter->setFont(categoryFont);
	style->drawItemText(painter, categoryArea, Qt::AlignRight, opt.palette, option.state & QStyle::State_Enabled, categoryStr);
	painter->restore();
}

QSize QRssItemDelegate::feedSizeHint(const QStyleOptionViewItem& option, RssFeed* pRssFeed) const
{
	const QStyle* style(QApplication::style());
	const int iconSize(style->pixelMetric(QStyle::PM_ToolBarIconSize));
	QFont nameFont(option.font);
	nameFont.setWeight(QFont::Bold);
	const QFontMetrics nameFM(nameFont);
	const QString nameStr(pRssFeed->title());
	int nameWidth = nameFM.width(nameStr);
	QFont statusFont(option.font);
	statusFont.setPointSize(int(option.font.pointSize() * 0.9));
	const QFontMetrics statusFM(statusFont);
	QString statusStr;
	QString errorStr = pRssFeed->error();

	if (errorStr.isEmpty())
	{
		if (pRssFeed->isUpdating())
		{
			statusStr = tr("STATUS_UPDATING");
		}
		else
		{
			statusStr = tr("STATUS_OK");
		}
	}
	else
	{
		statusStr = errorStr;
	}

	int statusWidth = statusFM.width(statusStr);
	QFont updateTimeFont(statusFont);
	const QFontMetrics updateTimeFM(updateTimeFont);
	const QString updateTimeStr(StaticHelpers::toTimeString(pRssFeed->nextUpdate()));
	const int updateTimeWidth = updateTimeFM.width(updateTimeStr);
	const QSize m(margin(*style));
	return QSize(m.width() + iconSize + MAX3(nameWidth, statusWidth, updateTimeWidth),
	             m.height() * 2 + nameFM.lineSpacing() + updateTimeFM.lineSpacing());
}

QSize QRssItemDelegate::feedItemSizeHint(const QStyleOptionViewItemV4& opt, RssItem* pFeedItem) const
{
	QStyle* style;

	if (opt.widget != NULL)
	{
		style = opt.widget->style();
	}
	else
	{
		style = QApplication::style();
	}

	QFont nameFont(opt.font);
	nameFont.setWeight(QFont::Bold);
	const QFontMetrics nameFM(nameFont);
	QString nameStr(pFeedItem->title());
	QSize nameSize(nameFM.size(0, nameStr));
	QFont sizeFont(opt.font);
	sizeFont.setWeight(QFont::Bold);
	const QFontMetrics sizeFM(sizeFont);
	QString sizeStr(StaticHelpers::toKbMbGb(pFeedItem->size()));
	QSize sizeSize(sizeFM.size(0, sizeStr));
	QFont categoryFont(opt.font);
	categoryFont.setWeight(QFont::Bold);
	const QFontMetrics categoryFM(categoryFont);
	QString categoryStr(pFeedItem->category());
	QSize categorySize(categoryFM.size(0, categoryStr));
	const QSize m(margin(*style));
	QRect fillArea(opt.rect);
	fillArea.adjust(m.width(), m.height(), -m.width(), -m.height());
	QRect nameArea(fillArea.x(), fillArea.y(), fillArea.width(), nameSize.height());
	QRect sizeArea(nameArea);
	sizeArea.setWidth(nameArea.width() / 2);
	sizeArea.moveRight(nameArea.width() / 2);
	QRect categoryArea(nameArea);
	categoryArea.moveTop(nameArea.y() + categoryFM.lineSpacing() + GUI_PAD / 2);
	return QSize(m.width() + nameSize.width(), m.height() * 2 + nameSize.height() + categorySize.height());
}


