/**
  @file
  @author Stefan Frings
*/

#ifndef LISTENER_H
#define LISTENER_H

#include <QTcpServer>
#include <QSettings>
#include <QBasicTimer>
#include "httpconnectionhandler.h"
#include "httpconnectionhandlerpool.h"
#include "httprequesthandler.h"
#include "QApplicationSettings.h"
/**
  Listens for incoming TCP connections and and passes all incoming HTTP requests to your implementation of HttpRequestHandler,
  which processes the request and generates the response (usually a HTML document).
  <p>
  Example for the required settings in the config file:
  <code><pre>
  port=8080
  minThreads=1
  maxThreads=10
  cleanupInterval=1000
  readTimeout=60000
  maxRequestSize=16000
  maxMultiPartSize=1000000
  </pre></code>
  The port number is the incoming TCP port that this listener listens to.
  @see HttpConnectionHandlerPool for description of config settings minThreads, maxThreads and cleanupInterval
  @see HttpConnectionHandler for description of config settings readTimeout
  @see HttpRequest for description of config settings maxRequestSize and maxMultiPartSize
*/

class HttpListener : public QTcpServer
{
	Q_OBJECT
	Q_DISABLE_COPY(HttpListener)
public:

	/**
	  Constructor.
	  @param settings Configuration settings for the HTTP server. Must not be 0.
	  @param requestHandler Processes each received HTTP request, usually by dispatching to controller classes.
	  @param parent Parent object.
	*/

	HttpListener(QString name, HttpRequestHandler* requestHandler, QObject* parent = 0);
	void Start();

	/** Destructor */
	virtual ~HttpListener();

protected:

	/** Serves new incoming connection requests */
	void incomingConnection(int socketDescriptor);

private:

	QString serverName;
	/** Configuration settings for the HTTP server */
	QApplicationSettings* settings;

	/** Pool of connection handlers */
	HttpConnectionHandlerPool* pool;

signals:

	/**
	  Emitted when the connection handler shall process a new incoming onnection.
	  @param socketDescriptor references the accepted connection.
	*/
	void handleConnection(int socketDescriptor);

};

#endif // LISTENER_H
