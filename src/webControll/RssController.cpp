#include "RssController.h"
#include <json/json.h>
#include "RssManager.h"
#include "RssFeed.h"
#include "RssItem.h"

RssController::RssController(QObject* parent) : HttpRequestHandler("WebControl", parent), m_pRssManager(RssManager::getInstance())
{
}

RssController::~RssController()
{
}

void RssController::service(HttpRequest& request, HttpResponse& response)
{
	if (!CheckCreditinals(request, response))
	{
		return;
	}

	QString method = request.getMethod();

	if (method.compare("get", Qt::CaseInsensitive) == 0)
	{
		QtJson::JsonArray respJson;
		QString page = request.getParameter("page");
		QString pageSize = request.getParameter("pageSize");

		if (page.length() > 0)
		{
			bool ok;
			int iPage = page.toInt(&ok);

			if (ok)
			{
				int iPageSize = pageSize.toInt(&ok);

				if (ok)
				{
					QList<RssFeed*> rss_feeds = m_pRssManager->feeds();

					for (int i = (iPage - 1) * iPageSize; i < std::min(rss_feeds.count(), iPage * iPageSize); i++)
					{
						RssFeed* pFeed = rss_feeds[i];
						QtJson::JsonObject jFeed;
						jFeed["name"] = pFeed->displayName(true);
						QString feedId = pFeed->uid().toString().replace("{", "").replace("}", "");
						jFeed["id"] = feedId;
						jFeed["isUpdating"] = pFeed->isUpdating();
						QtJson::JsonArray jFeedItems;
						QList<RssItem*> feedItems = pFeed->GetFeedItems();

						for (int j = 0; j < feedItems.count(); j++)
						{
							RssItem* pFeedItem = feedItems[j];
							QtJson::JsonObject jFeedItem;
							jFeedItem["name"] = pFeedItem->title();
							jFeedItem["feedId"] = feedId;
							jFeedItem["describtion"] = pFeedItem->description();
							jFeedItem["category"] = pFeedItem->category();
							jFeedItems.append(jFeedItem);
						}

						jFeed["feedItems"] = jFeedItems;
						respJson.append(jFeed);
					}

					if (respJson.size() > 0)
					{
						QtJson::JsonObject first = respJson[0].toMap();
						first["rssFeedsCount"] = rss_feeds.count();
						respJson[0] = first;
					}

					response.setHeader("Content-Type", "application/json");
					response.write(QtJson::serialize(respJson));
				}
			}
		}
	}
	else if (method.compare("put", Qt::CaseInsensitive) == 0)
	{
		QUrl feedUrl = QUrl::fromPercentEncoding(request.getParameter("feedUrl"));
		QRegExp httpLinkRegexp("(http|ftp|https):\\/\\/[\\w\\-_]+(\\.[\\w\\-_]+)+([\\w\\-\\.,@?^=%&amp;:/~\\+#]*[\\w\\-\\@?^=%&amp;/~\\+#])?");

		if (httpLinkRegexp.exactMatch(feedUrl.toString()))
		{
			bool isNew = false;
			m_pRssManager->addFeed(feedUrl, isNew);

			if (!isNew)
			{
				QtJson::JsonObject resp;
				resp["isNew"] = isNew;
				response.setHeader("Content-Type", "application/json");
				response.write(QtJson::serialize(resp));
			}
		}
		else
		{
			response.setStatus(400, "Bad Request.");
			response.write("<BODY><h3>400 Bad Request.</h3>");
			response.write("<h3>feedUrl parameter not specified or invalid</h3>");
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

