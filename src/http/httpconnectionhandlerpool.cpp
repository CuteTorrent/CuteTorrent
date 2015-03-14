#include "httpconnectionhandlerpool.h"


HttpConnectionHandlerPool::HttpConnectionHandlerPool(QString name, HttpRequestHandler* _requestHandler)
	: QObject()
{
	serverName = name;
	this->settings = QApplicationSettings::getInstance();
	this->requestHandler = _requestHandler;
	cleanupTimer.start(settings->value(serverName, "cleanupInterval", 1000).toInt());
	connect(&cleanupTimer, SIGNAL(timeout()), SLOT(cleanup()));
}


HttpConnectionHandlerPool::~HttpConnectionHandlerPool()
{
	// delete all connection handlers and wait until their threads are closed
	foreach(HttpConnectionHandler* handler, pool)
	{
		delete handler;
	}

	QApplicationSettings::FreeInstance();
}


HttpConnectionHandler* HttpConnectionHandlerPool::getConnectionHandler()
{
	HttpConnectionHandler* freeHandler = 0;
	mutex.lock();

	// find a free handler in pool
	foreach(HttpConnectionHandler* handler, pool)
	{
		if(!handler->isBusy())
		{
			freeHandler = handler;
			freeHandler->setBusy();
			break;
		}
	}

	// create a new handler, if necessary
	if(!freeHandler)
	{
		int maxConnectionHandlers = settings->value(serverName, "maxThreads", 100).toInt();

		if(pool.count() < maxConnectionHandlers)
		{
			freeHandler = new HttpConnectionHandler(requestHandler);
			freeHandler->setBusy();
			pool.append(freeHandler);
		}
	}

	mutex.unlock();
	return freeHandler;
}



void HttpConnectionHandlerPool::cleanup()
{
	int maxIdleHandlers = settings->value(serverName, "minThreads", 1).toInt();
	int idleCounter = 0;
	mutex.lock();

	foreach(HttpConnectionHandler* handler, pool)
	{
		if(!handler->isBusy())
		{
			if(++idleCounter > maxIdleHandlers)
			{
				pool.removeOne(handler);
				delete handler;
				break; // remove only one handler in each interval
			}
		}
	}

	mutex.unlock();
}
