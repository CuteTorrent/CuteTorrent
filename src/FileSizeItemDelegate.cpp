#include "FileSizeItemDelegate.h"
#include "StaticHelpers.h"


QString FileSizeItemDelegate::displayText(const QVariant& value, const QLocale& locale) const
{
	if(value.type() == QVariant::DateTime)
	{
		return StaticHelpers::toKbMbGb(value.toULongLong());
	}

	return "";
}
