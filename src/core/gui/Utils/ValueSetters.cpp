#include "ValueSetters.h"
#include <QApplicationSettings.h>
#include <StyleEngene.h>
#include <application.h>
#include <StaticHelpers.h>

void ValueSetters::SettingsValueSetter(QString group, QString name, QVariant value)
{
	QApplicationSettings::getInstance()->setValue(group, name, value);
}

void ValueSetters::SettingsEncryptedValueSetter(QString group, QString name, QVariant value)
{
	QApplicationSettings::getInstance()->setSecuredValue(group, name, value.toString());
}

void ValueSetters::StyleValueSetter(QString group, QString name, QVariant value)
{
	int styleIndex = value.toInt();
	StyleEngene* styleEngene = StyleEngene::getInstance();
	QList<StyleInfo> styleInfos = styleEngene->getAvaliableStyles();

	if (styleIndex < styleInfos.size() && styleIndex > -1)
	{
		QString currentStyle = styleInfos[styleIndex].InternalName;
		QApplicationSettings::getInstance()->setValue(group, name, currentStyle);

		if (styleEngene->getCuurentStyle().InternalName.compare(currentStyle) != 0)
		{
			styleEngene->setStyle(currentStyle);
		}
	}
	else
	{
		qCritical() << "Style index out of bounds";
	}
}

void ValueSetters::LanguageValueSetter(QString group, QString name, QVariant value)
{
	QStringList availableLanguages = Application::availableLanguages();
	int languageIndex = value.toInt();

	if (languageIndex < availableLanguages.size() && languageIndex > -1)
	{
		QString languageCode = availableLanguages[languageIndex];
		QApplicationSettings::getInstance()->setValue(group, name, languageCode);
		Application::setLanguage(languageCode);
		Application::setLanguageQt(languageCode);
	}
	else
	{
		qCritical() << "Langiage index out of bounds";
	}
}

void ValueSetters::MagnetAssociationValueSetter(QString group, QString name, QVariant value)
{
	bool associate = value.toBool();
#ifdef Q_WS_WIN
	QSettings asocSettings("HKEY_CURRENT_USER\\SOFTWARE\\Classes", QSettings::NativeFormat);
	QString applicationFilePath = QDir::toNativeSeparators(QFileInfo(QApplication::applicationFilePath()).absoluteFilePath());

	if (associate)
	{
		asocSettings.setValue("Magnet/.", "Magnet URI");
		asocSettings.setValue("Magnet/Content Type", "application/x-magnet");
		asocSettings.setValue("Magnet/URL Protocol", "");
		asocSettings.setValue("Magnet/shell/open/command/.",
		                      "\"" + applicationFilePath + "\"" + " \"%1\"");
	}
	else
	{
		asocSettings.remove("Magnet/.");
		asocSettings.remove("Magnet/Content Type");
		asocSettings.remove("Magnet/URL Protocol");
		asocSettings.remove("Magnet/shell/open/command/.");
	}

#else Q_WS_X11
	QFile associtaionGnomeConfig(StaticHelpers::CombinePathes(QDir::homePath() , ".config/mimeapps.list"));

	if (associtaionGnomeConfig.exists())
	{
		if (associtaionGnomeConfig.open(QFile::ReadOnly))
		{
			QByteArray asscoiationData = associtaionGnomeConfig.readAll();
			associtaionGnomeConfig.close();
			QString associationStr = QString::fromUtf8(asscoiationData);
			QStringList lines = associationStr.split('\n');
			bool magnetFound = false;

			for (int i = 0; i < lines.size(); i++)
			{
				QString line = lines[i];

				if (line.startsWith("x-scheme-handler/magnet", Qt::CaseInsensitive))
				{
					magnetFound = true;

					if (associate)
					{
						lines[i] = "x-scheme-handler/magnet=CuteTorrent.desktop;";
					}
					else
					{
						lines[i] = "";
					}
				}
			}

			if (!magnetFound && associate)
			{
				lines.append("x-scheme-handler/magnet=CuteTorrent.desktop;\n");
			}

			if (associtaionGnomeConfig.open(QFile::WriteOnly))
			{
				for (int i = 0; i < lines.size(); i++)
				{
					QString line = lines[i];

					if (!line.isEmpty())
					{
						associtaionGnomeConfig.write((line + "\n").toUtf8());
					}
				}

				associtaionGnomeConfig.close();
			}
			else
			{
				qCritical() << "Unable to open gnome config file for writing file asscoiations" << associtaionGnomeConfig.errorString();
			}
		}
		else
		{
			qCritical() << "Unable to open gnome config file for reading file asscoiations" << associtaionGnomeConfig.errorString();
		}
	}
	else
	{
		QStringList lines;
		lines << "x-scheme-handler/magnet=CuteTorrent.desktop;";

		if (associtaionGnomeConfig.open(QFile::WriteOnly))
		{
			for (int i = 0; i < lines.size(); i++)
			{
				QString line = lines[i];

				if (!line.isEmpty())
				{
					associtaionGnomeConfig.write((line + "\n").toUtf8());
				}
			}

			associtaionGnomeConfig.close();
		}
		else
		{
			qCritical() << "Unable to open gnome config file for writing file asscoiations" << associtaionGnomeConfig.errorString();
		}
	}

#endif
}

