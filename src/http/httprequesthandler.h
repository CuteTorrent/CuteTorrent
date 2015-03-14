/**
  @file
  @author Stefan Frings
*/

#ifndef HTTPREQUESTHANDLER_H
#define HTTPREQUESTHANDLER_H

#include "httprequest.h"
#include "httpresponse.h"

/**
   The request handler generates a response for each HTTP request. Web Applications
   usually have one central request handler that maps incoming requests to several
   controllers (servlets) based on the requested path.
   <p>
   You need to override the service() method or you will always get an HTTP error 501.
   <p>
   @warning Be aware that the main request handler instance must be created on the heap and
   that it is used by multiple threads simultaneously.
   @see StaticFileController which delivers static local files.
*/

class HttpRequestHandler : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(HttpRequestHandler)

public:
	struct Creditionals
	{
		QString username, password;
	};
	bool requireAuth;
	Creditionals account;
	/** Constructor */
	HttpRequestHandler(QString name, QObject* parent = 0);

	/** Destructor */
	virtual ~HttpRequestHandler();

	/**
	  Generate a response for an incoming HTTP request.
	  @param request The received HTTP request
	  @param response Must be used to return the response
	  @warning This method must be thread safe
	*/
	virtual void service(HttpRequest& request, HttpResponse& response);

	bool CheckCreditinals(HttpRequest& request, HttpResponse& response);
private:
	QMap<quint32, QString> nonceContainer;
	QString serverName;
	void initSettings();
	QString _generateNonce(const int len);
	QString _getAuthentificateHeader();
	QString _getOpaque(QString realm, QString nonce);
	QString realm, authMethod , nonce;
};

#endif // HTTPREQUESTHANDLER_H

