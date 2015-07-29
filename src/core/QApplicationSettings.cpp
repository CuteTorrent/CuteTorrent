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
#include "QApplicationSettings.h"
#include "messagebox.h"
#include <QMutex>
#include <QSettings>
#include "StaticHelpers.h"
#include "SympleCrypt.h"


QApplicationSettings::QApplicationSettings()
{
	QString dataDir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
	QString oldStylePath = StaticHelpers::CombinePathes(QApplication::applicationDirPath(), "CuteTorrent.ini");
	QString newStylePath = StaticHelpers::CombinePathes(dataDir, "settings.ini");
	QFile oldFile(oldStylePath);

	if (oldFile.exists())
	{
		if (!oldFile.rename(newStylePath))
		{
			CustomMessageBox::warning(NULL, "Move file failed", QString("Failed to move %1 to %2!").arg(oldStylePath, newStylePath));
		}
	}

	settings = new QSettings(newStylePath, QSettings::IniFormat);
	locker = new QMutex();
    m_pCryptor = new SimpleCrypt(0x1D5AE35A4BDD232LL);
	ReedSettings();
}

QApplicationSettings::~QApplicationSettings()
{
	WriteSettings();
}

QStringList QApplicationSettings::GetGroupNames()
{
	locker->lock();
	QStringList res = settings->childGroups();
	locker->unlock();
	return res;
}

void QApplicationSettings::setValue(const QString& group, const QString& key, const QVariant& value)
{
	if(!settings->group().isEmpty())
	{
		settings->endGroup();
	}

	settings->beginGroup(group);
	QVariant currentValue = settings->value(key);
	if (currentValue != value)
	{
		settings->setValue(key, value);
		emit PropertyChanged(group, key);
	}
	settings->endGroup();
	WriteSettings();
}

QVariant QApplicationSettings::value(const QString& group, const QString& key, const QVariant& defaultVal)
{
	locker->lock();

	if(!settings->group().isEmpty())
	{
		settings->endGroup();
	}

	settings->beginGroup(group);
	QVariant res = settings->value(key);

	if(!res.isValid() &&  defaultVal.isValid())
	{
		settings->setValue(key, defaultVal);
		res = defaultVal;
	}

	settings->endGroup();
	locker->unlock();
	return res;
}
int QApplicationSettings::valueInt(const QString& group, const QString& key, const int& defalt)
{
	try
	{
		QVariant val = value(group, key, defalt);
		return val.toInt();
	}
	catch(...)
	{
		return defalt;
	}
}

QMap<QString, QVariant> QApplicationSettings::getGroupValues(QString group)
{
	locker->lock();

	if(!settings->group().isEmpty())
	{
		settings->endGroup();
	}

	settings->beginGroup(group);
	QStringList keys = settings->childKeys();
	QMap<QString, QVariant> result;

	foreach(QString key, keys)
	{
		result.insert(key, settings->value(key));
	}

	settings->endGroup();
	locker->unlock();
	return result;
}
void QApplicationSettings::setGroupValues(QString group, QMap<QString, QVariant> values)
{
	locker->lock();

	if(!settings->group().isEmpty())
	{
		settings->endGroup();
	}

	settings->beginGroup(group);
	QStringList keys = values.keys();

	foreach(QString key, keys)
	{
		settings->setValue(key, values[key]);
	}

	settings->endGroup();
	locker->unlock();
}
QString	QApplicationSettings::valueString(const QString& group, const QString& key, const QString& defalt)
{
	QVariant val = value(group, key, defalt);
	return val.toString();
}

bool QApplicationSettings::valueBool(const QString& group, const QString& key, const bool defalt)
{
	QVariant val = value(group, key, defalt);
	return val.toBool();
}

float QApplicationSettings::valueFloat(const QString& group, const QString& key, float defalt)
{
	QVariant val = value(group, key, defalt);
	return val.toFloat();
}

void  QApplicationSettings::ReedSettings()
{
	settings->sync();
}
void QApplicationSettings::SaveFilterGropups(QList<GroupForFileFiltering>& filters)
{
	locker->lock();
	settings->beginGroup("FileFiltering");
	settings->remove("");

	for(int i = 0; i < filters.count(); i++)
	{
		GroupForFileFiltering group = filters.at(i);
		settings->setValue(group.Name(), QString("savepath;;%1;;extensions;;%2").arg(group.SavePath()).arg(group.Extensions()));
	}

	settings->endGroup();
	locker->unlock();
}
QList<GroupForFileFiltering> QApplicationSettings::GetFileFilterGroups()
{
	QList<GroupForFileFiltering> res;
	locker->lock();

	if(!settings->group().isEmpty())
	{
		settings->endGroup();
	}

	settings->beginGroup("FileFiltering");
	QStringList groupNames = settings->childKeys();

	for(int i = 0; i < groupNames.count(); i++)
	{
		QStringList tmpVal = settings->value(groupNames.at(i)).toString().split(";;");

		if(tmpVal.at(0) != "savepath" && tmpVal.count() < 4)
		{
			continue;
		}
		else
		{
			QString savepath = tmpVal.at(1);
			QString extensions = tmpVal.at(3);
			res.append(GroupForFileFiltering(groupNames.at(i), extensions, savepath));
		}
	}

	settings->endGroup();
	locker->unlock();
	return res;
}

