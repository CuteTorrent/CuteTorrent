#ifndef _EMEAIL_NOTIFIER_INCLUDED_
#define _EMEAIL_NOTIFIER_INCLUDED_

#include <QObject>

class SmtpClient;
class EmailNotifier : public QObject
{
public:
	explicit EmailNotifier(QObject* parent = NULL);
	void SendEmail(QString to, QString subject, QString body);

};
#endif