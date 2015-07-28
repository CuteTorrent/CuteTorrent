#ifndef SpeedItemDelegate_INCLUDED
#define SpeedItemDelegate_INCLUDED
#include <QStyledItemDelegate>

class SpeedItemDelegate : public QStyledItemDelegate
{
public:
	SpeedItemDelegate(QObject* parent) : QStyledItemDelegate(parent) {};

	QString displayText(const QVariant& value, const QLocale& locale) const override;
};

#endif