#ifndef TORRENTRACKER_H
#define TORRENTRACKER_H

#include <QObject>
#include "trackercommon.h"
#include "ServiceCommon.h"
class TrackerRequestHandler;
class HttpListener;
class TorrentTracker : public QObject, public Singleton<TorrentTracker>
{
	friend class Singleton<TorrentTracker>;
	Q_OBJECT
private:
	HttpListener* httpServer;
	TrackerRequestHandler* requestHandler;
	QApplicationSettingsPtr m_pSettings;
private slots:
	void OnSettngsChnaged(QString, QString);
protected:
	explicit TorrentTracker(QObject* parent = 0);

public:
	bool isRunning();
	void start();
	void stop();
	~TorrentTracker();
signals:

public slots:

};
#endif // TORRENTRACKER_H
