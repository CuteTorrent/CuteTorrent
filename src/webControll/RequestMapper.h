#pragma once
#include "http/httprequesthandler.h"
#include "http/staticfilecontroller.h"
#include "TorrentApiController.h"
#include "UploadController.h"
#include "CommandsApiController.h"
#include "SettingsAPiController.h"
#include "MagnetApiController.h"
class RequestMapper: public HttpRequestHandler
{
	Q_OBJECT
	Q_DISABLE_COPY(RequestMapper)
public:

	RequestMapper(QObject* parent = 0);
	~RequestMapper();
	void service(HttpRequest& request, HttpResponse& response);
private:
	MagnetApiController* magnetController;
	StaticFileController* staticFileController;
	TorrentApiController* torrentController;
	UploadController* uploadController;
	CommandsApiController* commandsController;
	SettingsAPiController* settingsController;
};
