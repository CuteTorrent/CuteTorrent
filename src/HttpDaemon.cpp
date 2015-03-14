#include "HttpDaemon.h"
#include <QDebug>

HttpDaemon::HttpDaemon(quint16 port, QString baseFolder, QObject* parent /*= 0*/) : QTcpServer(parent), disabled(false)
{
	baseDir = baseFolder;
	listen(QHostAddress::Any, port);
}

void HttpDaemon::incomingConnection(int socket)
{
	if(disabled)
	{
		return;
	}

	QTcpSocket* s = new QTcpSocket(this);
	connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));
	connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
	s->setSocketDescriptor(socket);
}

void HttpDaemon::pause()
{
	disabled = true;
}

void HttpDaemon::resume()
{
	disabled = false;
}

void HttpDaemon::readClient()
{
	if(disabled)
	{
		return;
	}

	QTcpSocket* socket = (QTcpSocket*) sender();

	if(socket->canReadLine())
	{
		QStringList tokens = QString(socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));
		qDebug() << tokens;

		if(tokens[0] == "GET")
		{
			QTextStream os(socket);
			os.setAutoDetectUnicode(true);
			os << "HTTP/1.0"  + (QFile::exists(baseDir + tokens[1]) ? QString("200 Ok") : "404 File Not found") + "\r\n"
			   "Content-Type: text/html; charset=\"utf-8\"\r\n"
			   "\r\n";
			locker.lock();
			QFile file(baseDir + tokens[1]);

			if(file.open(QFile::ReadOnly))
			{
				while(!file.atEnd())
				{
					QByteArray line = file.readLine();
					os << file.readLine();
				}
			}

			locker.unlock();
			socket->close();

			if(socket->state() == QTcpSocket::UnconnectedState)
			{
				delete socket;
			}
		}
	}
}

void HttpDaemon::discardClient()
{
	QTcpSocket* socket = (QTcpSocket*) sender();
	socket->deleteLater();
}
