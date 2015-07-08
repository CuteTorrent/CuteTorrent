#include "QSearchItemDelegate.h"
#include "QSearchDisplayModel.h"
#include "SearchResult.h"
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
	SearchEnginePtr instance = SearchEngine::getInstance();
	QList<ISerachProvider*> iSerachProviders = instance->GetSearchProviders();
	ISerachProvider* pFoundProvider = NULL;
	QString name = res->Engine();
	for (int i = 0; i < iSerachProviders.length(); i++)
	{
		if (iSerachProviders[i]->Name() == name)
		{
			pFoundProvider = iSerachProviders[i];
			break;
		}
	}
	QIcon mimeIcon;
	if (pFoundProvider != NULL)
	{
		mimeIcon = pFoundProvider->getIcon();
	}
	QString nameStr(res->Name());
	QSize nameSize(nameFM.size(0, nameStr));
	QFont statusFont(option.font);
	statusFont.setPointSize(int (option.font.pointSize() * 0.9));
	const QFontMetrics statusFM(statusFont);
	const QString statusStr(res->Size());
	QFont progressFont(statusFont);
	const QFontMetrics progressFM(progressFont);
	const QString progressStr(GetPeersStr(res));
	painter->save();
	painter->setRenderHint(QPainter::Antialiasing);
	style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, opt.widget);
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

	if (nameArea.x() + nameArea.width() > opt.rect.width())
	{
		nameArea.setWidth(opt.rect.width() - nameArea.x());
	}

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
	const QString nameStr(res->Name());
	int nameWidth = nameFM.width(nameStr);

	if (nameWidth + iconSize > option.rect.width())
	{
		nameWidth = option.rect.width() - iconSize;
	}

	QFont statusFont(option.font);
	statusFont.setPointSize(int (option.font.pointSize() * 0.9));
	const QFontMetrics statusFM(statusFont);
	const QString statusStr(GetPeersStr(res));
	int statusWidth = statusFM.width(statusStr);

	if (statusWidth > option.rect.width())
	{
		statusWidth = option.rect.width();
	}

	QFont progressFont(statusFont);
	const QFontMetrics progressFM(progressFont);
	const QString progressStr(res->Size());
	const int progressWidth = progressFM.width(progressStr);
	const QSize m(margin(*style));
	return QSize(m.width() + iconSize + MAX3(nameWidth, statusWidth, progressWidth),
	             m.height() * 3 + nameFM.lineSpacing() + progressFM.lineSpacing());
}

QString QSearchItemDelegate::GetPeersStr(const SearchResult* res) const
{
	if(res->Leechers() >= 0 && res->Seeders() >= 0)
	{
		return tr("Leechs: %1 Seeds: %2").arg(res->Leechers(), res->Seeders());
	}
	else
	{
		return tr("Leechs: N/A Seeds: N/A");
	}
}
