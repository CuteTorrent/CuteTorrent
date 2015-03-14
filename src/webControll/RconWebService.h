#ifndef WEB_SVC_H
#define WEB_SVC_H

#include <QObject>
#include "http/httplistener.h"
#include "RequestMapper.h"
class RconWebService : QObject
{
	Q_OBJECT
private:
	static RconWebService* instnce;
	static int intanceCount;
	QList<QPair<uint, uint> > allowedIP, notAllowedIP;
	HttpListener* listener;
	RequestMapper* mapper;
protected:
	RconWebService(void);
	~RconWebService(void);
public:
	void parseIpFilter(QString filter);
	void Start();
	bool isRunning();
	void Stop();
	static RconWebService* getInstance();
	static void freeInstance();
};
#endif
