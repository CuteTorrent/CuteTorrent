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
#ifndef _GROUPFORFILTER_H
#define _GROUPFORFILTER_H

#include <QSet>
#include <QString>

class GroupForFileFiltering
{
private:
	QSet<QString> extensions;
	QString name, path;
public:
	GroupForFileFiltering(QString name, QString extensions, QString savepath);
	bool Contains(QString extension) const;
	QString SavePath() const;
	QString Name() const;
	QString Extensions() const;
	bool operator ==(const GroupForFileFiltering& other);
	bool operator !=(const GroupForFileFiltering& other);
};


#endif

