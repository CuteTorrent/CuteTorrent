#include "PowerManagement.h"
#include <QDebug>

#ifdef Q_WS_WIN
#include <windows.h>
#else
#include <QtDBus/QDBusMessage>
#include <QtDBus/qdbusconnection.h>
#include <QtDBus/QDBusInterface>
#include <QProcess>
#endif

PowerManagement::PowerManagement()
	: m_idleType(NOT_IDLE)
	, m_idleAction(NONE)
	, m_bInited(false)
{
	
}

void PowerManagement::setIdleType(IdleType idleType)
{
	m_idleType = idleType;
}

void PowerManagement::setIdleAction(ActionType idleAction)
{
	m_idleAction = idleAction;
}

void PowerManagement::pefromActionOnIdle(IdleType currentIdleType)
{
	if (currentIdleType != NOT_IDLE && m_idleType == currentIdleType)
	{
		switch (m_idleAction)
		{
			case SHUTDOWN: 
				shutdown();
				break;
			case REBOOT: 
				reboot();
				break;
			case SLEEP: 
				sleep();
				break;
			case HIBERNATE:
				hibernate();
				break;
			case NONE: 
			default: break;
		}
		m_idleType = NOT_IDLE;
		m_idleAction = NONE;
		emit resetPowerState();
	}
}
#ifdef Q_WS_WIN
void PowerManagement::winInitPriveleges()
{
	if (!m_bInited)
	{
		HANDLE hToken; 
		TOKEN_PRIVILEGES tkp;
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		{
			return;
		}

		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, static_cast<PTOKEN_PRIVILEGES>(NULL), 0))
		{
			
		}
		m_bInited = true;
	}
	
}
#endif

void PowerManagement::hibernate()
{
#ifdef Q_WS_WIN
	winInitPriveleges();
	bool successful = SetSystemPowerState(FALSE, FALSE);
	qDebug() << "PowerManagement::hibernate" << successful;
#else
	bool gnome_power1 = false;
	bool gnome_power2 = false;
	bool hal_works = false;
	QDBusMessage response;
	gnome_power1 = QProcess::startDetached("gnome-power-cmd.sh suspend");
	gnome_power2 = QProcess::startDetached("gnome-power-cmd suspend");
	if (!gnome_power1 && !gnome_power2 )
		qWarning() << "W: gnome-power-cmd and gnome-power-cmd.sh didn't work";

	if (!gnome_power1 && !gnome_power2){
		QDBusInterface powermanagement("org.freedesktop.Hal",
			"/org/freedesktop/Hal/devices/computer",
			"org.freedesktop.Hal.Device.SystemPowerManagement",
			QDBusConnection::systemBus());
		response = powermanagement.call("Suspend", 0);
		if (response.type() == QDBusMessage::ErrorMessage){
			
				qWarning() << "W: " << response.errorName() << ":" << response.errorMessage();
		}
		else
			hal_works = true;
	}

	if (!hal_works && !gnome_power1 && !gnome_power2){
		QDBusInterface powermanagement("org.freedesktop.DeviceKit.Power",
			"/org/freedesktop/DeviceKit/Power",
			"org.freedesktop.DeviceKit.Power", QDBusConnection::systemBus());
		response = powermanagement.call("Suspend");
		if (response.type() == QDBusMessage::ErrorMessage){
			
				qWarning() << "W: " << response.errorName() << ":" << response.errorMessage();
		}
	}
#endif
}

void PowerManagement::sleep()
{
#ifdef Q_WS_WIN
	winInitPriveleges();
	bool successful = SetSystemPowerState(TRUE, FALSE);
	qDebug() << "PowerManagement::sleep" << successful;
#else
	bool gnome_power1 = false;
	bool gnome_power2 = false;
	bool hal_works = false;
	QDBusMessage response;

	gnome_power1 = QProcess::startDetached("gnome-power-cmd.sh suspend");
	gnome_power2 = QProcess::startDetached("gnome-power-cmd suspend");
	if (!gnome_power1 && !gnome_power2 && verbose)
		qWarning() << "W: gnome-power-cmd and gnome-power-cmd.sh didn't work";

	if (!gnome_power1 && !gnome_power2){
		QDBusInterface powermanagement("org.freedesktop.Hal",
			"/org/freedesktop/Hal/devices/computer",
			"org.freedesktop.Hal.Device.SystemPowerManagement",
			QDBusConnection::systemBus());
		response = powermanagement.call("Suspend", 0);
		if (response.type() == QDBusMessage::ErrorMessage){
			if (verbose)
				qWarning() << "W: " << response.errorName() << ":" << response.errorMessage();
		}
		else
			hal_works = true;
	}

	if (!hal_works && !gnome_power1 && !gnome_power2){
		QDBusInterface powermanagement("org.freedesktop.DeviceKit.Power",
			"/org/freedesktop/DeviceKit/Power",
			"org.freedesktop.DeviceKit.Power", QDBusConnection::systemBus());
		response = powermanagement.call("Suspend");
		if (response.type() == QDBusMessage::ErrorMessage){
			if (verbose)
				qWarning() << "W: " << response.errorName() << ":" << response.errorMessage();
		}
	}
#endif
}

