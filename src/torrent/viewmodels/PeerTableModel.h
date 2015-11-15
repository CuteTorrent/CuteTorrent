#ifndef PeerTableModel_INCLUDED
#define PeerTableModel_INCLUDED
#include <libtorrent/peer_info.hpp>
#include <QStringList>
#include <QMutex>
#include <QStandardItem>
#include "StyleEngene.h"
class PeerTableModel : public QStandardItemModel
{
	Q_OBJECT
public:
	enum Columns
	{
		IP,
		CLIENT_NAME,
		PEER_PROGRESS,
		DOWNLOAD_RATE,
		UPLOAD_RATE,
		DOWNLOADED,
		UPLOADED,
		PEER_SPEED,
		COLUMN_COUNT
	};
	PeerTableModel(QObject* parent = NULL);
	void UpdateData(std::vector<libtorrent::peer_info>& peersInfo);
	void Retranslate();
	void Clear();
private:
	StyleEngene* m_pStyleEngene;
	QMutex* m_pUpdateMutex;
	QHash<QString, QStandardItem*> m_peerItems;
	void updatePeer(libtorrent::peer_info peerInfo);
	QStandardItem* addPeer(libtorrent::peer_info peerInfo);
};

Q_DECLARE_METATYPE(libtorrent::peer_info)
#endif