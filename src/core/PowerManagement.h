
#ifndef POWERMANAGEMENT_INCLUDED
#define POWERMANAGEMENT_INCLUDED
#include <Qt>
#include "Singleton.h"
#include <QObject>

class PowerManagement : public QObject, public Singleton<PowerManagement>
{
	Q_OBJECT
protected:
	PowerManagement();
public:
	
	enum ActionType
	{
		NONE,
		SHUTDOWN,
		REBOOT,
		SLEEP,
		HIBERNATE
	};
	enum IdleType
	{
		NOT_IDLE,
		IDLE_DOWNLOAD,
		IDLE_ALL
	};
	friend class Singleton<PowerManagement>;
	void setIdleType(IdleType idleType);
	void setIdleAction(ActionType idleAction);
	void pefromActionOnIdle(IdleType currentIdleType);
	void hibernate();
	void sleep();
	void reboot();
	void shutdown();
private:
	IdleType m_idleType;
	ActionType m_idleAction;
#ifdef Q_WS_WIN
	bool m_bInited;
	void winInitPriveleges();
#endif
signals:
	void resetPowerState();
};

#endif
