
#include <QtCore>
#include "TorrentManager.h"

class RssDownloadManager: public QObject
{
	Q_OBJECT
public:
	RssDownloadManager();
	~RssDownloadManager();
public slots:
	void onNewRssItem(feed_item);
private:
	TorrentManager* m_pTorrentManager;
	void StartDownload();
};

Q_DECLARE_METATYPE(feed_item)