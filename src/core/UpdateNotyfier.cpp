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
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "Version.h"
#include <QStringList>
#include "NotificationSystem.h"
UpdateNotifier::UpdateNotifier(QObject* parent) : QObject(parent)
{
	m_manager = new QNetworkAccessManager(this);
	connect(m_manager, SIGNAL(finished(QNetworkReply*)),
	        this, SLOT(replyFinished(QNetworkReply*)));
	m_pNotificationSystem = NotificationSystem::getInstance();
	connect(this, SIGNAL(Notify(int, QString, QVariant)), m_pNotificationSystem.get(), SLOT(OnNewNotification(int, QString, QVariant)));
}

void UpdateNotifier::fetch()
{
	m_manager->get(QNetworkRequest(QUrl("https://raw.githubusercontent.com/ruslanfedoseenko/CuteTorrent/master/UpdateInfo/version")));
}

void UpdateNotifier::replyFinished(QNetworkReply* pReply)
{
	QByteArray data = pReply->readAll();
	QString str(data);
	QStringList parts = str.split('.');

	if(parts.count() != 4)
	{
		if (pReply->error() != QNetworkReply::NoError)
		{
			emit Notify(NotificationSystem::UPDATE_ERROR, tr("ERROR_GETTING_VERSION_STR %1").arg(pReply->errorString()), QVariant());
			return;
		}

		emit Notify(NotificationSystem::UPDATE_ERROR, tr("ERROR_GETTING_VERSION_STR"), QVariant());
		return;
	}

	Version current = Version::CurrentVersion();
	Version recived(str);

	if (recived > current)
	{
		emit Notify(NotificationSystem::UDPATE_INFO, tr("NEW_VERSION_AVALIABLE %1").arg(str), QVariant());
	}
}

UpdateNotifier::~UpdateNotifier()
{
	delete m_manager;
}
