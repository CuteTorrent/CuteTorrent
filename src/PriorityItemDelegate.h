#pragma once
#include <QStyledItemDelegate>
#include <QVariant>
class PriorityItemDelegate :
	public QStyledItemDelegate
{
	Q_OBJECT
public:
	PriorityItemDelegate(QObject* parent) : QStyledItemDelegate(parent) {};

	QString displayText(const QVariant& value, const QLocale& locale) const;


};