void ValueSetters::TorrentAssociationValueSetter(QString group, QString name, QVariant value)
{
	bool associate = value.toBool();
#ifdef Q_WS_WIN
	QSettings asocSettings("HKEY_CURRENT_USER\\SOFTWARE\\Classes", QSettings::NativeFormat);
	QString applicationFilePath = QDir::toNativeSeparators(QFileInfo(QApplication::applicationFilePath()).absoluteFilePath());

	if (associate)
	{
		asocSettings.setValue(".torrent/.", "CuteTorrent.file");
		asocSettings.setValue("CuteTorrent.file/.", QApplication::translate("SettingsDialog", "Torrent file"));
		asocSettings.setValue(".torrent/OpenWithProgids/CuteTorrent.file", "");
		asocSettings.setValue("CuteTorrent.file/shell/open/command/.",
		                      "\"" + applicationFilePath + "\"" + " \"%1\"");
		asocSettings.setValue("CuteTorrent.file/DefaultIcon/.",
		                      applicationFilePath + ",1");
	}
	else
	{
		asocSettings.remove(".torrent/OpenWithProgids/CuteTorrent.file");
		asocSettings.remove(".torrent/.");
		asocSettings.remove("CuteTorrent.file/.");
		asocSettings.remove("CuteTorrent.file/shell/open/command/.");
		asocSettings.remove("CuteTorrent.file/DefaultIcon/.");
	}

#else Q_WS_X11
	QFile associtaionGnomeConfig(StaticHelpers::CombinePathes(QDir::homePath(), ".config/mimeapps.list"));

	if (associtaionGnomeConfig.exists())
	{
		if (associtaionGnomeConfig.open(QFile::ReadOnly))
		{
			QByteArray asscoiationData = associtaionGnomeConfig.readAll();
			associtaionGnomeConfig.close();
			QString associationStr = QString::fromUtf8(asscoiationData);
			QStringList lines = associationStr.split('\n');
			bool torrentFound = false;

			for (int i = 0; i < lines.size(); i++)
			{
				QString line = lines[i];

				if (line.startsWith("application/x-bittorrent", Qt::CaseInsensitive))
				{
					torrentFound = true;

					if (associate)
					{
						lines[i] = "application/x-bittorrent=CuteTorrent.desktop;";
					}
					else
					{
						lines[i] = "";
					}
				}
			}

			if (!torrentFound && associate)
			{
				lines.append("application/x-bittorrent=CuteTorrent.desktop;\n");
			}

			if (associtaionGnomeConfig.open(QFile::WriteOnly))
			{
				for (int i = 0; i < lines.size(); i++)
				{
					QString line = lines[i];

					if (!line.isEmpty())
					{
						associtaionGnomeConfig.write((line + "\n").toUtf8());
					}
				}

				associtaionGnomeConfig.close();
			}
			else
			{
				qCritical() << "Unable to open gnome config file for writing file asscoiations" << associtaionGnomeConfig.errorString();
			}
		}
		else
		{
			qCritical() << "Unable to open gnome config file for reading file asscoiations" << associtaionGnomeConfig.errorString();
		}
	}
	else
	{
		QStringList lines;
		lines << "application/x-bittorrent=CuteTorrent.desktop;";

		if (associtaionGnomeConfig.open(QFile::WriteOnly))
		{
			for (int i = 0; i < lines.size(); i++)
			{
				QString line = lines[i];

				if (!line.isEmpty())
				{
					associtaionGnomeConfig.write((line + "\n").toUtf8());
				}
			}

			associtaionGnomeConfig.close();
		}
		else
		{
			qCritical() << "Unable to open gnome config file for writing file asscoiations" << associtaionGnomeConfig.errorString();
		}
	}

#endif
}

