#include "TorrentCommandsApiController.h"
#include "TorrentManager.h"
TorrentCommandsApiController::TorrentCommandsApiController(QObject* parent/*=0*/) : HttpRequestHandler("WebControl", parent), m_pTorrentManager(TorrentManager::getInstance())
{}

void TorrentCommandsApiController::service(HttpRequest& request, HttpResponse& response)
{
	if(requireAuth)
	{
		if(!CheckCreditinals(request, response))
		{
			return;
		}
	}

	QString method = request.getMethod();
	if(method.compare("post", Qt::CaseInsensitive) == 0)
	{
		QString action = request.getParameter("action");
		QString id = request.getParameter("id");
		Torrent* tor = m_pTorrentManager->GetTorrentByInfoHash(id);

		if(tor != nullptr)
		{
			if(action == "pause")
			{
				tor->pause();
			}
			else if(action == "resume")
			{
				tor->resume();
			}
			else if(action == "remove")
			{
				if (!QMetaObject::invokeMethod(m_pTorrentManager, "RemoveTorrent", Qt::QueuedConnection, Q_ARG(QString, id), Q_ARG(bool, true)))
				{
					qWarning() << "QMetaObject::invokeMethod RemoveTorrent FAILED";
				}
				//m_pTorrentManager->RemoveTorrent(id, true);
			}
			else
			{
			}
		}
		else
		{
		}
	}
	else
	{
		response.setStatus(405, "Method Not Allowed");
		response.write("<BODY><h3>405 Method Not Allowed.</h3>");
		response.write("</BODY>");
	}
}

TorrentCommandsApiController::~TorrentCommandsApiController()
{
	TorrentManager::freeInstance();
}
