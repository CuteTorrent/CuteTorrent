#pragma once
#include "http/httprequesthandler.h"
#include "TorrentCommon.h"

class UploadController : public HttpRequestHandler
{
	Q_OBJECT
	Q_DISABLE_COPY(UploadController)
private:
	TorrentManagerPtr m_pTorrentManager;
public:
	UploadController(QObject*);
	void service(HttpRequest& request, HttpResponse& response) override;
	~UploadController();
};

