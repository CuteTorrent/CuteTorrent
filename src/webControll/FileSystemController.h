#pragma once
#include "http/httprequesthandler.h"
class FileSystemController : public HttpRequestHandler
{

public:
	FileSystemController(QObject* parent)
		: HttpRequestHandler("WebControl", parent)
	{
	}

	~FileSystemController();
	void service(HttpRequest& request, HttpResponse& response) override;
public:
};
