#include "NotificationSystem.h"
#include "QApplicationSettings.h"
#include <QEvent>


NotificationSystem::NotificationSystem() : QObject(), m_pSettings(QApplicationSettings::getInstance()), m_isShwoingNotification(false), m_defaultMessageDuration(5000)
{
	UpdateNotificationSettings();
}

void NotificationSystem::UpdateNotificationSettings()
{
	m_notificationMask = ALL;

	if (!m_pSettings->valueBool("Notifications", "report_tracker_errors", false))
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

	QMutexLocker lock(&m_notificationMutex);

	for (int i = m_notifications.size() - 1; i > -1; i--)
	{
		int notificationType = m_notifications[i].notificationType;

		if ((m_notificationMask & notificationType) != notificationType)
		{
			m_notifications.removeAt(i);
		}
	}

	m_enabled = m_pSettings->valueBool("Notifications", "use_notification_sys", true);
}

NotificationSystem::~NotificationSystem()
{
}

void NotificationSystem::OnNewNotification(int notificationType, QString message, QVariant data)
{
	if (m_enabled)
	{
		if ((m_notificationMask & notificationType) == notificationType)
		{
			QMutexLocker lock(&m_notificationMutex);
			Notification n;
			n.message = message;
			n.data = data;
			n.notificationType = notificationType;
			bool wasEmpty = m_notifications.isEmpty();
			m_notifications.enqueue(n);

			if (wasEmpty && !m_isShwoingNotification)
			{
				m_notificationWaitCOndition.wakeAll();
				QMetaObject::invokeMethod(this, "dispatchNotifications", Qt::QueuedConnection);
			}
		}
	}
}

NotificationSystem::Notification NotificationSystem::getPendingNotification()
{
	QMutexLocker lock(&m_notificationMutex);

	while (m_notifications.empty())
	{
		m_notificationWaitCOndition.wait(&m_notificationMutex, 0);
	}

	return m_notifications.dequeue();
}

void NotificationSystem::dispatchNotifications()
{
	Notification notification = getPendingNotification();
	QSystemTrayIcon::MessageIcon icon = gessIcon(notification.notificationType);
	QBalloonTip::QBaloonType type = gessBaloonType(notification.notificationType);
	QWidget* balloon = QBalloonTip::showBalloon("CuteTorrent", notification.message, type, notification.data, icon, m_defaultMessageDuration);
	m_isShwoingNotification = true;
	balloon->installEventFilter(this);
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
		(notificationType & SYSTEM_ERROR) == SYSTEM_ERROR ||
		(notificationType & UPDATE_ERROR) == UPDATE_ERROR)
	{
		return QBalloonTip::Error;
	}

	return QBalloonTip::Info;
}

QSystemTrayIcon::MessageIcon NotificationSystem::gessIcon(int notificationType)
{
	if ((notificationType & TORRENT_INFO) == TORRENT_INFO ||
		(notificationType & TORRENT_COMPLETED) == TORRENT_COMPLETED ||
		(notificationType & UDPATE_INFO) == UDPATE_INFO)
	{
		return QSystemTrayIcon::Information;
	}

	if ((notificationType & TORRENT_ERROR) == TORRENT_ERROR ||
		(notificationType & RSS_ERROR) == TORRENT_ERROR ||
		(notificationType & TRACKER_ERROR) == TRACKER_ERROR ||
		(notificationType & SYSTEM_ERROR) == SYSTEM_ERROR ||
		(notificationType & UPDATE_ERROR) == UPDATE_ERROR)
	{
		return QSystemTrayIcon::Critical;
	}

	return QSystemTrayIcon::Information;
}

bool NotificationSystem::eventFilter(QObject* obj, QEvent* event)
{
	if (event->type() == QEvent::Close)
	{
		m_isShwoingNotification = false;

		if (! m_notifications.isEmpty())
		{
			m_notificationWaitCOndition.wakeAll();
			QMetaObject::invokeMethod(this, "dispatchNotifications", Qt::QueuedConnection);
		}
	}

	return false;
}

