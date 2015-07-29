#include "IpItemDelegate.h"

QString IpItemDelegate::displayText(const QVariant& value, const QLocale& locale) const
{
    if (value.canConvert<QString>())
	{
        boost::asio::ip::address adr = boost::asio::ip::address::from_string(value.toString().toStdString());
		return QString::fromStdString(adr.to_string());
	}
	return "";
}