void ValueSetters::RunOnBootValueSetter(QString group, QString name, QVariant value)
{
	bool runOnBoot = value.toBool();
#ifdef Q_WS_WIN
	QSettings bootUpSettings(QString("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), QSettings::NativeFormat);
	QString applicationFilePath = QDir::toNativeSeparators(QFileInfo(QApplication::applicationFilePath()).absoluteFilePath());

	if (runOnBoot)
	{
		bootUpSettings.setValue("CuteTorrent", "\"" + applicationFilePath + "\"");
	}
	else
	{
		bootUpSettings.remove("CuteTorrent");
	}

#else Q_WS_X11

	if (runOnBoot)
	{
		if (!QFile::exists(StaticHelpers::CombinePathes(QDir::homePath(), ".config/autostart/CuteTorrent.desktop")))
		{
			QFile shortcut("/usr/share/applications/CuteTorrent.desktop");
			std::string autostartDir = StaticHelpers::CombinePathes(QDir::homePath(), ".config/autostart").toUtf8().data();
			error_code ec;
			create_directories(autostartDir, ec);

			if (ec || !shortcut.copy(StaticHelpers::CombinePathes(QDir::homePath(), ".config/autostart/CuteTorrent.desktop")))
			{
				qCritical() << "Unable to copy /usr/share/applications/CuteTorrent.desktop to ~/.config/autostart/CuteTorrent.desktop" << shortcut.errorString();
			}
		}
	}
	else
	{
		QFile shortcut(StaticHelpers::CombinePathes(QDir::homePath(), ".config/autostart/CuteTorrent.desktop"));

		if (shortcut.exists())
		{
			if (!shortcut.remove())
			{
				qCritical() << "failed to remove ~/.config/autostart/CuteTorrent.desktop" << shortcut.errorString();
			}
		}
	}

#endif
}
#ifdef Q_WS_WIN
void ValueSetters::RunOnBootMinimizedValueSetter(QString group, QString name, QVariant value)
{
	bool runOnBoot = value.toBool();
	QSettings bootUpSettings(QString("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), QSettings::NativeFormat);
	QString applicationFilePath = QDir::toNativeSeparators(QFileInfo(QApplication::applicationFilePath()).absoluteFilePath());

	if (runOnBoot)
	{
		bootUpSettings.setValue("CuteTorrent", "\"" + applicationFilePath + "\"" + " -m");
	}
	else
	{
		bootUpSettings.setValue("CuteTorrent", "\"" + applicationFilePath + "\"");
	}
}

void ValueSetters::WindowsShellValueSetter(QString group, QString name, QVariant value)
{
	bool enabled = value.toBool();
	QSettings asocSettings("HKEY_CURRENT_USER\\Software\\Classes", QSettings::NativeFormat);
	QString applicationFilePath = QDir::toNativeSeparators(QFileInfo(QApplication::applicationFilePath()).absoluteFilePath());

	if (enabled)
	{
		asocSettings.setValue("*/shell/cutetorrent/.", QApplication::translate("CustomWindow", "MENU_CREATE_TORRENT"));
		asocSettings.setValue("*/shell/cutetorrent/Icon", QString("\"%1\",0").arg(applicationFilePath));
		QString command = QString("\"%1\" --create_torrent \"%2\"").arg(applicationFilePath, "%1");
		asocSettings.setValue("*/shell/cutetorrent/command/.", command);
		asocSettings.setValue("Directory/shell/cutetorrent/.", QApplication::translate("CustomWindow", "MENU_CREATE_TORRENT"));
		asocSettings.setValue("Directory/shell/cutetorrent/Icon", QString("\"%1\",0").arg(applicationFilePath));
		asocSettings.setValue("Directory/shell/cutetorrent/command/.", command);
	}
	else
	{
		asocSettings.remove("*/shell/cutetorrent/.");
		asocSettings.remove("*/shell/cutetorrent/Icon");
		asocSettings.remove("*/shell/cutetorrent/command/.");
		asocSettings.remove("*/shell/cutetorrent/command");
		asocSettings.remove("*/shell/cutetorrent");
		asocSettings.remove("Directory/shell/cutetorrent/.");
		asocSettings.remove("Directory/shell/cutetorrent/Icon");
		asocSettings.remove("Directory/shell/cutetorrent/command/.");
		asocSettings.remove("Directory/shell/cutetorrent/command");
		asocSettings.remove("Directory/shell/cutetorrent");
	}
}
#endif

