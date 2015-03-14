#include "Scheduller.h"
#include "QBaloon.h"
#include <QDebug>
Scheduller::Scheduller()
{
	settings = QApplicationSettings::getInstance();
	tasks = settings->GetSchedullerQueue();
	checkTasks();

	if(!tasks.isEmpty())
	{
		QDateTime now = QDateTime::currentDateTime();
		int toNextTask = tasks.first().startTime().toTime_t() - now.toTime_t();
		cuurentTimerID = startTimer(toNextTask * 1000);
	}
}

Scheduller* Scheduller::_instance = NULL;
int Scheduller::_instanceCount = 0;
Scheduller* Scheduller::getInstance()
{
	if(_instance == NULL)
	{
		_instance = new Scheduller();
	}

	_instanceCount++;
	return _instance;
}
void Scheduller::freeInstance()
{
	_instanceCount--;

	if(!_instanceCount)
	{
		_instance->~Scheduller();
		_instance = NULL;
	}
}

Scheduller::~Scheduller()
{
	QApplicationSettings::FreeInstance();
}

void Scheduller::UpdateTasks()
{
	tasks = settings->GetSchedullerQueue();
	checkTasks();
	killTimer(cuurentTimerID);
	QDateTime now = QDateTime::currentDateTime();
	uint toNextTask = tasks.first().startTime().toTime_t() - now.toTime_t();
	cuurentTimerID = startTimer(toNextTask * 1000);
}

void Scheduller::checkTasks()
{
	QDateTime now = QDateTime::currentDateTime();

	for(QList<SchedulerTask>::iterator i = tasks.begin(); i != tasks.end(); ++i)
	{
		if(i->startTime() < now)
		{
			tasks.erase(i);
		}
	}

	settings->SaveSchedullerQueue(tasks);
}

void Scheduller::timerEvent(QTimerEvent* event)
{
	if(event->timerId() == cuurentTimerID)
	{
		QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information;
		SchedulerTask first = tasks.first();
		QBalloonTip::showBalloon("CuteTorrent", tr("CT_PEFORMING_TASK %1").arg(first.name()), QBalloonTip::Info, qVariantFromValue(0), icon,
		                         5 * 1000);
		first.pefromTask();
		tasks.removeFirst();
		killTimer(cuurentTimerID);

		if(!tasks.isEmpty())
		{
			QDateTime now = QDateTime::currentDateTime();
			uint toNextTask = first.startTime().toTime_t() - now.toTime_t();
			cuurentTimerID = startTimer(toNextTask * 1000);
		}

		settings->SaveSchedullerQueue(tasks);
	}
}

