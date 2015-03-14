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
#include "UpdateNotyfier.h"
#include <QDebug>
#include "versionInfo.h"
#include <QUrl>
#include <QStringList>
#include "QBaloon.h"
UpdateNotifier::UpdateNotifier()
{
	m_manager = new QNetworkAccessManager(this);
	connect(m_manager, SIGNAL(finished(QNetworkReply*)),
	        this, SLOT(replyFinished(QNetworkReply*)));
}

void UpdateNotifier::fetch()
{
	m_manager->get(QNetworkRequest(QUrl("http://cutetorrent.googlecode.com/svn/trunk/CuteTorrent/UpdateInfo/version")));
}

void UpdateNotifier::replyFinished(QNetworkReply* pReply)
{
	QByteArray data = pReply->readAll();
	QString str(data);
	int currentVersion = 1000 * VERSION_MAJOR + 100 * VERSION_MINOR + 10 * VERSION_REVISION + VERSION_BUILD;
	int recevedVersion = 0;
	QStringList parts = str.split('.');

	if(parts.count() != 4)
	{
		QBalloonTip::showBalloon(tr("ERROR_STR"), tr("ERROR_GETTING_VERSION_STR"), QBalloonTip::Error, QVariant(0), QSystemTrayIcon::Critical);
	}

	if(!pReply->isFinished())
	{
		QBalloonTip::showBalloon(tr("ERROR_STR"), pReply->errorString(), QBalloonTip::Error, QVariant(0), QSystemTrayIcon::Critical);
	}

	int mul = 1000;

	for(int i = 0; i < parts.count(); i++)
	{
		recevedVersion += mul * parts[i].toInt();
		mul /= 10;
	}

	if(recevedVersion > currentVersion)
	{
		emit showUpdateNitify(str);
	}
}

UpdateNotifier::~UpdateNotifier()
{
	delete m_manager;
}
