#include "MetaDataDownloadWaiter.h"
#include "StaticHelpers.h"
#include "TorrentManager.h"

MetaDataDownloadWaiter::MetaDataDownloadWaiter(QString metaLink, QObject* parrent/*=NULL*/)
	: QThread(parrent)
{
	MetaLink = metaLink;
	m_pTorrentManager = TorrentManager::getInstance();
}

MetaDataDownloadWaiter::~MetaDataDownloadWaiter()
{
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

	boost::scoped_ptr<openmagnet_info> ti(m_pTorrentManager->GetTorrentInfo(h));

	if (ti != NULL)
	{
		ti->link = MetaLink;
		emit DownloadCompleted(*ti);
	}
}

