#pragma once
#include <QtGui>
#include "SerachResult.h"
#include "StyleEngene.h"
class QSearchItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT
private:
	StyleEngene* m_pStyleEngine;
public:
	QSearchItemDelegate();
	~QSearchItemDelegate();
	static int max_width;
	virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

	virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	QSize margin(const QStyle& style) const;
	QString GetPeersStr(const SearchResult* res) const;
};

