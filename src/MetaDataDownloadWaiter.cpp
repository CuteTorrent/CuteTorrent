#include "MetaDataDownloadWaiter.h"

MetaDataDownloadWaiter::MetaDataDownloadWaiter(QString metaLink, QObject* parrent/*=NULL*/, bool autoAdd/*=false*/) : QThread(parrent)
{
	MetaLink = metaLink;
	_autoAdd = autoAdd;
	m_pTorrentManager = TorrentManager::getInstance();
}

MetaDataDownloadWaiter::~MetaDataDownloadWaiter()
{
	TorrentManager::freeInstance();
}

void MetaDataDownloadWaiter::run()
{
	error_code ec;
	torrent_handle h = m_pTorrentManager->ProcessMagnetLink(MetaLink, ec);

	if (ec)
	{
		m_pTorrentManager->CancelMagnetLink(MetaLink);
		emit ErrorOccured(StaticHelpers::translateLibTorrentError(ec));
		return;
	}

	if(!_autoAdd)
	{
		boost::scoped_ptr<openmagnet_info> ti(m_pTorrentManager->GetTorrentInfo(h));

		if (ti != NULL)
		{
			ti->link = MetaLink;
			emit DownloadCompleted(*ti);
		}
	}
	else
	{
		m_pTorrentManager->AddMagnet(h, QString::fromStdString(h.status(torrent_handle::query_save_path).save_path), "", QMap<QString, qint8>());
	}

	//this->deleteLater();
}
