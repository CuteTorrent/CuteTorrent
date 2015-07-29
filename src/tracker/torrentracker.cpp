#include "torrentracker.h"
#include "http/httplistener.h"
#include "trackerrequestmapper.h"
#include "QApplicationSettings.h"
TorrentTracker::TorrentTracker(QObject* parent) :
	QObject(parent)
{
	requestHandler = new TrackerRequestHandler(this);
	httpServer = new HttpListener("TorrentTracker", requestHandler);
}

void TorrentTracker::OnSettngsChnaged(QString group, QString key)
{
	if (group == "TorrentTracker" && key=="enabled")
	{
		if (m_pSettings->valueBool("TorrentTracker", "enabled", false) && isRunning())
		{
			start();
		}
		else
		{
			if (isRunning())
			{
				stop();
			}
		}
	}
	
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
