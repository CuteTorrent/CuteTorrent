#include "TimeItemDelegate.h"
#include <helpers/StaticHelpers.h>

QString TimeItemDelegate::displayText(const QVariant& value, const QLocale& locale) const
{
	bool ok;
	float fVal = value.toFloat(&ok);

	if (ok && fVal == std::numeric_limits<float>::infinity())
	{
		return QString(QChar(8734));
	}
	return StaticHelpers::toTimeString(value.toInt());
}