void PowerManagement::reboot()
{
#ifdef Q_WS_WIN
	winInitPriveleges();
	bool successful = ExitWindowsEx(EWX_REBOOT | EWX_FORCE,
		SHTDN_REASON_MAJOR_OPERATINGSYSTEM |
		SHTDN_REASON_MINOR_UPGRADE |
		SHTDN_REASON_FLAG_PLANNED);
	qDebug() << "PowerManagement::reboot" << successful;
	 
#else
	bool reboot_works = false;
	bool gnome_power1 = false;
	bool gnome_power2 = false;
	bool hal_works = false;
	bool verbose = true;
	QDBusMessage response;
	QDBusInterface gnomeSessionManager("org.gnome.SessionManager",
		"/org/gnome/SessionManager", "org.gnome.SessionManager",
		QDBusConnection::sessionBus());
	response = gnomeSessionManager.call("RequestReboot");
	if (response.type() == QDBusMessage::ErrorMessage){
		if (verbose)
			qWarning() << "W: " << response.errorName() << ":" << response.errorMessage();
		gnome_power1 = QProcess::startDetached("gnome-power-cmd.sh reboot");
		gnome_power2 = QProcess::startDetached("gnome-power-cmd reboot");
		if (verbose && !gnome_power1 && !gnome_power2)
			qWarning() << "W: gnome-power-cmd and gnome-power-cmd.sh didn't work";
	}
	else
		reboot_works = true;

	QDBusInterface kdeSessionManager("org.kde.ksmserver", "/KSMServer",
		"org.kde.KSMServerInterface", QDBusConnection::sessionBus());
	response = kdeSessionManager.call("logout", 0, 2, 1);
	if (response.type() == QDBusMessage::ErrorMessage){
		if (verbose)
			qWarning() << "W: " << response.errorName() << ":" << response.errorMessage();
	}
	else
		reboot_works = true;

	if (!reboot_works && !gnome_power1 && !gnome_power2){
		QDBusInterface powermanagement("org.freedesktop.Hal",
			"/org/freedesktop/Hal/devices/computer",
			"org.freedesktop.Hal.Device.SystemPowerManagement",
			QDBusConnection::systemBus());
		response = powermanagement.call("Reboot");
		if (response.type() == QDBusMessage::ErrorMessage){
			if (verbose)
				qWarning() << "W: " << response.errorName() << ":" << response.errorMessage();
		}
		else
			hal_works = true;
	}

	if (!hal_works && !reboot_works && !gnome_power1 && !gnome_power2){
		QDBusInterface powermanagement("org.freedesktop.ConsoleKit",
			"/org/freedesktop/ConsoleKit/Manager", "org.freedesktop.ConsoleKit.Manager",
			QDBusConnection::systemBus());
		response = powermanagement.call("Restart");
		if (response.type() == QDBusMessage::ErrorMessage){
			if (verbose)
				qWarning() << "W: " << response.errorName() << ":" << response.errorMessage();
			QProcess::startDetached("sudo shutdown -r now");
		}
	}
#endif
}

void PowerManagement::shutdown()
{
#ifdef Q_WS_WIN
	winInitPriveleges();
	bool successful = ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 
		SHTDN_REASON_MAJOR_OPERATINGSYSTEM |
		SHTDN_REASON_MINOR_UPGRADE |
		SHTDN_REASON_FLAG_PLANNED);
	qDebug() << "PowerManagement::shutdown" << successful;
#else
	bool shutdown_works = false;
	bool gnome_power1 = false;
	bool gnome_power2 = false;
	bool hal_works = false;
	bool verbose = true;
	QDBusMessage response;
	QDBusInterface gnomeSessionManager("org.gnome.SessionManager",
		"/org/gnome/SessionManager", "org.gnome.SessionManager",
		QDBusConnection::sessionBus());
	response = gnomeSessionManager.call("RequestShutdown");
	if (response.type() == QDBusMessage::ErrorMessage){
		if (verbose)
			qWarning() << "W: " << response.errorName() << ":" << response.errorMessage();
		gnome_power1 = QProcess::startDetached("gnome-power-cmd.sh shutdown");
		gnome_power2 = QProcess::startDetached("gnome-power-cmd shutdown");
		if (verbose && !gnome_power1 && !gnome_power2)
			qWarning() << "W: gnome-power-cmd and gnome-power-cmd.sh didn't work";
	}
	else
		shutdown_works = true;

	QDBusInterface kdeSessionManager("org.kde.ksmserver", "/KSMServer",
		"org.kde.KSMServerInterface", QDBusConnection::sessionBus());
	response = kdeSessionManager.call("logout", 0, 2, 2);
	if (response.type() == QDBusMessage::ErrorMessage){
		if (verbose)
			qWarning() << "W: " << response.errorName() << ":" << response.errorMessage();
	}
	else
		shutdown_works = true;

	if (!shutdown_works && !gnome_power1 && !gnome_power2){
		QDBusInterface powermanagement("org.freedesktop.Hal",
			"/org/freedesktop/Hal/devices/computer",
			"org.freedesktop.Hal.Device.SystemPowerManagement",
			QDBusConnection::systemBus());
		response = powermanagement.call("Shutdown");
		if (response.type() == QDBusMessage::ErrorMessage){
			if (verbose)
				qWarning() << "W: " << response.errorName() << ":" << response.errorMessage();
		}
		else
			hal_works = true;
	}

	if (!hal_works && !shutdown_works && !gnome_power1 && !gnome_power2){
		QDBusInterface powermanagement("org.freedesktop.ConsoleKit",
			"/org/freedesktop/ConsoleKit/Manager", "org.freedesktop.ConsoleKit.Manager",
			QDBusConnection::systemBus());
		response = powermanagement.call("Stop");
		if (response.type() == QDBusMessage::ErrorMessage){
			if (verbose)
				qWarning() << "W: " << response.errorName() << ":" << response.errorMessage();
			QProcess::startDetached("sudo shutdown -P now");
		}
	}

#endif
}
