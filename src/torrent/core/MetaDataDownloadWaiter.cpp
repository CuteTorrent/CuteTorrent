#include "MetaDataDownloadWaiter.h"
#include "StaticHelpers.h"
#include "TorrentManager.h"

MetaDataDownloadWaiter::MetaDataDownloadWaiter(QString metaLink, TerminationToken* terminationToken, QObject* parrent/*=NULL*/)
	: QThread(parrent)
{
	MetaLink = metaLink;
	m_pTerminationToken = terminationToken;
	m_pTorrentManager = TorrentManager::getInstance();
}

MetaDataDownloadWaiter::~MetaDataDownloadWaiter()
{
}

void MetaDataDownloadWaiter::run()
{
	error_code ec;
	torrent_handle h = m_pTorrentManager->ProcessMagnetLink(MetaLink, m_pTerminationToken, ec);

	if (ec)
	{
		m_pTorrentManager->CancelMagnetLink(MetaLink);
		emit ErrorOccured(StaticHelpers::translateLibTorrentError(ec));
		return;
	}

	if (m_pTerminationToken->IsTerminationRequested)
		return;

	boost::scoped_ptr<openmagnet_info> ti(m_pTorrentManager->GetTorrentInfo(h));

	if (ti != NULL)
	{
		ti->link = MetaLink;
		emit DownloadCompleted(*ti);
	}
}

