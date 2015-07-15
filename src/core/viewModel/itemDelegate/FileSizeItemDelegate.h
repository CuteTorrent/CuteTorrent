#pragma once
#include <QStyledItemDelegate>

class FileSizeItemDelegate :
	public QStyledItemDelegate
{
public:
	FileSizeItemDelegate(QObject* parent) :
		QStyledItemDelegate(parent)	{};


	QString displayText(const QVariant& value, const QLocale& locale) const override;

};
