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
#include <QString>
#include <QIcon>
#include <QSet>
#include <QFileInfo>
#include <QApplication>

#include <QFile>
#include <QFileDialog>

#include <QInputDialog>
#include <QObject>

#include <QStyle>
#include "TorrentManager.h"
#include "SchedulerTask.h"
#include "defs.h"
class StaticHelpers
{
public:

	static QString toKbMbGb(libtorrent::size_type size);
	static QString translateLibTorrentError(boost::system::error_code const& ec);
	static void dellDir(QString path);
	static QString toTimeString(int seconds);
	static QString filePriorityToString(int priority);
	static QString SchedulerTypeToString(SchedulerTask::TaskType type);
	static QString GetBaseSuffix(const libtorrent::file_storage& storrage);
	static QString CombinePathes(QString path, QString suffix);
	template <typename T> static QList<T> reversed(const QList<T>& in);
};

template <typename T> QList<T>
StaticHelpers::reversed(const QList<T>& in)
{
	QList<T> result;
	result.reserve(in.size());    // reserve is new in Qt 4.7
	std::reverse_copy(in.begin(), in.end(), std::back_inserter(result));
	return result;
}

#endif
