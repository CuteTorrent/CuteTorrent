#pragma once
#include "http/httprequesthandler.h"
#include <QObject>
class TorrentManager;
class TorrentCommandsApiController: public HttpRequestHandler
{
	Q_OBJECT
	Q_DISABLE_COPY(TorrentCommandsApiController)
private:
	TorrentManager* m_pTorrentManager;
public:

	TorrentCommandsApiController(QObject* parent = 0);

	void service(HttpRequest& request, HttpResponse& response) override;
	~TorrentCommandsApiController();
};

