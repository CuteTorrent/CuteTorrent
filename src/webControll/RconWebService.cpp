#include "RconWebService.h"
#include <QStringList>

RconWebService::RconWebService(void) : m_pSettings(QApplicationSettings::getInstance())
{
	mapper = new RequestMapper(this);
	listener = new HttpListener("WebControl", mapper, this);
	connect(m_pSettings.get(), SIGNAL(PropertyChanged(QString, QString)), SLOT(OnSettngsChnaged(QString, QString)));
}

void RconWebService::OnSettngsChnaged(QString group, QString key)
{
	if (group == "WebControl" && key == "webui_enabled")
	{
		if (m_pSettings->valueBool("WebControl", "webui_enabled", false) && !isRunning())
		{
			if (m_pSettings->valueBool("WebControl", "enable_ipfilter", false))
			{
				parseIpFilter(m_pSettings->valueString("WebControl", "ipfilter"));
			}

			Start();
		}
		else
		{
			if (isRunning())
			{
				Stop();
			}
		}
	}
}

RconWebService::~RconWebService(void)
{
	if (listener != NULL)
	{
		listener->close();
	}

	delete mapper;
}

void RconWebService::Start()
{
	if (listener != NULL)
	{
		listener->close();
		listener->Start();
	}
	else
	{
		listener = new HttpListener("WebControl", mapper, this);
		listener->Start();
	}
}

void RconWebService::parseIpFilter(QString ipFilterStr)
{
	QStringList lines = ipFilterStr.split("\n");

	foreach(QString line, lines)
		{
			if (line.trimmed().startsWith("#"))
			{
				continue;
			}

			QStringList parts = line.trimmed().split(' ');

			if (parts[0] == "allow" || parts[0] == "deny")
			{
				if (parts[0] == "allow")
				{
					if (parts[1].contains('*') && !parts[1].contains('-'))
					{
						QString pattern = parts[1];
						QString startIP = pattern.replace("*", "1").trimmed();
						QString endIP = parts[1].replace("*", "255").trimmed();
						QHostAddress start(startIP);
						QHostAddress end(endIP);

						if (start.toIPv4Address() > end.toIPv4Address())
						{
							qSwap(start, end);
						}

						allowedIP.append(QPair<uint, uint>(start.toIPv4Address(), end.toIPv4Address()));
					}
					else
					{
					}

					if (parts[1].contains('-') && !parts[1].contains('*'))
					{
						QString pattern = parts[1];
						QString startIP = pattern.replace("*", "1").trimmed();
						QString endIP = pattern.replace("*", "255").trimmed();
						QHostAddress start(startIP);
						QHostAddress end(endIP);

						if (start.toIPv4Address() > end.toIPv4Address())
						{
							qSwap(start, end);
						}

						allowedIP.append(QPair<uint, uint>(start.toIPv4Address(), end.toIPv4Address()));
					}
					else
					{
					}
				}
				else
				{
					if (parts[1] == "all")
					{
						if (listener != NULL)
						{
							listener->close();
						}
					}

					if (parts[1].contains('*') && !parts[1].contains('-'))
					{
						QString pattern = parts[1];
						QString startIP = pattern.replace("*", "1").trimmed();
						QString endIP = parts[1].replace("*", "255").trimmed();
						QHostAddress start(startIP);
						QHostAddress end(endIP);

						if (start.toIPv4Address() > end.toIPv4Address())
						{
							qSwap(start, end);
						}

						notAllowedIP.append(QPair<uint, uint>(start.toIPv4Address(), end.toIPv4Address()));
					}
					else
					{
					}

					if (parts[1].contains('-') && !parts[1].contains('*'))
					{
						QString pattern = parts[1];
						QString startIP = pattern.replace("*", "1").trimmed();
						QString endIP = parts[1].replace("*", "255").trimmed();
						QHostAddress start(startIP);
						QHostAddress end(endIP);

						if (start.toIPv4Address() > end.toIPv4Address())
						{
							qSwap(start, end);
						}

						notAllowedIP.append(QPair<uint, uint>(start.toIPv4Address(), end.toIPv4Address()));
					}
					else
					{
					}
				}
			}
			else
			{
			}
		}

	HttpConnectionHandler::allowedIP = allowedIP;
	HttpConnectionHandler::notAllowedIP = notAllowedIP;
}

bool RconWebService::isRunning()
{
	if (listener != NULL)
	{
		return listener->isListening();
	}

	return false;
}

void RconWebService::Stop()
{
	if (listener != NULL)
	{
		if (listener->isListening())
		{
			listener->close();
		}

		delete listener;
		listener = NULL;
	}
}

