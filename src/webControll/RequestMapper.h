#pragma once
#include "http/httprequesthandler.h"
class RequestMapper: public HttpRequestHandler
{
	Q_OBJECT
	Q_DISABLE_COPY(RequestMapper)
public:

	RequestMapper(QObject* parent = 0);
	~RequestMapper();
	void service(HttpRequest& request, HttpResponse& response) override;
private:
	QMap<QString, HttpRequestHandler*> m_pRequestMap;

};
