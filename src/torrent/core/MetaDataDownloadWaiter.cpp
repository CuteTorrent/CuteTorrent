#include "MetaDataDownloadWaiter.h"
#include "StaticHelpers.h"

MetaDataDownloadWaiter::MetaDataDownloadWaiter(QString metaLink, QObject* parrent/*=NULL*/, bool autoAdd/*=false*/) : QThread(parrent)
{
	MetaLink = metaLink;
	_autoAdd = autoAdd;
	m_pTorrentManager = TorrentManager::getInstance();
}

MetaDataDownloadWaiter::~MetaDataDownloadWaiter()
{}

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
        std::string save_path =
#if LIBTORRENT_VERSION_NUM >= 10000
                h.status(torrent_handle::query_save_path).save_path;
#else
                h.save_path();
#endif
        m_pTorrentManager->AddMagnet(h, QString::fromUtf8(save_path.c_str()));
	}

	//this->deleteLater();
}
