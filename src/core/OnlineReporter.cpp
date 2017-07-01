#include "OnlineReporter.h"
#include <QNetworkInterface>
#include <QNetworkRequest>
#include <QCryptographicHash>
#include <QNetworkReply>
#include <QEventLoop>
#if defined(Q_OS_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include "Windows.h"
#include "Iptypes.h"
#endif
OnlineReporter::OnlineReporter(QObject* parent) : m_workerThread(new QThread(this)), m_pNetworAccessManager(new QNetworkAccessManager(this))
{
	moveToThread(m_workerThread);
	m_pNetworAccessManager->moveToThread(m_workerThread);
	connect(m_workerThread, SIGNAL(started()), SLOT(RunLoop()));
	connect(m_workerThread, SIGNAL(finished()), m_workerThread, SLOT(deleteLater()));
	connect(m_workerThread, SIGNAL(finished()), SLOT(deleteLater()));
}

void OnlineReporter::start()
{
	m_isRunning = true;
	qDebug() << "OnlineReporter::start";
	m_workerThread->start();
}

void OnlineReporter::stop()
{
	m_isRunning = false;
}

void OnlineReporter::RunLoop() const
{
	int sz = sizeof(IP_ADAPTER_INFO);
	qDebug() << "OnlineReporter::RunLoop" << m_isRunning << sz;
	ulong secondsCounter = 0;
	int ms = 1000;
	QNetworkReply* reply = m_pNetworAccessManager->get(QNetworkRequest(buildUrl()));
	QEventLoop loop;
	connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();
	qDebug() << reply->error() << reply->errorString() << reply->readAll();
	while (m_isRunning)
	{
		if (secondsCounter == 1800)
		{
			secondsCounter = 0;
			QNetworkReply* reply2 = m_pNetworAccessManager->get(QNetworkRequest(buildUrl()));
			connect(reply2, SIGNAL(finished()), &loop, SLOT(quit()));
			loop.exec();
			qDebug() << reply2->error() << reply2->errorString() << reply2->readAll();
		}

#if defined(Q_OS_WIN)
		Sleep(DWORD(ms));
#else
		struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
		nanosleep(&ts, NULL);
#endif
		secondsCounter++;
	}
	qDebug() << "OnlineReporter::RunLoop exit" << m_isRunning;
}

QString OnlineReporter::getMacAddress()
{
	foreach(QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
		{
			// Return only the first non-loopback MAC Address
			if (!(netInterface.flags() & QNetworkInterface::IsLoopBack))
				return netInterface.hardwareAddress();
		}
	return QString();
}

QString OnlineReporter::buildUrl() const
{
	QString mac = getMacAddress();

	QString macHash = QCryptographicHash::hash(mac.toUtf8(), QCryptographicHash::Md5).toHex();
	qDebug() << "Mac:" << mac << "UserID" << macHash;

	return QString("http://integration.cutetorrent.info/tracking/online?uid=%1").arg(macHash);
}

