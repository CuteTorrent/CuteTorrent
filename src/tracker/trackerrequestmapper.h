#ifndef TRACKERREQUESTMAPPER_H
#define TRACKERREQUESTMAPPER_H

#include <QObject>
#include "http/httprequesthandler.h"
#include "AnnounceRequest.h"
class TrackerRequestHandler : public HttpRequestHandler
{
	Q_OBJECT
public:

	explicit TrackerRequestHandler(QObject* parent = 0);
	void service(HttpRequest& request, HttpResponse& response);
private:
	typedef QMap<QString, PeerInfo> PeerList;
	QMap<QString, PeerList> torrents;
	QMap<int, char*> failtureText;
	void ProcessAnnounceRequest(HttpRequest& request, HttpResponse& response);
	void ReplyWithPeerList(HttpResponse& response, AnnounceRequest& announceRequest);
	void ReplyError(int code, HttpResponse& response);
signals:

public slots:

};

#endif // TRACKERREQUESTMAPPER_H
