#ifndef _SERVICE_COMMON_H_
#define _SERVICE_COMMON_H_
#include <boost/smart_ptr.hpp>


class FaviconDownloader;
typedef boost::shared_ptr<FaviconDownloader>  FaviconDownloaderPtr;

class TorrentDownloader;
typedef boost::shared_ptr<TorrentDownloader>  TorrentDownloaderPtr;

class NotificationSystem;
typedef boost::shared_ptr<NotificationSystem>  NotificationSystemPtr;
#endif