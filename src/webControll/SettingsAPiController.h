#pragma once
#include "http/httprequesthandler.h"
#include "QApplicationSettings.h"

class SettingsAPiController :
	public HttpRequestHandler
{
	Q_OBJECT
	Q_DISABLE_COPY(SettingsAPiController)
private:
	QApplicationSettingsPtr settings;
public:
	SettingsAPiController(QObject*);
	void service(HttpRequest& request, HttpResponse& response) override;
	~SettingsAPiController(void);
};

