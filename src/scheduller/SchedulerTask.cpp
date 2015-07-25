#include "SchedulerTask.h"
#include "TorrentManager.h"
#include <QDebug>
SchedulerTask::SchedulerTask(QString name, TaskType type, QVariant limit, QDateTime begin, QObject* parent /*= 0*/) : QThread(parent)
{
	m_ruleName = name;
	m_taskType = type;
	bool ok;

	if(type == LIMIT_DOWNLOAD || type == LIMIT_UPLOAD)
	{
		m_speedLimit = limit.toInt(&ok);

		if(!ok)
		{
		}
	}

	m_beginDate = begin;
}

SchedulerTask::SchedulerTask(const SchedulerTask& other)
{
	m_taskType = other.m_taskType;
	m_ruleName  = other.m_ruleName;
	m_speedLimit = other.m_speedLimit;
	m_beginDate = other.m_beginDate;
}
SchedulerTask::SchedulerTask()
{
	m_taskType = UNKNOWN;
}

void SchedulerTask::run()
{
}

SchedulerTask::TaskType SchedulerTask::type() const
{
	return m_taskType;
}

void SchedulerTask::pefromTask()
{
	TorrentManagerPtr tManager = TorrentManager::getInstance();
	libtorrent::session_settings current = tManager->readSettings();

	switch(m_taskType)
	{
		case START_ALL		:
			tManager->StartAllTorrents();
			break;

		case PAUSE_ALL		:
			tManager->PauseAllTorrents();
			break;

		case LIMIT_UPLOAD	:
		{
			current.upload_rate_limit = m_speedLimit;
			tManager->updateSettings(current);
		}
		break;

		case LIMIT_DOWNLOAD	:
		{
			current.download_rate_limit = m_speedLimit;
			tManager->updateSettings(current);
		}
		break;

		default:
			break;
	}
}

bool SchedulerTask::operator< (const SchedulerTask& other) const
{
	return m_beginDate < other.startTime();
}

QDateTime SchedulerTask::startTime() const
{
	return m_beginDate;
}


int SchedulerTask::limit()
{
	return m_speedLimit;
}

SchedulerTask& SchedulerTask::operator= (const SchedulerTask& other)
{
	if(this == &other)
	{
		return *this;
	}

	m_taskType = other.m_taskType;
	m_ruleName  = other.m_ruleName;
	m_speedLimit = other.m_speedLimit;
	m_beginDate = other.m_beginDate;
	return *this;
}

bool SchedulerTask::operator==(const SchedulerTask& other)
{
	return m_taskType == other.m_taskType
		&& m_beginDate == other.m_beginDate
		&& m_speedLimit == other.m_speedLimit
		&& m_ruleName == other.m_ruleName;
}

bool SchedulerTask::operator!=(const SchedulerTask& other)
{
	return !operator==(other);
}

QString SchedulerTask::name() const
{
	return m_ruleName;
}
