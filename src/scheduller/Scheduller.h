#pragma once
#include <QObject>
#include <QTimerEvent>
#include "SchedulerTask.h"
#include "QApplicationSettings.h"
class Scheduller : public QObject
{
	Q_OBJECT
protected:
	Scheduller();
	~Scheduller();
	static Scheduller* _instance;
	static int _instanceCount;
	void timerEvent(QTimerEvent* event);
public:
	static Scheduller* getInstance();
	static void freeInstance();
private:
	int cuurentTimerID;
	QList<SchedulerTask> tasks;
	QApplicationSettings* settings;
	void checkTasks();
public slots:
	void UpdateTasks();
};

