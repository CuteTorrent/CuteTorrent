#include "avaliable_space_verifier_plugin.h"
#include <TorrentManager.h>
#include <libtorrent/torrent.hpp>
#include <qstorageinfo.h>
#include <libtorrent/alert_manager.hpp>


void avaliable_space_verifier_plugin::verify_avaliable_space()
{
	
	if (m_torrent.valid_metadata())
	{
		int64_t totalSize = m_torrent.torrent_file().files().total_size();
		QString savePath = QString::fromUtf8(m_torrent.save_path().c_str());
		QStorageInfo info(savePath);
		if (totalSize > info.bytesAvailable())
		{
			if (m_torrent.alerts().should_post<not_enough_space_alert>())
			{
				m_torrent.alerts().emplace_alert<not_enough_space_alert, torrent_handle>(m_torrent.get_handle());
				error_code ec(ERROR_DISK_FULL, boost::system::get_system_category());
				m_torrent.set_error(ec, torrent_status::error_file_none);
			}
		}
	}
}


int not_enough_space_alert::type() const
{
	return alert_type;
}

std::string not_enough_space_alert::message() const
{
	return what();
}

int not_enough_space_alert::category() const
{
	return static_category;
}

char const* not_enough_space_alert::what() const
{
	return QT_TRANSLATE_NOOP3("Torrent","There is not enough disk space to save torrent files.");
}

avaliable_space_verifier_plugin::avaliable_space_verifier_plugin(libtorrent::torrent& tor)
: m_torrent(tor)
{
	
}

void avaliable_space_verifier_plugin::on_state(int state)
{
	
	if (state == torrent_status::downloading)
	{
		verify_avaliable_space();
	}
}

