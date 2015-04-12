#pragma once
#include "http/httprequesthandler.h"
class TorrentManager;
class CommandsApiController: public HttpRequestHandler
{
	Q_OBJECT
	Q_DISABLE_COPY(CommandsApiController)
private:
	TorrentManager* m_pTorrentManager;
public:

	CommandsApiController(QObject* parent = 0);

	void service(HttpRequest& request, HttpResponse& response);
	~CommandsApiController();
};

