#ifndef TimeItemDelegate_INCLUDED
#define TimeItemDelegate_INCLUDED
#include <QStyledItemDelegate>

class TimeItemDelegate: public QStyledItemDelegate
{
public:
	explicit TimeItemDelegate(QObject* parent)
		: QStyledItemDelegate(parent)
	{
	}

	QString displayText(const QVariant& value, const QLocale& locale) const override;
};

#endif

