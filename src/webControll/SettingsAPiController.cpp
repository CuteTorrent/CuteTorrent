#include "SettingsAPiController.h"
#include "json/json.h"
SettingsAPiController::SettingsAPiController(QObject* parent) : HttpRequestHandler("WebControl", parent), settings(QApplicationSettings::getInstance())
{
}

SettingsAPiController::~SettingsAPiController(void)
{
	QApplicationSettings::FreeInstance();
}

void SettingsAPiController::service(HttpRequest& request, HttpResponse& response)
{
	if(!CheckCreditinals(request, response))
	{
		return;
	}

	if(request.getMethod() == "GET")
	{
		QtJson::JsonObject jSettings;
		jSettings["listen_port"] = settings->valueInt("Torrent", "listen_port");
		jSettings["active_downloads"] = settings->valueString("Torrent", "active_downloads");
		jSettings["download_rate_limit"] = settings->valueString("Torrent", "download_rate_limit");
		jSettings["active_limit"] = settings->valueString("Torrent", "active_limit");
		jSettings["upload_rate_limit"] = settings->valueString("Torrent", "upload_rate_limit");
		jSettings["active_seeds"] = settings->valueString("Torrent", "active_seeds");
		jSettings["disk_io_read_mode"] = settings->valueString("Torrent", "disk_io_read_mode");
		jSettings["cache_size"] = settings->valueInt("Torrent", "cache_size") * 16;
		jSettings["use_disk_read_ahead"] = settings->valueBool("Torrent", "use_disk_read_ahead");
		jSettings["allow_reordered_disk_operations"] = settings->valueBool("Torrent", "allow_reordered_disk_operations");
		jSettings["lock_files"] = settings->valueBool("Torrent", "lock_files");
		jSettings["low_prio_disk"] = settings->valueBool("Torrent", "low_prio_disk");
		jSettings["use_read_cache"] = settings->valueString("Torrent", "use_read_cache");
		QtJson::JsonArray jFilteringGroups;
		QList<GroupForFileFiltering> filteringGroups = settings->GetFileFilterGroups();

		for(int i = 0; i < filteringGroups.count(); i++)
		{
			QtJson::JsonObject group;
			group["name"] = filteringGroups[i].Name();
			group["extensions"] = filteringGroups[i].Extensions();
			group["path"] = filteringGroups[i].SavePath();
			jFilteringGroups.append(group);
		}

		jSettings["filteringGroups"] = jFilteringGroups;
		response.setHeader("Content-Type", "application/json");
		response.write(QtJson::serialize(jSettings));
	}
	else if(request.getMethod() == "PUT" || request.getMethod() == "POST")
	{
		QString jsonSettings = request.getBody();
		bool ok;
		QtJson::JsonObject settingsVal = QtJson::parse(jsonSettings, ok).toMap();

		if(ok)
		{
			QVariantList groups = settingsVal["filteringGroups"].toList();

			if(!ok)
			{
				response.setStatus(500, "Internal Server Error");
				return;
			}

			settings->setValue("Torrent", "listen_port", settingsVal["listen_port"]);
			settings->setValue("Torrent", "active_downloads", settingsVal["active_downloads"]);
			settings->setValue("Torrent", "download_rate_limit", settingsVal["download_rate_limit"]);
			settings->setValue("Torrent", "active_limit", settingsVal["active_limit"]);
			settings->setValue("Torrent", "upload_rate_limit", settingsVal["upload_rate_limit"]);
			settings->setValue("Torrent", "active_seeds", settingsVal["active_seeds"]);
			settings->setValue("Torrent", "disk_io_read_mode", settingsVal["disk_io_read_mode"]);
			settings->setValue("Torrent", "cache_size", settingsVal["cache_size"].toInt() / 16);
			settings->setValue("Torrent", "use_disk_read_ahead", settingsVal["use_disk_read_ahead"]);
			settings->setValue("Torrent", "allow_reordered_disk_operations", settingsVal["allow_reordered_disk_operations"]);
			settings->setValue("Torrent", "lock_files", settingsVal["lock_files"]);
			settings->setValue("Torrent", "low_prio_disk", settingsVal["low_prio_disk"]);
			settings->setValue("Torrent", "use_read_cache", settingsVal["use_read_cache"]);
			QList<GroupForFileFiltering> filteringGroups;

			for(int i = 0; i < groups.size(); i++)
			{
				QMap<QString, QVariant> groupData = groups[i].toMap();
				filteringGroups.append(GroupForFileFiltering(groupData["name"].toString(), groupData["extensions"].toString(), groupData["path"].toString()));
			}

			settings->SaveFilterGropups(filteringGroups);
		}
		else
		{
			response.setStatus(400, "Bad Request");
			response.write("<BODY><h3>400 Bad Request.</h3></BODY>");
		}
	}
	else
	{
		response.setStatus(405, "Method Not Allowed");
		response.write("<BODY><h3>405 Method Not Allowed.</h3>");
		response.write("</BODY>");
	}
}