QString QApplicationSettings::securedValueString(const QString& group, const QString& key, const QString& defalt)
{
	QString encryptedValue = valueString(group, key, defalt);
	return m_pCryptor->decryptToString(encryptedValue);
}

void QApplicationSettings::setSecuredValue(const QString& group, const QString& key, const QString& value)
{
	QString encryptedValue = m_pCryptor->encryptToString(value);
	setValue(group, key, encryptedValue);
}

void  QApplicationSettings::WriteSettings()
{
	settings->sync();
}

QList<SchedulerTask> QApplicationSettings::GetSchedullerQueue()
{
	QList<SchedulerTask> res;
	res.clear();
	QString dataDir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
	QFile file(StaticHelpers::CombinePathes(dataDir, "BtSessionData/schedulertasks.xml"));

	if(!file.open(QFile::ReadOnly))
	{
		return res;
	}

	QXmlStreamReader xml(&file);

	while(!xml.atEnd())
	{
		QXmlStreamReader::TokenType tType = xml.readNext();

		if(xml.name() == "tasks")
		{
			continue;
		}

		if(xml.name() != "task")
		{
			continue;
		}

		if(tType != QXmlStreamReader::StartElement)
		{
			continue;
		}

		QXmlStreamAttributes atribbutes = xml.attributes();
		SchedulerTask::TaskType type = SchedulerTask::UNKNOWN;
		int limit = 0;
		QDateTime begin, end;
		QString name;

		if(atribbutes.hasAttribute("TYPE"))
		{
			QString val = atribbutes.value("TYPE").toString();

			if(val == "PAUSE_ALL")
			{
				type = SchedulerTask::PAUSE_ALL;
			}
			else if(val == "START_ALL")
			{
				type = SchedulerTask::START_ALL;
			}
			else if(val == "LIMIT_DL")
			{
				type = SchedulerTask::LIMIT_DOWNLOAD;
			}
			else if(val == "LIMIT_UL")
			{
				type = SchedulerTask::LIMIT_UPLOAD;
			}
			else
			{
			}
		}

		if(atribbutes.hasAttribute("NAME"))
		{
			name = atribbutes.value("NAME").toString();
		}

		if(atribbutes.hasAttribute("LIMIT"))
		{
			QString val = atribbutes.value("LIMIT").toString();
			limit = val.toInt();
		}

		if(atribbutes.hasAttribute("TBEGIN"))
		{
			QString val = atribbutes.value("TBEGIN").toString();
			begin = QDateTime::fromString(val, "dd:MM:yyyy hh:mm:ss");
		}

		if(atribbutes.hasAttribute("TEND"))
		{
			QString val = atribbutes.value("TEND").toString();
			end = QDateTime::fromString(val, "dd:MM:yyyy hh:mm:ss");
		}

		SchedulerTask task(name, type, qVariantFromValue(limit), begin);
		res.push_back(task);
	}

	if(xml.hasError())
	{
	}

	file.close();
	return res;
}


void QApplicationSettings::SaveSchedullerQueue(QList<SchedulerTask>& tasks)
{
	QString dataDir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
	QFile file(StaticHelpers::CombinePathes(dataDir, "BtSessionData/schedulertasks.xml"));

	if(!file.open(QFile::WriteOnly))
	{
		CustomMessageBox::warning(NULL, "", "Error open for writing BtSessionData/schedulertasks.xml");
		return;
	}

	QXmlStreamWriter xml(&file);
	xml.setAutoFormatting(true);
	xml.writeStartDocument();
	qSort(tasks);
	xml.writeStartElement("tasks");

	for(QQueue<SchedulerTask>::iterator i = tasks.begin(); i != tasks.end(); ++i)
	{
		xml.writeStartElement("task");
		xml.writeAttribute("TYPE", StaticHelpers::SchedulerTypeToString(i->type()));
		xml.writeAttribute("NAME", i->name());
		xml.writeAttribute("LIMIT", QString::number(i->limit()));
		xml.writeAttribute("TBEGIN", i->startTime().toString("dd:MM:yyyy hh:mm:ss"));
		xml.writeEndElement(); // bookmark
	}

	xml.writeEndElement();
	xml.writeEndDocument();
	file.close();
}
