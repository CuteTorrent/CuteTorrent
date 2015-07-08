#ifndef TORRENTRACKER_H
#define TORRENTRACKER_H

#include <QObject>
#include "http/httplistener.h"
#include "trackerrequestmapper.h"
#include "Singleton.h"
class TorrentTracker : public QObject, public Singleton<TorrentTracker>
{
	friend class Singleton<TorrentTracker>;
	Q_OBJECT
private:
	HttpListener* httpServer;
	TrackerRequestHandler* requestHandler;
	
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

DEFINE_PTR_CALSS(TorrentTracker)
#endif // TORRENTRACKER_H
