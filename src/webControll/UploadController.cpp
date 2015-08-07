#include "UploadController.h"
#include <QDir>
#include "StaticHelpers.h"
#include "TorrentManager.h"
UploadController::UploadController(QObject* parent) : HttpRequestHandler("WebControl", parent), m_pTorrentManager(TorrentManager::getInstance())
{
}

void UploadController::service(HttpRequest& request, HttpResponse& response)
{
	if(!CheckCreditinals(request, response))
	{
		return;
	}

	if(request.getMethod() == "POST")
	{
		//std::map<QByteArray,QByteArray> parametrs=request.getParameterMap().toStdMap();
		QString save_path = request.getParameter("savePath");
		QTemporaryFile* file = request.getUploadedFile("files[]");

		if (file != NULL)
		{
			if (save_path.isEmpty() || !QDir(save_path).exists())
			{
				response.setStatus(400, "Bad Request");
				response.write("<BODY><h3>400 Bad Request.</h3>");
				response.write("<h3>Invalid save-path</h3></BODY>");
				return;
			}

			error_code ec;
			QString tempFileName = file->fileName();
			m_pTorrentManager->AddTorrent(tempFileName, save_path, ec);

			if (ec)
			{
				response.setStatus(500, " Internal Server Error");
				response.write("<BODY><h3>500  Internal Server Error.</h3>");
				response.write(QString("<h3>" + StaticHelpers::translateLibTorrentError(ec) + "</h3></BODY>").toUtf8());
				return;
			}
		}
		else
		{
			response.setStatus(400, "Bad Request");
			response.write("<BODY><h3>400 Bad Request.</h3>");
			response.write("<h3>File not specified</h3></BODY>");
		}
	}
	else
	{
		response.setStatus(405, "Method Not Allowed");
		response.write("<BODY><h3>405 Method Not Allowed.</h3>");
		response.write("</BODY>");
	}
}

UploadController::~UploadController()
{
}
