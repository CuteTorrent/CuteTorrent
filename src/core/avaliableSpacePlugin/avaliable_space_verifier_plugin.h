#include <libtorrent/extensions.hpp>
#include <libtorrent/alert_types.hpp>


struct not_enough_space_alert : libtorrent::torrent_alert
{
public:
	not_enough_space_alert(libtorrent::aux::stack_allocator& alloc, libtorrent::torrent_handle const& h)
		: torrent_alert(alloc, h),
		  handle(h)
	{
	}

	const static int priority = 60;
	const static int alert_type = libtorrent::user_alert_id + 1;
	int type() const override;

	std::string message() const override;

	const static int static_category = error_notification;
	int category() const override;

	char const* what() const override;

	libtorrent::torrent_handle handle;
};

struct avaliable_space_verifier_plugin : libtorrent::plugin
{
	void on_alert(libtorrent::alert const*) override;
	void verify_avaliable_space(libtorrent::torrent* t);

	boost::shared_ptr<libtorrent::torrent_plugin> new_torrent(libtorrent::torrent_handle const&, void*) override;
};

inline boost::shared_ptr<libtorrent::plugin> create_avaliable_space_verifier_plugin()
{
	

	return boost::static_pointer_cast<libtorrent::plugin>(boost::make_shared<avaliable_space_verifier_plugin>());
}

