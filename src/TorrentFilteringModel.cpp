#include "TorrentFilteringModel.h"



TorrentFilteringModel::TorrentFilteringModel(QObject* parent) : QSortFilterProxyModel(parent)
{
}

TorrentFilteringModel::~TorrentFilteringModel(void)
{
}

bool TorrentFilteringModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
	qDebug() << "TorrentFilteringModel::filterAcceptsRow" << source_row << source_parent;
	return true;
}

bool TorrentFilteringModel::filterAcceptsColumn(int source_column, const QModelIndex& source_parent) const
{
	qDebug() << "TorrentFilteringModel::filterAcceptsColumn" << source_row << source_parent;
	return true;
}


