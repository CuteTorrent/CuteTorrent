/*
CuteTorrent BitTorrent Client with dht support, user friendly interface
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

//#define Q_WS_WIN
#include <iostream>
#include "CuteTorrentMainWindow.h"
#include <qtsingleapplication.h>
#include <cstdio>
#include "application.h"
#include "StaticHelpers.h"

void myMessageOutput(QtMsgType type, const char* msg)
{
	fflush(stdout);

	switch(type)
	{
		case QtDebugMsg:
			printf("Debug: %s\n", msg);
			break;

		case QtWarningMsg:
			printf("Warning: %s\n", msg);
			break;

		case QtCriticalMsg:
			printf("Critical: %s\n", msg);
			break;

		case QtFatalMsg:
			printf("Fatal: %s\n", msg);
			abort();
	}

	fflush(stdout);
}

int main(int argc, char* argv[])
{/*
#ifdef Q_WS_WIN
	bool console = attachOutputToConsole();
#endif
	qsrand(time(NULL));
	po::options_description desc("CuteTorrent command line options");
	desc.add_options()
	("help,h", "Produce this message")
	("minimize,m", "Start application in minimized mode")
	("debug,d", "Write debug logs")
	("create_torrent,c", po::value<std::string>()->implicit_value(""), "Create a torrent from a specified path")
	("settings,s", "Open settings dialog")
	("input-file", po::value<std::string>()->default_value(""), "Open a torrent file");
	po::positional_options_description p;
	p.add("input-file", -1);
	po::variables_map vm;

	try
	{
		po::store(po::command_line_parser(argc, argv).
		          options(desc).positional(p).run(), vm);
		po::notify(vm);
	}
	catch (std::exception ex)
	{
		std::cout << "Error parsing cmd line: " << ex.what() << std::endl;
#ifdef Q_WS_WIN

		if (console)
		{
			sendEnterKey();
		}

#endif
		return 0;
	}*/

	Application a(argc, argv);
	QStringList arguments = Application::arguments();
	
	arguments.removeFirst();
	QVariantMap vm;

	vm["minimize"] = arguments.contains("-m") || arguments.contains("--minimize");
	arguments.removeAll("-m"); arguments.removeAll("--minimize");
	vm["debug"] = arguments.contains("-d") || arguments.contains("--debug");
	arguments.removeAll("-d"); arguments.removeAll("--debug");
	vm["settings"] = arguments.contains("-s") || arguments.contains("--settings");
	arguments.removeAll("-s"); arguments.removeAll("--settings");
	QString dataDir;
#ifdef Q_WS_MAC
	dataDir = "/Library/CuteTorrent/";
#else
	dataDir = QApplication::applicationDirPath() + QDir::separator();
#endif
	FILE* fp = NULL;

	if (vm["debug"].toBool())
	{
		QString logFileName = dataDir + "CuteTorrent." + QString::number(QApplication::applicationPid()) + ".log";
		fp = freopen(logFileName.toAscii().data(), "a+", stdout);
		qInstallMsgHandler(myMessageOutput);
	}
	if (arguments.contains("-c") || arguments.contains("--create_torrent"))
	{
		int index = arguments.indexOf("-c");
		if (index == -1)
		{
			index = arguments.indexOf("--create_torrent");
		}
		qDebug() << "--create_torrent index" << index;
		if (index != -1)
		{

			index++;
			if (arguments.size() > index)
			{
				QString path = arguments[index];
				vm["create_torrent"] = path;
				arguments.removeAll(path);
			}
			else
			{
				vm["create_torrent"] = "";
			}
			arguments.removeAll("-c"); arguments.removeAll("--create_torrent");
		}
	}

	if (!arguments.empty())
	{
		vm["input-file"] = arguments.first();
	}
	qDebug() << "arguments" << QApplication::arguments();
	qDebug() << "parsed values" << vm;
	
	a.setWindowIcon(QIcon(":/icons/app.ico"));
	a.setDesktopSettingsAware(false);
	a.setEffectEnabled(Qt::UI_FadeMenu, true);
	a.setEffectEnabled(Qt::UI_AnimateCombo, true);
	QString file2open;
	if (!vm.empty())
	{

		file2open = vm["input-file"].toString();
	}
	if(a.isRunning())
	{
		if (!file2open.isEmpty())
		{
			a.sendMessage(QString("open:%1").arg(file2open));
		}

		if (vm.contains("create_torrent"))
		{

			QString torrentCreationSource = vm["create_torrent"].toString();

			a.sendMessage(QString("create_torrent:%1").arg(torrentCreationSource));
		}

		if (vm["settings"].toBool())
		{
			a.sendMessage(QString("settings:"));
		}

		return 0;
	}
	else
	{
		if (vm.count("help"))
		{
			//std::cout << desc << std::endl;

			return 0;
		}
	}

	
	
	
	a.loadTranslations(":/translations");
	a.loadTranslationsQt(":/translations_qt");
	a.addLibraryPath(StaticHelpers::CombinePathes(QCoreApplication::applicationDirPath(), "plugins"));
	CuteTorrentMainWindow w;
	a.setActivationWindow(&w);
	a.setActiveWindow(&w);
	w.ConnectMessageReceved(&a);

	if (vm["minimize"].toBool())
	{
		w.showMinimized();
	}
	else
	{
		w.show();
	}

	if (!file2open.isEmpty())
	{
		w.HandleNewTorrent(file2open);
	}

	if (vm.contains("create_torrent"))
	{
		QString torrentCreationSource = vm["create_torrent"].toString();
		w.ShowCreateTorrentDialog(torrentCreationSource);
	}

	if (vm["settings"].toBool())
	{
		w.OpenSettingsDialog();
	}

	int res = a.exec();

	if (vm["debug"].toBool() && fp)
	{
		fclose(fp);
	}

	return res;
}
