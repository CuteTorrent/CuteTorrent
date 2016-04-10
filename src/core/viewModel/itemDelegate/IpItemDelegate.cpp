#include "IpItemDelegate.h"

QString IpItemDelegate::displayText(const QVariant& value, const QLocale& locale) const
{
	if (value.type() == QVariant::UserType)
	{
		boost::asio::ip::address adr = value.value<boost::asio::ip::address>();
		return QString::fromStdString(adr.to_string());
	}

	return "";
}

