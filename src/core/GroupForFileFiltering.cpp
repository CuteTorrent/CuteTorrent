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
#include "GroupForFileFiltering.h"

#include <QStringList>


GroupForFileFiltering::GroupForFileFiltering(QString _name, QString _extensions, QString _savepath)
{
	name = _name;
	path = _savepath;
	QStringList extensionList = _extensions.split('|');

	if (extensionList.count() > 0)
	{
		for (int i = 0; i < extensionList.count(); i++)
		{
			if (!extensionList.at(i).isEmpty())
			{
				extensions.insert(extensionList.at(i));
			}
		}
	}
}

bool GroupForFileFiltering::Contains(QString extension) const
{
	return extensions.contains(extension);
}

QString GroupForFileFiltering::Name() const
{
	return name;
}

QString GroupForFileFiltering::Extensions() const
{
	QString res;

	for (QSet<QString>::ConstIterator i = extensions.begin(); i != extensions.end(); ++i)
	{
		res = res.append(*i);
		res = res.append("|");
	}

	return res;
}

bool GroupForFileFiltering::operator==(const GroupForFileFiltering& other)
{
	return other.extensions == extensions && other.name == name && other.path == path;
}

bool GroupForFileFiltering::operator!=(const GroupForFileFiltering& other)
{
	return !operator==(other);
}

QString GroupForFileFiltering::SavePath() const
{
	return path;
}

