#ifndef IpItemDelegate_INCLUDED
#define IpItemDelegate_INCLUDED
#include <QStyledItemDelegate>
#include <boost/asio/ip/address.hpp>
class IpItemDelegate : public QStyledItemDelegate
{
public:
	IpItemDelegate(QObject* parent) : QStyledItemDelegate(parent) {};

	QString displayText(const QVariant& value, const QLocale& locale) const override;
};
Q_DECLARE_METATYPE(boost::asio::ip::address)
#endif