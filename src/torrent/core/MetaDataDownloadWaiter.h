#include <QThread>
#include "TorrentManager.h"
class MetaDataDownloadWaiter : public QThread
{
	Q_OBJECT
signals:
	void DownloadCompleted(openmagnet_info ti);
	void ErrorOccured(QString error);
public:
	MetaDataDownloadWaiter(QString metaLink, QObject* parrent = NULL);
	~MetaDataDownloadWaiter();
private:
	QString MetaLink;
	TorrentManagerPtr m_pTorrentManager;
protected:
	void run() override;
};


