#include "RequestMapper.h"
#include <QDir>
#include <QApplication>
#include "http/staticfilecontroller.h"
#include "TorrentApiController.h"
#include "UploadController.h"
#include "TorrentCommandsApiController.h"
#include "SettingsAPiController.h"
#include "MagnetApiController.h"
#include "FileSystemController.h"
#include "RssController.h"
#include "RssCommandsApiController.h"
RequestMapper::RequestMapper(QObject* parent)
	: HttpRequestHandler("WebControl", parent)
{
	FileSystemController* fsController = new FileSystemController(this);
	StaticFileController* staticFileController = new StaticFileController(this);
	TorrentApiController* torrentController = new TorrentApiController(this);
	UploadController* uploadController = new UploadController(this);
	TorrentCommandsApiController* commandsController = new TorrentCommandsApiController(this);
	SettingsAPiController* settingsController = new SettingsAPiController(this);
	MagnetApiController* magnetController = new MagnetApiController(this);
	RssController* rssController = new RssController(this);
	RssCommandsApiController* rssCommandsController = new RssCommandsApiController(this);
	m_pRequestMap.insert("/rss/action", rssCommandsController);
	m_pRequestMap.insert("/rss/api", rssController);
	m_pRequestMap.insert("/fs/api", fsController);
	m_pRequestMap.insert("/magnet/api", magnetController);
	m_pRequestMap.insert("/settings/api", settingsController);
	m_pRequestMap.insert("/torrent/api", torrentController);
	m_pRequestMap.insert("/torrent/upload", uploadController);
	m_pRequestMap.insert("/torrent/action", commandsController);
	m_pRequestMap.insert("default", staticFileController);
}


void RequestMapper::service(HttpRequest& request, HttpResponse& response)
{
	QString path = request.getPath();
	bool isProcessed = false;
	QMap<QString, HttpRequestHandler*>::Iterator it = m_pRequestMap.begin(), end = m_pRequestMap.end();

	for (it; it != end; ++it)
	{
		if (path.startsWith(it.key(), Qt::CaseInsensitive))
		{
			it.value()->service(request, response);
			isProcessed = true;
			break;
		}
	}

	if (!isProcessed)
	{
		m_pRequestMap["default"]->service(request, response);
	}
}

RequestMapper::~RequestMapper()
{
	qDeleteAll(m_pRequestMap.values());
}
