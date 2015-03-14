#include "MagnetApiController.h"
#include "MetaDataDownloadWaiter.h"

MagnetApiController::MagnetApiController(QObject* parrent) : HttpRequestHandler("WebControl", parrent), m_pTorrentManager(TorrentManager::getInstance())
{
}

MagnetApiController::~MagnetApiController(void)
{
	TorrentManager::freeInstance();
}

void MagnetApiController::service(HttpRequest& request, HttpResponse& response)
{
	if(!CheckCreditinals(request, response))
	{
		return;
	}

	if(request.getMethod() == "POST")
	{
		QString magnetLink = request.getParameter("magnetLink");
		QString savePath = request.getParameter("savePath");

		if(magnetLink.isEmpty() || savePath.isEmpty())
		{
			response.write("Invalid data passed");
			response.setStatus(500, "Internal server error");
			return;
		}

		savePathMap.insert(magnetLink, savePath);
		qRegisterMetaType<openmagnet_info> ("openmagnet_info");
		MetaDataDownloadWaiter* magnetWaiter = new MetaDataDownloadWaiter(magnetLink);
		QObject::connect(magnetWaiter, SIGNAL(DownloadCompleted(openmagnet_info)), this, SLOT(DownloadMetadataCompleted(openmagnet_info)));
		//	QMessageBox::critical(NULL,"ERROR","NOT_CONNECTID");
		magnetWaiter->start(QThread::HighPriority);
		response.setStatus(200, "OK");
	}
	else
	{
		response.setStatus(405, "Method Not Allowed");
		response.write("<BODY><h3>405 Method Not Allowed.</h3>");
		response.write("</BODY>");
	}
}

void MagnetApiController::DownloadMetadataCompleted(const openmagnet_info& _info)
{
	m_pTorrentManager->AddMagnet(_info.handle, savePathMap[_info.link], "", QMap<QString, qint8>());
}
