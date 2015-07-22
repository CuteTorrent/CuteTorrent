#ifndef _NOTIFICATION_SYSTEM_INCLUDED_
#define _NOTIFICATION_SYSTEM_INCLUDED_

#include <QObject>
#include "QBaloon.h"
#include "ServiceCommon.h"
#include <QMutex>
#include <QWaitCondition>
#include "Singleton.h"
class QApplicationSettings;
class QSystemTrayIcon;
class QTimerEvent;
class NotificationSystem : public QObject, public Singleton<NotificationSystem>
{
	friend class Singleton<NotificationSystem>;
	Q_OBJECT
public slots:
	void OnNewNotification(int notificationType, QString message, QVariant data);
private slots :
	void dispatchNotifications();
public:
	enum NotificationType
	{
		TRACKER_ERROR = 1,
		DISK_ERROR = 2,
		RSS_ERROR = 4,
		TORRENT_ERROR = 8,
		SYSTEM_ERROR = 16,
		TORRENT_INFO = 32,
		TORRENT_COMPLETED = 64,
		UDPATE_INFO = 128,
		UPDATE_ERROR = 256,
		ALL = TRACKER_ERROR | DISK_ERROR | RSS_ERROR | TORRENT_ERROR | SYSTEM_ERROR | TORRENT_INFO | UDPATE_INFO | UPDATE_ERROR | TORRENT_COMPLETED,
		ERRORS = TRACKER_ERROR | DISK_ERROR | RSS_ERROR | TORRENT_ERROR | SYSTEM_ERROR | UPDATE_ERROR
	};

	~NotificationSystem();
	void UpdateNotificationSettings();


private:
	struct Notification
	{
		int notificationType;
		QString message;
		QVariant data;
	};
	Notification getPendingNotification();
	QQueue<Notification> m_notifications;
	NotificationSystem();
	QMutex m_notificationMutex;
	QWaitCondition m_notificationWaitCOndition;
	QBalloonTip::QBaloonType gessBaloonType(int notificationType);
	QSystemTrayIcon::MessageIcon gessIcon(int notificationType);
	QApplicationSettingsPtr m_pSettings;
	bool m_isShwoingNotification;
	bool m_enabled;
	int m_notificationMask;
	int m_defaultMessageDuration;
protected:
	bool eventFilter(QObject* obj, QEvent* event) override;
};
#endif //_NOTIFICATION_SYSTEM_INCLUDED_ 
