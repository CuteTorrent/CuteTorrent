#include "NotificationSystem.h"
#include "QApplicationSettings.h"
#include <QDebug>
NotificationSystemPtr NotificationSystem::getInstance()
{
	NotificationSystemPtr instance = m_pInstance.lock();
	if (!instance)
	{
		instance.reset(new NotificationSystem());
		m_pInstance = instance;
	}
	return instance;
}
NotificationSystem::NotificationSystem() : QObject(), m_pSettings(QApplicationSettings::getInstance()), m_pTrayIcon(NULL)
{
	m_enabled = m_pSettings->valueBool("Notifications", "use_notification_sys", true);
	m_useCustomFrame = m_pSettings->valueBool("Notifications", "use_custom_frame", true);
	UpdateNotificationMask();
	
}
void NotificationSystem::UpdateNotificationMask()
{
	m_notificationMask = ALL;
	if (m_pSettings->valueBool("Notifications", "report_errors", true))
	{
		if (!m_pSettings->valueBool("Notifications", "report_tracker_errors", true))
		{
			m_notificationMask &= ~TRACKER_ERROR;
		}
		if (!m_pSettings->valueBool("Notifications", "report_disk_errors", true))
		{
			m_notificationMask &= ~DISK_ERROR;
		}
		if (!m_pSettings->valueBool("Notifications", "report_rss_errors", true))
		{
			m_notificationMask &= ~RSS_ERROR;
		}
	}
	else
	{
		m_notificationMask &= ~ERRORS;
	}
}
NotificationSystem::~NotificationSystem()
{
	QApplicationSettings::FreeInstance();
}

void NotificationSystem::OnNewNotification(int notificationType, QString message, QVariant data)
{
	qDebug() << Q_FUNC_INFO << "notificationType=" << notificationType << " message=" << message << " data=" << data << " acceptedMask=" << m_notificationMask;
	if (m_enabled)
	{
		if ((m_notificationMask & notificationType) == notificationType)
		{
			QSystemTrayIcon::MessageIcon icon = gessIcon(notificationType);
			if (m_useCustomFrame)
			{
				QBalloonTip::QBaloonType type = gessBaloonType(notificationType);
				QBalloonTip::showBalloon("CuteTorrent", message, type, data, icon, 10000);
			}
			else
			{
				if (m_pTrayIcon != NULL)
				{
					m_pTrayIcon->showMessage("CuteTorrent", message, icon);
				}
			}
		}
	}
}

QBalloonTip::QBaloonType NotificationSystem::gessBaloonType(int notificationType)
{
	if ((notificationType & TORRENT_INFO) == TORRENT_INFO)
	{
		return QBalloonTip::Info;
	}
	if ((notificationType & TORRENT_COMPLETED) == TORRENT_COMPLETED)
	{
		return QBalloonTip::TorrentCompleted;
	}
	if ((notificationType & UDPATE_INFO) == UDPATE_INFO)
	{
		return QBalloonTip::UpdateNotyfy;
	}
	if ((notificationType & TORRENT_ERROR) == TORRENT_ERROR ||
		(notificationType & RSS_ERROR) == TORRENT_ERROR || 
		(notificationType & TRACKER_ERROR) == TORRENT_ERROR ||
		(notificationType & SYSTEM_ERROR) == SYSTEM_ERROR)
	{
		return QBalloonTip::Error;
	}
}

QSystemTrayIcon::MessageIcon NotificationSystem::gessIcon(int notificationType)
{
	if ((notificationType & TORRENT_INFO) == TORRENT_INFO ||
		(notificationType & TORRENT_COMPLETED) == TORRENT_COMPLETED ||
		(notificationType & UDPATE_INFO) == UDPATE_INFO	)
	{
		return QSystemTrayIcon::Information;
	}
	if ((notificationType & TORRENT_ERROR) == TORRENT_ERROR ||
		(notificationType & RSS_ERROR) == TORRENT_ERROR ||
		(notificationType & TRACKER_ERROR) == TORRENT_ERROR ||
		(notificationType & SYSTEM_ERROR) == SYSTEM_ERROR)
	{
		return QSystemTrayIcon::Critical;
	}
}

void NotificationSystem::setTrayIcon(QSystemTrayIcon* pTrayIcon)
{
	m_pTrayIcon = pTrayIcon;
}

boost::weak_ptr<NotificationSystem> NotificationSystem::m_pInstance;
