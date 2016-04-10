#include "InitializationWorker.h"
#include "TorrentManager.h"

void InitializationWorker::PeformInit()
{
	TorrentManager::getInstance()->InitSession(boost::bind(&InitializationWorker::progress, this, _1, _2));
	emit finished();
}

