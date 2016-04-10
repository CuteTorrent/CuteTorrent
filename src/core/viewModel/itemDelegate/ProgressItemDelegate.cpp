#include "ProgressItemDelegate.h"
#include <QDebug>


QString ProgressItemDelegate::displayText(const QVariant& value, const QLocale& locale) const
{
	bool ok;
	float fVal = value.toFloat(&ok);

	if (ok && fVal > 0.0f)
	{
		return QString::number(fVal, 'f', 2) + " %";
	}

	return "";
}

