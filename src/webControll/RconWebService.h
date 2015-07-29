#ifndef WEB_SVC_H
#define WEB_SVC_H

#include <QObject>
#include "http/httplistener.h"
#include "RequestMapper.h"


class RconWebService : public QObject, public Singleton<RconWebService>
{
	friend class Singleton<RconWebService>;
	Q_OBJECT
private:
	QList<QPair<uint, uint> > allowedIP, notAllowedIP;
	HttpListener* listener;
	RequestMapper* mapper;
	QApplicationSettingsPtr m_pSettings;
private slots:
	void OnSettngsChnaged(QString, QString);
protected:
	RconWebService(void);
public:
	~RconWebService(void);
	void parseIpFilter(QString filter);
	void Start();
	bool isRunning();
	void Stop();
};
#endif
