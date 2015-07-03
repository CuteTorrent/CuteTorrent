#ifndef _EMEAIL_NOTIFIER_INCLUDED_
#define _EMEAIL_NOTIFIER_INCLUDED_

#include <QObject>


class EmailNotifier
{
public:
	explicit EmailNotifier(QObject* parent = NULL);
	void SendEmail(QString to, QString subject, QString body);

};
#endif