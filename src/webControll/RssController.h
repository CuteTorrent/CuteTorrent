#pragma once
#include "http/httprequesthandler.h"
#include "RssCommon.h"

class RssController : public HttpRequestHandler
{
private:
	RssManagerPtr m_pRssManager;
public:
	RssController(QObject* parent);

	~RssController();
	void service(HttpRequest& request, HttpResponse& response) override;
public:
};

