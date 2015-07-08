#include "TorrentApiController.h"
#include <libtorrent/peer_info.hpp>
#include "json/json.h"
TorrentApiController::TorrentApiController(QObject* parent/*=0*/) : HttpRequestHandler("WebControl", parent), m_pTorrentManager(TorrentManager::getInstance()), m_pTorrentStorrage(TorrentStorrage::getInstance())
{

}

void TorrentApiController::service(HttpRequest& request, HttpResponse& response)
{
	if (!CheckCreditinals(request,response))
	{
		return;
	}
//	QMultiMap<QByteArray,QByteArray> parametrs=request.getParameterMap();
	if(request.getMethod().toUpper() == "GET")
	{
		QtJson::JsonArray respJson;
		QString page = request.getParameter("page");
		QString pageSize = request.getParameter("pageSize");

		if(page.length() > 0)
		{
			bool ok;
			int iPage = page.toInt(&ok);

			if(ok)
			{
				int iPageSize = pageSize.toInt(&ok);

				if(ok)
				{
					for(int i = (iPage - 1) * iPageSize; i < std::min(m_pTorrentStorrage->count(), iPage * iPageSize); i++)
					{
						QtJson::JsonObject torrent;
						Torrent* curret = m_pTorrentStorrage->at(i);
						torrent["id"] = curret->GetInfoHash();
						torrent["torrentType"] = curret->GetType();
						torrent["torrentName"] = curret->GetName();
						torrent["torrentStatus"] = QString::number(curret->GetStatus());
						torrent["hasError"] = curret->hasError();
						torrent["isPaused"] = curret->isPaused();
						torrent["errorMessage"] = curret->GetErrorMessage();
						torrent["progress"] = QString::number(curret->GetProgress()) ;
						torrent["escapedTime"] = curret->GetActiveTime() ;
						torrent["remainingTime"] = curret->GetRemainingTime() ;
						torrent["dlSpeed"] = curret->GetDwonloadSpeed() ;
						torrent["ulSpeed"] = curret->GetUploadSpeed() ;
						torrent["dlSize"] = curret->GetTotalDownloaded() ;
						torrent["ulSize"] = curret->GetTotalUploaded() ;
						torrent["describtion"] = curret->GetDiscribtion() ;
						torrent["torrentSize"] = curret->GetTotalSize() ;
						torrent["savePath"] = curret->GetSavePath();
						torrent["seedStr"] = curret->GetSeedString() ;
						torrent["peerStr"] = curret->GetPeerString() ;
						torrent["torrentsCount"] = m_pTorrentStorrage->count();
						QtJson::JsonArray peers;
						std::vector<peer_info> peerInfos = curret->GetPeerInfo();

						for(int j = 0; j < peerInfos.size(); j++)
						{
							QtJson::JsonObject peer;
							peer["ip"] = QString::fromStdString(peerInfos[j].ip.address().to_string()) ;
							peer["client"] = QString::fromUtf8(peerInfos[j].client.c_str()) ;
							peer["percent"] = QString::number(peerInfos[j].progress_ppm / 10000.f, 'f', 1) ;
							peer["peerDlSpeed"] = StaticHelpers::toKbMbGb(peerInfos[j].down_speed) + "/s" ;
							peer["peerUlSpeed"] = StaticHelpers::toKbMbGb(peerInfos[j].up_speed) + "/s" ;
							peer["dlSize"] = StaticHelpers::toKbMbGb(peerInfos[j].total_download) ;
							peer["ulSize"] = StaticHelpers::toKbMbGb(peerInfos[j].total_upload) ;
							peers.append(peer);
						}

						torrent["peers"] = peers;
						QtJson::JsonArray trackers;
						std::vector<announce_entry> trackerInfos = curret->GetTrackerInfo();

						for(int j = 0; j < trackerInfos.size(); j++)
						{
							QtJson::JsonObject tracker;
							tracker["url"] = QString::fromStdString(trackerInfos[j].url) ;
							tracker["state"] = (trackerInfos[j].message.length() > 0 ? QString::fromUtf8(trackerInfos[j].message.c_str()) : "Ok");
							trackers.append(tracker);
						}

						torrent["trackers"] = trackers;
						QtJson::JsonArray files;
						files_info fileinfo = curret->GetFileDownloadInfo();
						int counter = 0;

						for (int j = 0; j < fileinfo.storrage.num_files(); ++j)
						{
							QtJson::JsonObject file;
							file["path"] = QString::fromStdString(fileinfo.storrage.file_path(j));
							file["size"] = StaticHelpers::toKbMbGb(fileinfo.storrage.file_size(j)) ;
							file["percent"] = QString::number(fileinfo.progresses[counter] * 100.0f, 'f', 0) + " %";
							file["priority"] = StaticHelpers::filePriorityToString(fileinfo.priorities[counter]);
							counter++;
							files.append(file);
						}

						torrent["files"] = files;
						respJson.append(torrent);
					}

					response.setHeader("Content-Type", "application/json");
					response.write(QtJson::serialize(respJson));
				}
				else
				{
					response.setStatus(400, "Bad Request");
					response.write("<BODY><h3>400 Bad Request.</h3></BODY>");
				}
			}
			else
			{
				response.setStatus(400, "Bad Request");
				response.write("<h3>400 Bad Request.</h3>");
			}
		}
		else
		{
			response.setStatus(405, "Method Not Allowed");
			response.write("<BODY><h3>405 Method Not Allowed.</h3>");
			response.write("</BODY>");
		}
	}
}

TorrentApiController::~TorrentApiController()
{
}


