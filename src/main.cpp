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

#define DEBUG
#include "CuteTorrent.h"
#include <QDir>
#include <qtsingleapplication.h>
#include <cstdio>
#include <QStringList>
#include <QDebug>
#include <QDateTime>
#include "application.h"
#include <QTextCodec>
#include "CuteTorentStyle.h"
#include  "StyleEngene.h"
#ifdef DEBUG

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

#endif // DEBUG



int main(int argc, char* argv[])
{
	Application a(argc, argv);
	a.setWindowIcon(QIcon(":/icons/app.ico"));
	bool minimize = false, debug = false;
	QString file2open;
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	qsrand(time(NULL));

	if(a.isRunning())
	{
		if(argc >= 2)
			for(int i = 1; i < argc; i++)
			{
				if(argv[i][0] != '-')
				{
					a.sendMessage(QString::fromLocal8Bit(argv[i]));
				}
			}

		return -1;
	}
	else
	{
		if(argc >= 2)
		{
			for(int i = 1; i < argc; i++)
			{
				if(argv[i][0] == '-')
				{
					if(!strcmp(argv[i], "-m"))
					{
						minimize = true;
					}
					else if(!strcmp(argv[i], "-debug"))
					{
						debug = true;
					}
				}
				else
				{
					file2open = QString::fromLocal8Bit(argv[i]);
				}
			}
		}
	}

	QString dataDir;
#ifdef Q_WS_MAC
	dataDir = "/Library/CuteTorrent/";
#else
	dataDir = QApplication::applicationDirPath() + QDir::separator();
#endif
	FILE* fp;

	if(debug)
	{
		QString logFileName = dataDir + "ct_debug.log";
		fp = freopen(logFileName.toAscii().data(), "a+", stdout);
		qInstallMsgHandler(myMessageOutput);
	}

	a.loadTranslations(":/translations");
	a.loadTranslationsQt(":/translations_qt");
	a.addLibraryPath(QCoreApplication::applicationDirPath() + "/plugins");
	CuteTorrent w;
	a.setActivationWindow(&w);
	a.setActiveWindow(&w);
	w.ConnectMessageReceved(&a);

	if(minimize)
	{
		w.showMinimized();
	}
	else
	{
		w.showNormal();
	}

	if(!file2open.isEmpty())
	{
		w.HandleNewTorrent(file2open);
	}

	int res = a.exec();

	if(debug && fp)
	{
		fclose(fp);
	}

	_CrtDumpMemoryLeaks();
	return res;
}
