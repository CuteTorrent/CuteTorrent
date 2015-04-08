#include "QSearchItemDelegate.h"
#include "QSearchDisplayModel.h"
#include "QTorrentItemDelegat.h"
#include "StaticHelpers.h"


enum
{
	GUI_PAD = 6,
	BAR_HEIGHT = 12
};

namespace
{
int MAX3(int a, int b, int c)
{
	const int ab(a > b ? a : b);
	return ab > c ? ab : c;
}
}
int QSearchItemDelegate::max_width = 0;
QSearchItemDelegate::QSearchItemDelegate(QObject* parent) : QStyledItemDelegate(parent), m_pStyleEngine(StyleEngene::getInstance())
{
}


QSearchItemDelegate::~QSearchItemDelegate()
{
}

QSize QSearchItemDelegate::margin(const QStyle& style) const
{
	Q_UNUSED(style);
	return QSize(4, 4);
}

void QSearchItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	SearchResult* res = index.data(QSearchDisplayModel::SearchItemRole).value<SearchResult*>();
	QStyleOptionViewItemV4 opt = option;
	initStyleOption(&opt, index);
	QStyle* style;

	if(opt.widget != NULL)
	{
		style = opt.widget->style();
	}
	else
	{
		style = QApplication::style();
	}

	const int iconSize(style->pixelMetric(QStyle::PM_ToolBarIconSize));
	QFont nameFont(option.font);
	nameFont.setWeight(QFont::Bold);
	const QFontMetrics nameFM(nameFont);
	const QIcon mimeIcon = m_pStyleEngine->getIcon(res->Engine);
	QString nameStr(res->Name);
	QSize nameSize(nameFM.size(0, nameStr));

	if(nameSize.width() > max_width)
	{
		nameSize.setWidth(max_width);
	}

	QFont statusFont(option.font);
	statusFont.setPointSize(int (option.font.pointSize() * 0.9));
	const QFontMetrics statusFM(statusFont);
	const QString statusStr(StaticHelpers::toKbMbGb(res->size));
	QFont progressFont(statusFont);
	const QFontMetrics progressFM(progressFont);
	const QString progressStr(GetPeersStr(res));
	painter->save();
	painter->setRenderHint(QPainter::Antialiasing);
	style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, opt.widget);
	/* if (option.state & QStyle::State_Selected) {
	QPalette::ColorGroup cg = option.state & QStyle::State_Enabled
	? QPalette::Active : QPalette::Disabled;
	if (cg == QPalette::Normal && !(option.state & QStyle::State_Active))
	cg = QPalette::Inactive;


	painter->fillRect(option.rect, opt.backgroundBrush);
	}*/
	//style->drawControl(QStyle::CE_ItemViewItem,&option,painter);
	QIcon::Mode im = QIcon::Normal;
	QIcon::State qs = QIcon::On;
	QPalette::ColorGroup cg = QPalette::Normal;
	QPalette::ColorRole cr;

	if(option.state & QStyle::State_Selected)
	{
		cr = QPalette::HighlightedText;
	}

	cr = QPalette::Text;
	// layout
	const QSize m(margin(*style));
	QRect fillArea(option.rect);
	fillArea.adjust(m.width(), m.height(), -m.width(), -m.height());
	QRect iconArea(fillArea.x(), fillArea.y() + (fillArea.height() - iconSize) / 2, iconSize, iconSize);
	QRect nameArea(iconArea.x() + iconArea.width() + GUI_PAD, fillArea.y(),
	               fillArea.width() - GUI_PAD - iconArea.width(), nameSize.height());
	nameArea.setWidth(max_width);
	QRect progArea(nameArea);
	progArea.moveTop(nameArea.y() + statusFM.lineSpacing() + GUI_PAD / 2);
	progArea.setWidth(nameArea.width() / 2);
	QRect statusArea(nameArea);
	statusArea.moveTop(nameArea.y() + statusFM.lineSpacing() + GUI_PAD / 2);
	statusArea.moveLeft(nameArea.width() / 2 + iconArea.width() + GUI_PAD);
	statusArea.setWidth(nameArea.width() / 2);
	statusArea.setHeight(nameSize.height());
	painter->setPen(opt.palette.color(cg, cr));
	mimeIcon.paint(painter, iconArea, Qt::AlignCenter, im, qs);
	painter->setFont(nameFont);
	nameStr = nameFM.elidedText(nameStr, Qt::ElideRight, nameArea.width());
	style->drawItemText(painter, nameArea, Qt::AlignLeft, opt.palette, option.state & QStyle::State_Enabled, nameStr);
	painter->setFont(statusFont);
	style->drawItemText(painter, statusArea, Qt::AlignRight, opt.palette, option.state & QStyle::State_Enabled, statusStr);
	painter->setFont(progressFont);
	style->drawItemText(painter, progArea, Qt::AlignLeft, opt.palette, option.state & QStyle::State_Enabled, progressStr);
	painter->restore();
}

QSize QSearchItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	SearchResult* res = index.data(QSearchDisplayModel::SearchItemRole).value<SearchResult*>();
	const QStyle* style(QApplication::style());
	const int iconSize(style->pixelMetric(QStyle::PM_ToolBarIconSize));
	QFont nameFont(option.font);
	nameFont.setWeight(QFont::Bold);
	const QFontMetrics nameFM(nameFont);
	const QString nameStr(res->Name);
	int nameWidth = nameFM.width(nameStr);

	if(nameWidth > max_width)
	{
		nameWidth = max_width;
	}

	QFont statusFont(option.font);
	statusFont.setPointSize(int (option.font.pointSize() * 0.9));
	const QFontMetrics statusFM(statusFont);
	const QString statusStr(GetPeersStr(res));
	int statusWidth = statusFM.width(statusStr);

	if(statusWidth > max_width)
	{
		statusWidth = max_width;
	}

	QFont progressFont(statusFont);
	const QFontMetrics progressFM(progressFont);
	const QString progressStr(StaticHelpers::toKbMbGb(res->size));
	const int progressWidth = progressFM.width(progressStr);
	const QSize m(margin(*style));
	return QSize(m.width() + iconSize + MAX3(nameWidth, statusWidth, progressWidth),
	             m.height() * 3 + nameFM.lineSpacing() + progressFM.lineSpacing());
}

QString QSearchItemDelegate::GetPeersStr(const SearchResult* res) const
{
	if(res->leechers >= 0 && res->seeders >= 0)
	{
		return tr("Leechs: %1 Seeds: %2").arg(QString::number(res->leechers), QString::number(res->seeders));
	}
	else
	{
		return tr("Leechs: N/A Seeds: N/A");
	}
}
