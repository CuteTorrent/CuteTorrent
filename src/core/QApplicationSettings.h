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
#ifndef _QAPPSETTINGS
#define _QAPPSETTINGS

#include "GroupForFileFiltering.h"
#include "SchedulerTask.h"
#include "ServiceCommon.h"
#include "Singleton.h"
class QMutex;
class QSettings;
class SimpleCrypt;
class QApplicationSettings : public QObject, public Singleton<QApplicationSettings>
{
	Q_OBJECT
	friend class Singleton<QApplicationSettings>;
protected:

	QApplicationSettings();

private:
	SimpleCrypt* m_pCryptor;
	QSettings* settings;
	QMutex* locker;
signals:
	void PropertyChanged(QString group, QString key);
public:
	~QApplicationSettings();
	void ReedSettings();
	QVariant value(const QString& group, const QString& key, const QVariant& defaultVal = QVariant(QVariant::Invalid));
	Q_DECL_DEPRECATED void SaveFilterGropups(QList<GroupForFileFiltering>&);
	Q_DECL_DEPRECATED QList<GroupForFileFiltering> GetFileFilterGroups();
	QStringList GetGroupNames();
	QList<SchedulerTask> GetSchedullerQueue();
	void SaveSchedullerQueue(QList<SchedulerTask>&);
	void setValue(const QString& group, const QString& key, const QVariant& value);
	int valueInt(const QString& group, const QString& key, const int& defalt = 0);
	QMap<QString, QVariant> getGroupValues(QString group);
	void setGroupValues(QString group, QMap<QString, QVariant> values);
	QString valueString(const QString& group, const QString& key, const QString& defalt = "");
	bool valueBool(const QString& group, const QString& key, bool defalt = true);
	float valueFloat(const QString& group, const QString& key, float defalt = 0.0f);
	QString securedValueString(const QString& group, const QString& key, const QString& defalt = "");
	void setSecuredValue(const QString& group, const QString& key, const QString& value);
	void WriteSettings();
};



#endif
