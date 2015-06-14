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
#include <QDir>
#include <qtsingleapplication.h>
#include <cstdio>
#include <QStringList>
#include <QDebug>
#include <QDateTime>
#include "application.h"
#include <QTextCodec>
#include  "StyleEngene.h"
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
namespace po = boost::program_options;
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

#ifdef Q_WS_WIN
#include <windows.h> 
#include <io.h> 
#include <fcntl.h> 
#include <stdio.h>
BOOL attachOutputToConsole(void)
{
	HANDLE consoleHandleOut, consoleHandleError;
	int fdOut, fdError;
	FILE *fpOut, *fpError;
	if (AttachConsole(ATTACH_PARENT_PROCESS))
	{
		//redirect unbuffered STDOUT to the console 
		consoleHandleOut = GetStdHandle(STD_OUTPUT_HANDLE);
		fdOut = _open_osfhandle((intptr_t)consoleHandleOut, _O_TEXT);
		fpOut = _fdopen(fdOut, "w");
		*stdout = *fpOut;
		setvbuf(stdout, NULL, _IONBF, 0);
		//redirect unbuffered STDERR to the console 
		consoleHandleError = GetStdHandle(STD_ERROR_HANDLE);
		fdError = _open_osfhandle((intptr_t)consoleHandleError, _O_TEXT);
		fpError = _fdopen(fdError, "w");
		*stderr = *fpError;
		setvbuf(stderr, NULL, _IONBF, 0);
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		GetConsoleScreenBufferInfo(consoleHandleOut, &consoleInfo);
		int preintedTextLen = consoleInfo.dwCursorPosition.X;
		consoleInfo.dwCursorPosition.X = 0;
		SetConsoleCursorPosition(consoleHandleOut, consoleInfo.dwCursorPosition);
		for (int i = 0; i < preintedTextLen; i++)
		{
			printf(" ");
		}
		SetConsoleCursorPosition(consoleHandleOut, consoleInfo.dwCursorPosition);
		return TRUE;
	}
	//Not a console application 
	return FALSE;
}
void sendEnterKey(void) {
	INPUT ip; 
	// Set up a generic keyboard event. 
	ip.type = INPUT_KEYBOARD; 
	ip.ki.wScan = 0; 
	// hardware scan code for key 
	ip.ki.time = 0; 
	ip.ki.dwExtraInfo = 0;  
	//Send the "Enter" key 
	ip.ki.wVk = 0x0D; 
	// virtual-key code for the "Enter" key 
	ip.ki.dwFlags = 0; 
	// 0 for key press 
	SendInput(1, &ip, sizeof(INPUT));  
	// Release the "Enter" key 
	ip.ki.dwFlags = KEYEVENTF_KEYUP; 
	// KEYEVENTF_KEYUP for key release 
	SendInput(1, &ip, sizeof(INPUT)); }
#endif
int main(int argc, char* argv[])
{
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
	catch (po::error ex)
	{
		std::cout << "Error parsing cmd line: " << ex.what() << std::endl;
#ifdef Q_WS_WIN
		if (console)
		{
			sendEnterKey();
		}
#endif
		return 0;
	}

	Application a(argc, argv);
	a.setWindowIcon(QIcon(":/icons/app.ico"));
	QString file2open = QString::fromUtf8(vm["input-file"].as<std::string>().c_str());
	

	if(a.isRunning())
	{
		if (!file2open.isEmpty())
		{
			a.sendMessage(QString("open:%1").arg(file2open));
		}
		if (vm.count("create_torrent"))
		{
			QString torrentCreationSource = QString::fromUtf8(vm["create_torrent"].as<std::string>().c_str());
			a.sendMessage(QString("create_torrent:%1").arg(torrentCreationSource));
		}
		if (vm.count("settings"))
		{
			a.sendMessage(QString("settings:"));
		}
		return 0;
	}
	else
	{
		if (vm.count("help"))
		{
			std::cout << desc << std::endl;
#ifdef Q_WS_WIN
			if (console)
			{
				sendEnterKey();
			}
#endif
			return 0;
		}

	}

	QString dataDir;
#ifdef Q_WS_MAC
	dataDir = "/Library/CuteTorrent/";
#else
	dataDir = QApplication::applicationDirPath() + QDir::separator();
#endif
	FILE* fp = nullptr;

	if (vm.count("debug"))
	{
		QString logFileName = dataDir + "ct_debug.log";
		fp = freopen(logFileName.toAscii().data(), "a+", stdout);
		qInstallMsgHandler(myMessageOutput);
	}
	a.loadTranslations(":/translations");
	a.loadTranslationsQt(":/translations_qt");
	a.addLibraryPath(QCoreApplication::applicationDirPath() + "/plugins");
	CuteTorrentMainWindow w;
	a.setActivationWindow(&w);
	a.setActiveWindow(&w);
	w.ConnectMessageReceved(&a);

	if (vm.count("minimize"))
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

	if (vm.count("create_torrent"))
	{
		QString torrentCreationSource = QString::fromUtf8(vm["create_torrent"].as<std::string>().c_str());
		w.ShowCreateTorrentDialog(torrentCreationSource);
	}
	if (vm.count("settings"))
	{
		w.OpenSettingsDialog();
	}
	int res = a.exec();
	qDebug() << "Launch from console = " << console;
	if (vm.count("debug") && fp)
	{
		fclose(fp);
	}

//	_CrtDumpMemoryLeaks();
#ifdef Q_WS_WIN
	if (console)
	{
		sendEnterKey();
	}
#endif
	return res;
}
