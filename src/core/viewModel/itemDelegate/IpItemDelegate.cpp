#include "IpItemDelegate.h"

QString IpItemDelegate::displayText(const QVariant& value, const QLocale& locale) const
{
	if (value.canConvert<boost::asio::ip::address>())
	{
		boost::asio::ip::address adr = value.value<boost::asio::ip::address>();
		return QString::fromStdString(adr.to_string());
	}
	return "";
}