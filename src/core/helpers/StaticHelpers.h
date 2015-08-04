/*
CuteTorrent BitTorrent Client with dht support, userfriendly interface
and some additional features which make it more convenient.
Copyright (C) 2012 Ruslan Fedoseyenko

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _S_H_H
#define _S_H_H

#include "TorrentManager.h"
#include "SchedulerTask.h"
#include "NetworkDiskCache.h"
#include <QDir>

class StaticHelpers
{
private:
	static NetworkDiskCache* m_pDiskCache;
	static QString translateSessionError(error_code const& errorCode);
	static QString translateBEncodeError(error_code const& ec);
	static QString translateGzipError(error_code const& ec);
	static QString translateI2PError(error_code const& ec);
	static QString translateSocksError(error_code const& ec);
	static QString translateUpnpError(error_code const& ec);
	static QString translateError(error_code const& ec, char* msgs[], int msgs_len);
	template<typename T> static T CombinePathes(T t);
public:
#ifdef Q_WS_X11
	static void OpenFolderNautilus(QString& file);
#endif
#ifdef Q_WS_WIN
	static void OpenFileInExplorer(QString& file);
	static bool IsWow64();
#endif
	static QString toKbMbGb(size_type size, bool isSpped = false);
	static QString translateLibTorrentError(error_code const& ec);
	static QString toTimeString(int seconds);
	static QString filePriorityToString(int priority);
	static QString SchedulerTypeToString(SchedulerTask::TaskType type);
	static QString GetBaseSuffix(const file_storage& storrage);
	
	template<typename T, typename... Args> static QString CombinePathes(T first, Args &... args);
	static NetworkDiskCache* GetGLobalWebCache();
	static QByteArray gUncompress(QByteArray data);
	template <typename T> static size_t HashVector(const std::vector<T>& vector);
	template <typename T> static QList<T> reversed(const QList<T>& in);
	static QTime SecsToQTime(int secs);
	static int QTimeToSecs(const QTime& time);
};


template <typename T>
T StaticHelpers::CombinePathes(T t)
{
	return t;
}

template<typename T, typename... Args>
QString StaticHelpers::CombinePathes(T first, Args &... args)
{
	bool addSeparator = !QString(first).endsWith("/") || !QString(first).endsWith("\\");
	return QDir::toNativeSeparators(QDir::cleanPath(QString(first) + (addSeparator ? QDir::separator() : QString("")) + CombinePathes(args...)));
}

template <typename T>
size_t StaticHelpers::HashVector(const std::vector<T>& vector)
{
	size_t result = 0;
	boost::hash_range(result, vector.begin(), vector.end());
	return result;
}

template <typename T> QList<T>
StaticHelpers::reversed(const QList<T>& in)
{
	QList<T> result;
	result.reserve(in.size());
	std::reverse_copy(in.begin(), in.end(), std::back_inserter(result));
	return result;
}

#endif
