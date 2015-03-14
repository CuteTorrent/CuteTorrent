/**
  @file
  @author Stefan Frings
*/

#include "httplistener.h"
#include "httpconnectionhandler.h"
#include "httpconnectionhandlerpool.h"
#include <QCoreApplication>


HttpListener::HttpListener(QString name, HttpRequestHandler* requestHandler, QObject* parent)
	: QTcpServer(parent)
{
	// Create connection handler pool
	serverName = name;
	this->settings = QApplicationSettings::getInstance();
	pool = new HttpConnectionHandlerPool(name, requestHandler);
}

HttpListener::~HttpListener()
{
	close();
	delete pool;
	QApplicationSettings::FreeInstance();
}


void HttpListener::incomingConnection(int socketDescriptor)
{
#ifdef SUPERVERBOSE
#endif
	HttpConnectionHandler* freeHandler = pool->getConnectionHandler();

	// Let the handler process the new connection.
	if(freeHandler)
	{
		// The descriptor is passed via signal/slot because the handler lives in another
		// thread and cannot open the socket when called by another thread.
		connect(this, SIGNAL(handleConnection(int)), freeHandler, SLOT(handleConnection(int)));
		emit handleConnection(socketDescriptor);
		disconnect(this, SIGNAL(handleConnection(int)), freeHandler, SLOT(handleConnection(int)));
	}
	else
	{
		// Reject the connection
		QTcpSocket* socket = new QTcpSocket(this);
		socket->setSocketDescriptor(socketDescriptor);
		connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
		socket->write("HTTP/1.1 503 too many connections\r\nConnection: close\r\n\r\nToo many connections\r\n");
		socket->disconnectFromHost();
	}
}


void HttpListener::Start()
{
	// Start listening
	int port = settings->value(serverName, "port").toInt();
	listen(QHostAddress::Any, port);

	if(!isListening())
	{
		qCritical("HttpListener: Cannot bind on port %i: %s", port, qPrintable(errorString()));
	}

	/*	else {
			qDebug("HttpListener: %s Listening on port %i",qPrintable(serverName),port);
		}*/
}
