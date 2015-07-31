#include "PeerTableModel.h"
#include <QIcon>
#include <IpItemDelegate.h>
#include <QDebug>
#include <QMutexLocker>
#include <QVariant>
PeerTableModel::PeerTableModel(QObject* parent)
	: QStandardItemModel(0, COLUMN_COUNT)
	, m_pUpdateMutex(new QMutex())
	, m_peerItems()
{
	Retranslate();
}


void PeerTableModel::UpdateData(std::vector<libtorrent::peer_info>& peersInfo)
{
	QMutexLocker locker(m_pUpdateMutex);
	QSet<QString> oldPeers = m_peerItems.keys().toSet();

	for (int i = 0; i < peersInfo.size(); i++)
	{
		libtorrent::peer_info peerInfo = peersInfo[i];
		QString peerIp = QString::fromStdString(peerInfo.ip.address().to_string());

		if (m_peerItems.contains(peerIp))
		{
			updatePeer(peerInfo);
			oldPeers.remove(peerIp);
		}
		else
		{
			m_peerItems[peerIp] = addPeer(peerInfo);
		}
	}

	QSetIterator<QString> it(oldPeers);

	while (it.hasNext())
	{
		const QString& peerIp = it.next();
		QStandardItem* pItem = m_peerItems.take(peerIp);
		removeRow(pItem->row());
	}
}

void PeerTableModel::Retranslate()
{
	setHeaderData(IP, Qt::Horizontal, tr("PEER_IP"));
	setHeaderData(CLIENT_NAME, Qt::Horizontal, tr("PEER_CLIENT_NAME"));
	setHeaderData(PEER_PROGRESS, Qt::Horizontal, tr("PEER_%"));
	setHeaderData(DOWNLOAD_RATE, Qt::Horizontal, tr("PEER_DOWNLOAD_SPEED"));
	setHeaderData(UPLOAD_RATE, Qt::Horizontal, tr("PEER_UPLOAD_SPEED"));
	setHeaderData(DOWNLOADED, Qt::Horizontal, tr("PEER_DOWNLOADED"));
	setHeaderData(UPLOADED, Qt::Horizontal, tr("PEER_UPLOADED"));
	setHeaderData(PEER_SPEED, Qt::Horizontal, tr("PEER_SPEED"));
}

void PeerTableModel::Clear()
{
	QMutexLocker locker(m_pUpdateMutex);
	int curentLen = m_peerItems.size();

	if (curentLen != 0)
	{
		removeRows(0, rowCount());
	}

	m_peerItems.clear();
}

void PeerTableModel::updatePeer(libtorrent::peer_info peerInfo)
{
	QString peerIp = QString::fromStdString(peerInfo.ip.address().to_string());
	QStandardItem* pItem = m_peerItems[peerIp];
	int row = pItem->row();
	QString flagFileName = QString(":/flags/%1.png").arg(QString(QByteArray(peerInfo.country, 2)));
	QIcon flagIcon = QIcon(QPixmap(flagFileName));
	setData(index(row, IP), flagIcon, Qt::DecorationRole);
	setData(index(row, IP), QString::fromStdString(peerInfo.ip.address().to_string()), Qt::DisplayRole);
	QString client = QString::fromUtf8(peerInfo.client.c_str());

	if ((peerInfo.flags & libtorrent::peer_info::rc4_encrypted) == libtorrent::peer_info::rc4_encrypted
	        || (peerInfo.flags & libtorrent::peer_info::plaintext_encrypted) == libtorrent::peer_info::plaintext_encrypted)
	{
		client = client.append("*");
	}

	setData(index(row, CLIENT_NAME), client, Qt::DisplayRole);
	setData(index(row, PEER_PROGRESS), peerInfo.progress * 100.f, Qt::DisplayRole);
	setData(index(row, DOWNLOAD_RATE), peerInfo.down_speed, Qt::DisplayRole);
	setData(index(row, UPLOAD_RATE), peerInfo.up_speed, Qt::DisplayRole);
    setData(index(row, DOWNLOADED), QVariant::fromValue(peerInfo.total_download), Qt::DisplayRole);
    setData(index(row, UPLOADED), QVariant::fromValue(peerInfo.total_upload), Qt::DisplayRole);
	setData(index(row, PEER_SPEED), peerInfo.remote_dl_rate, Qt::DisplayRole);
}

QStandardItem* PeerTableModel::addPeer(libtorrent::peer_info peerInfo)
{
	int row = rowCount();
	QString peerIp = QString::fromStdString(peerInfo.ip.address().to_string());
	insertRow(row);
	QString flagFileName = QString(":/flags/%1.png").arg(QString(QByteArray(peerInfo.country, 2)));
	QIcon flagIcon = QIcon(QPixmap(flagFileName));
	setData(index(row, IP), flagIcon, Qt::DecorationRole);
	setData(index(row, IP), QString::fromStdString(peerInfo.ip.address().to_string()), Qt::DisplayRole);
	QString client = QString::fromUtf8(peerInfo.client.c_str());

	if ((peerInfo.flags & libtorrent::peer_info::rc4_encrypted) == libtorrent::peer_info::rc4_encrypted
	        || (peerInfo.flags & libtorrent::peer_info::plaintext_encrypted) == libtorrent::peer_info::plaintext_encrypted)
	{
		client = client.append("*");
	}

	setData(index(row, CLIENT_NAME), client, Qt::DisplayRole);
	setData(index(row, PEER_PROGRESS), peerInfo.progress * 100.f, Qt::DisplayRole);
	setData(index(row, DOWNLOAD_RATE), peerInfo.down_speed, Qt::DisplayRole);
	setData(index(row, UPLOAD_RATE), peerInfo.up_speed, Qt::DisplayRole);
    setData(index(row, DOWNLOADED), QVariant::fromValue(peerInfo.total_download), Qt::DisplayRole);
    setData(index(row, UPLOADED), QVariant::fromValue(peerInfo.total_upload), Qt::DisplayRole);
	setData(index(row, PEER_SPEED), peerInfo.remote_dl_rate, Qt::DisplayRole);
	return item(row, IP);
}
