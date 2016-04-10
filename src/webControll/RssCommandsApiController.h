#pragma once
#include "RequestMapper.h"
#include "RssCommon.h"

class RssCommandsApiController : public HttpRequestHandler
{
	Q_DISABLE_COPY(RssCommandsApiController)
public:
	RssCommandsApiController(QObject* parent = NULL);
	void service(HttpRequest& request, HttpResponse& response) override;
private:
	RssManagerPtr m_pRssManager;
};

