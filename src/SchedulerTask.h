#pragma once
#include <QThread>
#include <QDateTime>
#include <QVariant>
class SchedulerTask :	public QThread
{
public:
	enum TaskType
	{
		PAUSE_ALL = 1,
		START_ALL = 2,
		LIMIT_UPLOAD = 3,
		LIMIT_DOWNLOAD = 4,
		UNKNOWN = 5
	};

	SchedulerTask(QString name, TaskType type, QVariant limit, QDateTime begin, QObject* parent = 0);
	SchedulerTask(const SchedulerTask& other);
	SchedulerTask();
	void pefromTask();
	TaskType type() const;
	QDateTime startTime() const;
	int limit();
	QString name() const;
	bool operator < (const SchedulerTask& other) const;
	SchedulerTask& operator= (const SchedulerTask& right);
private:
	TaskType iType;
	QDateTime _begin;
	int _limit;
	QString _name;
protected:
	void run();

};

Q_DECLARE_METATYPE(SchedulerTask);
