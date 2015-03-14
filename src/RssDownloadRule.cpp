#include "RssDownloadRule.h"

bool RssDownloadRule::Match(libtorrent::feed_item item)
{
	QString include = QString::fromStdString(m_sIncludeContent);
	QString title = QString::fromStdString(item.title);
	QString description = QString::fromStdString(item.description);

	return !include.isEmpty() && (title.contains(include) || description.contains(include));
}

std::string RssDownloadRule::Name()
{
	return m_sName;
}

void RssDownloadRule::SetName(std::string value)
{
	m_sName = value;
}

void RssDownloadRule::SetName(QString value)
{
	m_sName = value.toStdString();
}

bool RssDownloadRule::UseSavePath()
{
	return m_bUseSavePath;
}

void RssDownloadRule::SetUseSavePath(bool value)
{
	m_bUseSavePath = value;
}

std::string RssDownloadRule::SavePath()
{
	return m_sSavePath;
}

void RssDownloadRule::SetSavePath(std::string value)
{
	m_sSavePath = value;
}

void RssDownloadRule::SetSavePath(QString value)
{
	m_sSavePath = value.toStdString();
}

bool RssDownloadRule::UseTorrentFiltering()
{
	return m_bUseTorrentFiltering;
}

void RssDownloadRule::SetUseTorrentFiltering(bool value)
{
	m_bUseTorrentFiltering = value;
}

std::string RssDownloadRule::IncludeContent()
{
	return m_sIncludeContent;
}

void RssDownloadRule::SetIncludeContent(std::string value)
{
	m_sIncludeContent = value;
}

void RssDownloadRule::SetIncludeContent(QString value)
{
	m_sIncludeContent = value.toStdString();
}
