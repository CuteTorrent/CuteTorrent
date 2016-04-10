#include "PeerSortModel.h"
#include "PeerTableModel.h"
#include "IpItemDelegate.h"

bool PeerSortModel::lessThan(const QModelIndex& left, const QModelIndex& right) const
{
	if (left.column() == PeerTableModel::IP)
	{
		QVariant rightData = right.data();
		QVariant leftData = left.data();

		if (rightData.isValid() && leftData.isValid())
		{
			boost::asio::ip::address leftAddress = leftData.value<boost::asio::ip::address>();
			boost::asio::ip::address rightAddress = rightData.value<boost::asio::ip::address>();
			return leftAddress < rightAddress;
		}
	}

	return QSortFilterProxyModel::lessThan(left, right);
}

PeerSortModel::PeerSortModel(QObject* parent): QSortFilterProxyModel(parent)
{
	setDynamicSortFilter(true);
}

