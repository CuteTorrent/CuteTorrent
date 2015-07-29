#include "FileSizeItemDelegate.h"
#include "StaticHelpers.h"


QString FileSizeItemDelegate::displayText(const QVariant& value, const QLocale& locale) const
{
	if (value.type() == QVariant::LongLong || value.type() == QVariant::ULongLong)
	{
		bool ok;
		quint64 size = value.toULongLong(&ok);
		if (ok)
		{
			if (m_hideZeroSize)
			{
				if (size > KbInt)
				{
					return StaticHelpers::toKbMbGb(size);
				}
				
			}
			else
			{
				return StaticHelpers::toKbMbGb(size);
			}
			
		}
	}

	return "";
}
