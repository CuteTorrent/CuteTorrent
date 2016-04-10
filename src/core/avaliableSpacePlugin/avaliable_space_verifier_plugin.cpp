#include "avaliable_space_verifier_plugin.h"
#include <TorrentManager.h>
#include <libtorrent/torrent.hpp>
#include <qstorageinfo.h>
#include <libtorrent/alert_manager.hpp>


void avaliable_space_verifier_plugin::verify_avaliable_space(torrent* t)
{
	if (t != NULL && t->valid_metadata())
	{
		int64_t totalSize = t->torrent_file().files().total_size();
		QString savePath = QString::fromUtf8(t->save_path().c_str());
		QStorageInfo info(savePath);
		if (totalSize > info.bytesAvailable())
		{
			if (t->alerts().should_post<not_enough_space_alert>())
			{
				t->alerts().emplace_alert<not_enough_space_alert, torrent_handle>(t->get_handle());
				libtorrent::error_code ec(ERROR_DISK_FULL, boost::system::get_system_category());
				t->set_error(ec, torrent_status::error_file_none);
			}
		}
	}
}

std::auto_ptr<alert> not_enough_space_alert::clone_impl() const
{
	return std::auto_ptr<alert>(new not_enough_space_alert(*this));
}

boost::shared_ptr<torrent_plugin> avaliable_space_verifier_plugin::new_torrent(const torrent_handle& t, void* userData)
{
	verify_avaliable_space(t.native_handle().get());

	return plugin::new_torrent(t, userData);
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
	return "There is not enough disk space to save torrent files.";
}

void avaliable_space_verifier_plugin::on_alert(alert const* a)
{
	if (a->type() == metadata_received_alert::alert_type)
	{
		const metadata_received_alert* p = alert_cast<metadata_received_alert>(a);
		const boost::shared_ptr<torrent> torrent = p->handle.native_handle();
		verify_avaliable_space(torrent.get());
	}
}

