#ifndef _NOTIFICATION_SYSTEM_INCLUDED_
#define _NOTIFICATION_SYSTEM_INCLUDED_

#include <QObject>
#include "QBaloon.h"
#include "ServiceCommon.h"
class QApplicationSettings;
class QSystemTrayIcon;
class NotificationSystem : public QObject
{
	Q_OBJECT
public slots:
	void OnNewNotification(int notificationType, QString message, QVariant data);
private slots :
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
		ALL = TRACKER_ERROR | DISK_ERROR | RSS_ERROR | TORRENT_ERROR | SYSTEM_ERROR | TORRENT_INFO | UDPATE_INFO | TORRENT_COMPLETED,
		ERRORS = TRACKER_ERROR | DISK_ERROR | RSS_ERROR | TORRENT_ERROR | SYSTEM_ERROR
	};
	
	static NotificationSystemPtr getInstance();
	~NotificationSystem();
	void setTrayIcon(QSystemTrayIcon* pTrayIcon);
	void UpdateNotificationMask();
private:
	static boost::weak_ptr<NotificationSystem> m_pInstance;
	NotificationSystem();
	QSystemTrayIcon* m_pTrayIcon;
	QBalloonTip::QBaloonType gessBaloonType(int notificationType);
	QSystemTrayIcon::MessageIcon gessIcon(int notificationType);

	QApplicationSettings* m_pSettings;
	bool m_enabled;
	bool m_useCustomFrame;
	int m_notificationMask;
protected:
};
#endif //_NOTIFICATION_SYSTEM_INCLUDED_ 
