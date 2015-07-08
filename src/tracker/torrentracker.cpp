#include "torrentracker.h"

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
