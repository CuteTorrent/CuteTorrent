#pragma once
#include "http/httprequesthandler.h"
#include "TorrentManager.h"
class UploadController : public HttpRequestHandler
{
	Q_OBJECT
	Q_DISABLE_COPY(UploadController)
private:
	TorrentManager* m_pTorrentManager;
public:
	UploadController(QObject*);
	void service(HttpRequest& request, HttpResponse& response);
	~UploadController();
};
