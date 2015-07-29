#pragma once
#include "TorrentCommon.h"
#include "http/httprequesthandler.h"
#include "TorrentStorrage.h"
class TorrentApiController: public HttpRequestHandler
{
	Q_OBJECT
	Q_DISABLE_COPY(TorrentApiController)
private:
	TorrentManagerPtr m_pTorrentManager;
	TorrentStorragePtr m_pTorrentStorrage;
public:

	TorrentApiController(QObject* parent = 0);
	~TorrentApiController();
	void service(HttpRequest& request, HttpResponse& response) override;

};
