#pragma once
#include <QtWidgets>
#include "StyleEngene.h"
class SearchResult;

class QSearchItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT
private:
	StyleEngene* m_pStyleEngine;
public:
	QSearchItemDelegate(QObject* parent = 0);
	~QSearchItemDelegate();
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	QSize margin(const QStyle& style) const;
	QString GetPeersStr(const SearchResult* res) const;
};

