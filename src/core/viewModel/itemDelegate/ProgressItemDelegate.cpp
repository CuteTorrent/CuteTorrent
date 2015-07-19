#include "ProgressItemDelegate.h"
#include <QDebug>


QString ProgressItemDelegate::displayText(const QVariant& value, const QLocale& locale) const
{
	bool ok;
	float fVal = value.toFloat(&ok);

	if(ok)
	{
		return QString::number(fVal, 'f', 2) + " %";
	}

	return "";
}
