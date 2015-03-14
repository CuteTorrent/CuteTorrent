#ifndef TORRENTRACKER_H
#define TORRENTRACKER_H

#include <QObject>
#include "http/httplistener.h"
#include "trackerrequestmapper.h"
class TorrentTracker : public QObject
{
	Q_OBJECT
private:
	static TorrentTracker* instance;
	static int instanceCount;
	HttpListener* httpServer;
	TrackerRequestHandler* requestHandler;
protected:
	explicit TorrentTracker(QObject* parent = 0);
	~TorrentTracker();
public:
	static TorrentTracker* getInstance();
	static void freeInstance();
	bool isRunning();
	void start();
	void stop();
signals:

public slots:

};

#endif // TORRENTRACKER_H
