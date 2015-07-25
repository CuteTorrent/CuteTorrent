#include "ValueGetters.h"
#include <QApplicationSettings.h>
#include <StyleEngene.h>
#include <application.h>
#include <StaticHelpers.h>

QVariant ValueGetters::SettingsValueGetter(QString group, QString name, QVariant defaultValue)
{
	return QApplicationSettings::getInstance()->value(group, name, defaultValue);
}

QVariant ValueGetters::SettingsEncryptedValueGetter(QString group, QString name, QVariant defaultValue)
{
	return QApplicationSettings::getInstance()->securedValueString(group, name, defaultValue.toString());
}

QVariant ValueGetters::StyleValueGetter(QString group, QString name, QVariant defaultValue)
{
	StyleEngene* styleEngine = StyleEngene::getInstance();
	QList<StyleInfo> styleInfos = styleEngine->getAvaliableStyles();
	QString currentStyle = QApplicationSettings::getInstance()->valueString(group, name, defaultValue.toString());
	for (int i = 0; i < styleInfos.size(); i++)
	{
		if (styleInfos[i].InternalName == currentStyle)
		{
			return i;
		}
	}
	return -1;
}

QVariant ValueGetters::LanguageValueGetter(QString group, QString name, QVariant defaultValue)
{
	QString currentLanguageCode = QApplicationSettings::getInstance()->valueString(group, name, defaultValue.toString());
	QStringList availableLanguages = Application::availableLanguages();
	return availableLanguages.indexOf(currentLanguageCode);
}

QVariant ValueGetters::MagnetAssociationValueGetter(QString group, QString name, QVariant defaultValue)
{
#ifdef Q_WS_WIN
	QSettings assocSettings("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
	QString magnetCommand = QDir::toNativeSeparators(assocSettings.value("Magnet/shell/open/command/.").toString());
	QString applicationFilePath = QDir::toNativeSeparators(QFileInfo(QApplication::applicationFilePath()).absoluteFilePath());
	return QVariant::fromValue(magnetCommand.contains(applicationFilePath, Qt::CaseInsensitive) == true);
#elif Q_WS_X11
	QFile associtaionGnomeConfig(StaticHelpers::CombinePathes(QDir::homePath() , ".config/mimeapps.list"));

	if (associtaionGnomeConfig.open(QFile::ReadOnly))
	{
		QByteArray asscoiationData = associtaionGnomeConfig.readAll();
		associtaionGnomeConfig.close();
		QString associationStr = QString::fromUtf8(asscoiationData);
		QStringList lines = associationStr.split('\n');

		foreach(QString line, lines)
		{
			if (line.startsWith("x-scheme-handler/magnet", Qt::CaseInsensitive))
			{
				return QVariant::fromValue(line.endsWith("CuteTorrent.desktop;", Qt::CaseInsensitive) == true);
			}
		}
		associtaionGnomeConfig.close();
	}
	else
	{
		qCritical() << "Unable to open gnome config file for reading" << associtaionGnomeConfig.errorString();
	}
	return false;
#endif
}

QVariant ValueGetters::TorrentAssociationValueGetter(QString group, QString name, QVariant defaultValue)
{
#ifdef Q_WS_WIN
	QSettings assocSettings("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
	QString torrentAssociation = assocSettings.value(".torrent/.").toString();

	return torrentAssociation == "CuteTorrent.file";
#elif Q_WS_X11
	QFile associtaionGnomeConfig(StaticHelpers::CombinePathes(QDir::homePath() , ".config/mimeapps.list"));

	if (associtaionGnomeConfig.open(QFile::ReadOnly))
	{
		QByteArray asscoiationData = associtaionGnomeConfig.readAll();
		associtaionGnomeConfig.close();
		QString associationStr = QString::fromUtf8(asscoiationData);
		QStringList lines = associationStr.split('\n');

		foreach(QString line, lines)
		{
			if (line.startsWith("application/x-bittorrent", Qt::CaseInsensitive))
			{
				return QVariant::fromValue(line.endsWith("CuteTorrent.desktop;", Qt::CaseInsensitive) == true);
			}
		}
		associtaionGnomeConfig.close();
	}
	else
	{
		qCritical() << "Unable to open gnome config file for reading" << associtaionGnomeConfig.errorString();
	}
	return false;
#endif
}

QVariant ValueGetters::RunOnBootValueGetter(QString group, QString name, QVariant defaultValue)
{

#ifdef Q_WS_WIN
	QSettings bootUpSettings(QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\") + (StaticHelpers::IsWow64() ? "Wow6432Node\\" : "") + "Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
	QString bootStartCommand = bootUpSettings.value("CuteTorrent").toString();
	QString applicationFilePath = QDir::toNativeSeparators(QFileInfo(QApplication::applicationFilePath()).absoluteFilePath());
	return QVariant::fromValue(bootStartCommand.contains(applicationFilePath, Qt::CaseInsensitive) == true);
#elif Q_WS_X11
	return QFile::exists(StaticHelpers::CombinePathes(QDir::homePath(), ".config/autostart/CuteTorrent.desktop"));
#endif
}
#ifdef Q_WS_WIN
QVariant ValueGetters::RunOnBootMinimizedValueGetter(QString group, QString name, QVariant defaultValue)
{
	QSettings bootUpSettings(QString("HKEY_LOCAL_MACHINE\\SOFTWARE\\") + (StaticHelpers::IsWow64() ? "Wow6432Node\\" : "") + "Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
	QString bootStartCommand = bootUpSettings.value("CuteTorrent").toString();
	QString applicationFilePath = QDir::toNativeSeparators(QFileInfo(QApplication::applicationFilePath()).absoluteFilePath());
	return QVariant::fromValue(bootStartCommand.contains(applicationFilePath, Qt::CaseInsensitive) == true && bootStartCommand.contains("-m") == true);

}

QVariant ValueGetters::WindowsShellValueGetter(QString group, QString name, QVariant defaultValue)
{
	QSettings assocSettings("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
	QString applicationFilePath = QDir::toNativeSeparators(QFileInfo(QApplication::applicationFilePath()).absoluteFilePath());
	QString commandShouldBe = QString("\"%1\" --create_torrent \"%2\"").arg(applicationFilePath, "%1");
	bool starCommandMatch = assocSettings.value("*/shell/cutetorrent/command/.") == commandShouldBe;
	bool folderCommandMatch = assocSettings.value("Folder/shell/cutetorrent/command/.") == commandShouldBe;
	bool dirCommandMatch = assocSettings.value("Directory/shell/cutetorrent/command/.") == commandShouldBe;

	return starCommandMatch && folderCommandMatch && dirCommandMatch;
}
#endif