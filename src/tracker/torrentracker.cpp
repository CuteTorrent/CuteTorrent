#include "torrentracker.h"

TorrentTracker* TorrentTracker::instance = NULL;
int TorrentTracker::instanceCount = 0;

TorrentTracker::TorrentTracker(QObject* parent) :
	QObject(parent)
{
	requestHandler = new TrackerRequestHandler(this);
	httpServer = new HttpListener("TorrentTracker", requestHandler);
}

TorrentTracker::~TorrentTracker()
{
	stop();
	delete httpServer;
	delete requestHandler;
}

TorrentTracker* TorrentTracker::getInstance()
{
	if(instance == NULL)
	{
		instance = new TorrentTracker();
	}

	instanceCount++;
	return instance;
}

void TorrentTracker::freeInstance()
{
	instanceCount--;

	if(instanceCount == 0)
	{
		delete instance;
		instance = NULL;
	}
}

bool TorrentTracker::isRunning()
{
	return httpServer->isListening();
}

void TorrentTracker::start()
{
	if(httpServer != NULL)
	{
		stop();
	}

	httpServer->Start();
}

void TorrentTracker::stop()
{
	if(httpServer != NULL)
	{
		if(httpServer->isListening())
		{
			httpServer->close();
		}
	}
}
