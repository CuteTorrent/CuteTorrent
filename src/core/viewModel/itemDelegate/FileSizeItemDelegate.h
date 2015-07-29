#pragma once
#include <QStyledItemDelegate>

class FileSizeItemDelegate :
	public QStyledItemDelegate
{
	bool m_hideZeroSize;
public:
	FileSizeItemDelegate(bool hideZeroSize = false, QObject* parent = NULL)
		: QStyledItemDelegate(parent)
		, m_hideZeroSize(hideZeroSize)
	{};


	QString displayText(const QVariant& value, const QLocale& locale) const override;

};
