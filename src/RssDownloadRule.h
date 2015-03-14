#include <QtCore>
#include <xstring>
#include <libtorrent/rss.hpp>
class RssDownloadRule
{
public:
	bool Match(libtorrent::feed_item);
	std::string Name();
	void SetName(std::string);
	void SetName(QString);
	bool UseSavePath();
	void SetUseSavePath(bool value);
	std::string SavePath();
	void SetSavePath(std::string);
	void SetSavePath(QString);
	bool UseTorrentFiltering();
	void SetUseTorrentFiltering(bool value);
	std::string IncludeContent();
	void SetIncludeContent(std::string);
	void SetIncludeContent(QString);
private:
	std::string m_sName;
	bool m_bUseSavePath;
	std::string m_sSavePath;
	bool m_bUseTorrentFiltering;
	std::string m_sIncludeContent;
};