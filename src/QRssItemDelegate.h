#pragma once
#include <QStyledItemDelegate>
class QRssItemDelegate :
	public QStyledItemDelegate
{
	Q_OBJECT
public:
	QRssItemDelegate();
	~QRssItemDelegate();

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};

