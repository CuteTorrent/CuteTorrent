#include <QVariant>
#include <boost/shared_ptr.hpp>
typedef QVariantHash RssItem;

class RssManager;
typedef boost::shared_ptr<RssManager> RssManagerPtr;

class RssFeed;

class RssParser;
typedef boost::shared_ptr<RssParser> RssParserPtr;
