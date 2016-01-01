

#include <libtorrent/extensions.hpp>
#include <libtorrent/alert_types.hpp>

struct not_enough_space_alert : libtorrent::torrent_alert
{
	not_enough_space_alert(libtorrent::torrent_handle const& h) : torrent_alert(h)
	{
		handle = h;
	}
	const static int alert_type = libtorrent::user_alert_id + 1;
	virtual int type() const { return alert_type; }

	virtual std::string message() const
	{
		return what();
	}

	virtual std::auto_ptr<alert> clone() const
	{
		return std::auto_ptr<alert>(new not_enough_space_alert(*this));
	}

	const static int static_category = alert::error_notification;
	virtual int category() const { return static_category; }

	virtual char const* what() const { return "There is not enough disk space to save torrent files."; }

	libtorrent::torrent_handle handle;
};

struct avaliable_space_verifier_plugin : libtorrent::plugin
{
	static void verify_avaliable_space(libtorrent::torrent* t);
	boost::shared_ptr<libtorrent::torrent_plugin> new_torrent(libtorrent::torrent*, void*) override;
	void on_alert(libtorrent::alert const*) override;
	
};

inline boost::shared_ptr<libtorrent::plugin> create_avaliable_space_verifier_plugin()
{
	boost::shared_ptr<libtorrent::plugin> plugin_ptr(new avaliable_space_verifier_plugin());

	return plugin_ptr;
}
