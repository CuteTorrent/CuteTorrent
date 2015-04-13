#include <boost/shared_ptr.hpp>


class RssManager;
typedef boost::shared_ptr<RssManager> RssManagerPtr;

class RssFeed;

class RssParser;
typedef boost::shared_ptr<RssParser> RssParserPtr;

class RssDownloader;
typedef boost::shared_ptr<RssDownloader> RssDownloaderPtr;
