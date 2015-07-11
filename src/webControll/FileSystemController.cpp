#include "FileSystemController.h"
#include <QUrl>
#include <QDir>
#include <json/json.h>
#include "qstorageinfo.h"
#include "StaticHelpers.h"
FileSystemController::~FileSystemController()
{
}

void FileSystemController::service(HttpRequest& request, HttpResponse& response)
{
	if (!CheckCreditinals(request, response))
	{
		return;
	}

	QString method = request.getMethod();

	if (method.compare("GET", Qt::CaseInsensitive) == 0)
	{
		QString currentPath = request.getParameter("currentPath");
		currentPath = QUrl::fromPercentEncoding(currentPath.toUtf8());
		QtJson::JsonArray respJson;

		if (!currentPath.isEmpty())
		{
			QDir pathDir(currentPath);

			if (pathDir.exists())
			{
				QStringList subdirsList = pathDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

				for (int i = 0; i < subdirsList.count(); i++)
				{
					QtJson::JsonObject dir;
					dir["type"] = "dir";
					dir["name"] = subdirsList[i];
					respJson.append(dir);
				}
			}
			else
			{
				QtJson::JsonObject errObj;
				errObj["type"] = "err";
				errObj["msg"] = "Path doesnot exist";
				respJson.append(errObj);
			}
		}
		else
		{
			QList<QStorageInfo> volumes = QStorageInfo::mountedVolumes();

			for (int i = 0; i < volumes.count(); i++)
			{
				QtJson::JsonObject drive;
				drive["type"] = "drive";
				drive["freeSpace"] = StaticHelpers::toKbMbGb(volumes[i].bytesFree());
				drive["size"] = StaticHelpers::toKbMbGb(volumes[i].bytesTotal());
				drive["name"] = volumes[i].rootPath();
				respJson.append(drive);
			}
		}

		response.setHeader("Content-Type", "application/json");
		response.write(QtJson::serialize(respJson));
	}
	else
	{
		response.setStatus(405, "Method Not Allowed");
		response.write("<BODY><h3>405 Method Not Allowed.</h3>");
		response.write("</BODY>");
	}
}