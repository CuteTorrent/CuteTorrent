
#pragma once
#include <QtGui>
enum TorrentFilterType
{
	ACTIVE,
	NOT_ACTIVE,
	SEEDING,
	DOWNLOADING,
	COMPLETED,
	EMPTY
};

class QTorrentFilterProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT
public:
	QTorrentFilterProxyModel(QObject* parent = 0);
	void setGroupFilter(QString groupName);
	void setTorrentFilter(TorrentFilterType activityFilter);
private slots:
	void Update();
private:
	enum InternalFilterType
	{
		GROUP,
		TORRENT
	};
	QTimer* m_pUpdateTimer;
	QMutex* m_pUpdateLocker;
	InternalFilterType m_currentFilterType;
	QString m_groupFilter;
	TorrentFilterType m_torrentFilter;
protected:
	bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;


};