#pragma once
#include <QStringList>
#include <QtNetwork\QTcpServer>
#include <QtNetwork\QTcpSocket>
#include <QMutex>
#include <QFile>
class HttpDaemon : public QTcpServer
{
	Q_OBJECT
public:
	HttpDaemon(quint16 port, QString baseFolder, QObject* parent = 0);

	void incomingConnection(int socket);
	void pause();
	void resume();
private slots:
	void readClient();
	void discardClient();
private:
	bool disabled;
	QString baseDir;
	QMutex locker;
};
