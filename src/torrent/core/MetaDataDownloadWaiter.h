#include <QThread>
#include "TorrentCommon.h"
#include "defs.h"

struct TerminationToken
{
	bool IsTerminationRequested;
};

class MetaDataDownloadWaiter : public QThread
{
	Q_OBJECT
	signals:
	void DownloadCompleted(openmagnet_info ti);
	void ErrorOccured(error_code ec);
public:
	MetaDataDownloadWaiter(QString metaLink, TerminationToken* terminationToken, QObject* parrent = NULL);
	~MetaDataDownloadWaiter();
private:
	QString MetaLink;
	TerminationToken* m_pTerminationToken;
	TorrentManagerPtr m_pTorrentManager;
protected:
	void run() override;
};

