#ifndef _ONLINE_REPORTER_INCLUDED
#define _ONLINE_REPORTER_INCLUDED
#include <QObject>
#include <QThread>
#include <QNetworkAccessManager>

class OnlineReporter : public QObject
{
	Q_OBJECT
public:
	OnlineReporter(QObject* parent = NULL);
	void start();
	void stop();
private slots:

	void RunLoop() const;
private:
	static QString getMacAddress();
	QString buildUrl() const;
	QThread* m_workerThread;
	QNetworkAccessManager* m_pNetworAccessManager;
	bool m_isRunning;
};
#endif

