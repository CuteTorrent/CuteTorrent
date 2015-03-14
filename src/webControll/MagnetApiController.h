#pragma once
#include "http/httprequesthandler.h"
#include "TorrentManager.h"
#include "defs.h"
class MagnetApiController :
	public HttpRequestHandler
{
	Q_OBJECT
private:
	QMap<QString, QString> savePathMap;
	TorrentManager* m_pTorrentManager;
public:
	MagnetApiController(QObject*);

	void service(HttpRequest& request, HttpResponse& response);
	~MagnetApiController(void);
public slots:
	void DownloadMetadataCompleted(const openmagnet_info& info);
};
