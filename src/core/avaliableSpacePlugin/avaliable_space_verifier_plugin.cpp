#include "avaliable_space_verifier_plugin.h"
#include <TorrentManager.h>
#include <libtorrent/torrent.hpp>
#include <qstorageinfo.h>

void avaliable_space_verifier_plugin::verify_avaliable_space(torrent* t)
{
	if (t->valid_metadata())
	{
		int64_t totalSize = t->torrent_file().files().total_size();
		QString savePath = QString::fromUtf8(t->save_path().c_str());
		QStorageInfo info(savePath);
		if (totalSize > info.bytesAvailable())
		{
			if (t->alerts().should_post<not_enough_space_alert>())
			{
				t->alerts().post_alert(not_enough_space_alert(t->get_handle()));
				error_code ec(ERROR_DISK_FULL, get_system_category());
				t->set_error(ec, "");
			}
		}
		 
	}
}

boost::shared_ptr<torrent_plugin> avaliable_space_verifier_plugin::new_torrent(torrent* t, void* userData)
{
	verify_avaliable_space(t);

	return plugin::new_torrent(t, userData);
}

void avaliable_space_verifier_plugin::on_alert(alert const* a)
{
	if (a->type() == metadata_received_alert::alert_type)
	{
		const metadata_received_alert* p = alert_cast<metadata_received_alert>(a);
		const boost::weak_ptr<torrent> torrent = p->handle.native_handle();
		verify_avaliable_space(torrent.lock().get());
	}
}
