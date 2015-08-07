#include "SpeedItemDelegate.h"
#include <helpers/StaticHelpers.h>

QString SpeedItemDelegate::displayText(const QVariant& value, const QLocale& locale) const
{
	if (value.type() == QVariant::Int)
	{
		bool ok;
		int speedValue = value.toInt(&ok);

		if (ok && speedValue > KbInt)
		{
			return StaticHelpers::toKbMbGb(speedValue, true);
		}
	}

	return "";
}