#pragma once
#include <QStyledItemDelegate>

class ProgressItemDelegate :
	public QStyledItemDelegate
{
public:
	ProgressItemDelegate(QObject* parent) : QStyledItemDelegate(parent) {};

	QString displayText(const QVariant& value, const QLocale& locale) const;


};
