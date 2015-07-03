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
#include <QDir>
#include <QFileInfo>
#include <QTranslator>
#include <QFileOpenEvent>
#include "application.h"
#include <QDebug>
#ifdef Q_WS_WIN
#include <Windows.h>
#endif
QTranslator* Application::current = 0;
QTranslator* Application::currentQt = 0;
Translators Application::translators;
Translators Application::qt_translators;
QString Application::current_locale = "";
QString Application::current_locale_qt = "";
QString Application::default_locale = "ru_RU";
Application::Application(int& argc, char* argv[])
	: QtSingleApplication(argc, argv)
{}

Application::~Application()
{
	qDeleteAll(translators.values());
	qDeleteAll(qt_translators.values());
}
void Application::loadTranslations(const QString& dir)
{
	loadTranslations(QDir(dir));
}

void Application::loadTranslations(const QDir& dir)
{
	// <language>_<country>.qm
	QString filter = "*_*.qm";
	QDir::Filters filters = QDir::Files | QDir::Readable;
	QDir::SortFlags sort = QDir::Name;
	QFileInfoList entries = dir.entryInfoList(QStringList() << filter, filters, sort);

	foreach(QFileInfo file, entries)
	{
		// pick country and language out of the file name
		QStringList parts = file.baseName().split("_");
		QString language = parts.at(parts.count() - 2).toLower();
		QString country  = parts.at(parts.count() - 1).toUpper();
		// construct and load translator
		QTranslator* translator = new QTranslator(instance());

		if(translator->load(file.absoluteFilePath()))
		{
			QString locale = language + "_" + country;
			qDebug() << "New Application langugae found" << locale;
			translators.insert(locale, translator);
		}
	}
}

void Application::loadTranslationsQt(const QString& dir)
{
	loadTranslationsQt(QDir(dir));
}

void Application::loadTranslationsQt(const QDir& dir)
{
	// <language>_<country>.qm
	QString filter = "*_*.qm";
	QDir::Filters filters = QDir::Files | QDir::Readable;
	QDir::SortFlags sort = QDir::Name;
	QFileInfoList entries = dir.entryInfoList(QStringList() << filter, filters, sort);

	foreach(QFileInfo file, entries)
	{
		// pick country and language out of the file name
		QStringList parts = file.baseName().split("_");
		QString language = parts.at(parts.count() - 2).toLower();
		QString country  = parts.at(parts.count() - 1).toUpper();
		// construct and load translator
		QTranslator* translator = new QTranslator(instance());

		if(translator->load(file.absoluteFilePath()))
		{
			QString locale = language + "_" + country;
			qDebug() << "New Qt langugae found" << locale;
			qt_translators.insert(locale, translator);
		}
	}
}

QString Application::currentLocale()
{
	return current_locale;
}


QString Application::currentLocaleQt()
{
	return current_locale_qt;
}

const QStringList Application::availableLanguages()
{
	// the content won't get copied thanks to implicit sharing and constness
	qDebug() << translators.keys();
	return QStringList(translators.keys());
}

bool Application::event(QEvent* event)
{
	if(event->type() == QEvent::FileOpen)
	{
		QFileOpenEvent* fileOpenEvent = static_cast<QFileOpenEvent*>(event);

		if(fileOpenEvent->file().endsWith(".torrent", Qt::CaseInsensitive))
		{
			emit OpenTorrent(fileOpenEvent->file());
		}

		fileOpenEvent->accept();
	}

	return QtSingleApplication::event(event);
}

void Application::setLanguage(QString locale)
{
	qDebug() << "Application::setLanguage" << locale;

	// remove previous
	if(current)
	{
		removeTranslator(current);
	}

	if(!translators.contains(locale))
	{
		locale = default_locale;
	}

	current_locale = locale;
	// install new
	current = translators.value(locale);

	if(current)
	{
		installTranslator(current);
	}
}

void Application::setLanguageQt(QString locale)
{
	qDebug() << "Application::setLanguageQt" << locale;

	if(currentQt)
	{
		removeTranslator(currentQt);
	}

	if(!translators.contains(locale))
	{
		locale = default_locale;
	}

	current_locale_qt = locale;
	// install new
	currentQt = qt_translators.value(locale);

	if(currentQt)
	{
		installTranslator(currentQt);
	}
}
#ifdef Q_WS_WIN
bool Application::winEventFilter(MSG* message, long* result)
{
	UINT& msg = message->message;

	if(msg == WM_QUERYENDSESSION || msg == WM_ENDSESSION)
	{
		*result = 1;
		quit();
		return true;
	}

	return QApplication::winEventFilter(message, result);
}
#endif
