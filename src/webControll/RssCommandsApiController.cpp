#include "RssCommandsApiController.h"
#include "RssManager.h"
RssCommandsApiController::RssCommandsApiController(QObject* parent) : HttpRequestHandler("WebControl", parent), m_pRssManager(RssManager::getInstance())
{
	qRegisterMetaType<QUuid>("QUuid");
}

void RssCommandsApiController::service(HttpRequest& request, HttpResponse& response)
{
	if (!CheckCreditinals(request, response))
	{
		return;
	}

	QString method = request.getMethod();

	if (method.compare("post", Qt::CaseInsensitive) == 0)
	{
		QString feedId = request.getParameter("feedId");
#ifdef Q_WS_WIN
		feedId = QString("{%1}").arg(feedId);
#endif
		QUuid uid(feedId);
		QString action = request.getParameter("action");
		RssFeed* pFeed = m_pRssManager->findFeed(uid);

		if (pFeed != NULL)
		{
			if (action.compare("remove", Qt::CaseInsensitive) == 0)
			{
				//QEventLoop loop;
				if (!QMetaObject::invokeMethod(m_pRssManager.get(), "removeFeed", Qt::QueuedConnection, Q_ARG(QUuid, uid)))
				{
					qWarning() << "QMetaObject::invokeMethod removeFeed failed";
				}

				//m_pRssManager->removeFeed(uid);
			}
			else if (action.compare("downloadItem", Qt::CaseInsensitive) == 0)
			{
			}
		}
		else
		{
			response.setStatus(404, "Not Found");
			response.write("<BODY><h3>404 RSS Feed not found.</h3>");
			response.write("</BODY>");
		}
	}
	else
	{
		response.setStatus(405, "Method Not Allowed");
		response.write("<BODY><h3>405 Method Not Allowed.</h3>");
		response.write("</BODY>");
	}
}
