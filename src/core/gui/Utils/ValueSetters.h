#pragma once

#include <QVariant>

class ValueSetters
{
public:
	static void SettingsValueSetter(QString group, QString name, QVariant value);
	static void StyleValueSetter(QString group, QString name, QVariant value);
	static void LanguageValueSetter(QString group, QString name, QVariant value);
	static void MagnetAssociationValueSetter(QString group, QString name, QVariant value);
	static void TorrentAssociationValueSetter(QString group, QString name, QVariant value);
	static void RunOnBootValueSetter(QString group, QString name, QVariant value);
#ifdef Q_WS_WIN
	static void RunOnBootMinimizedValueSetter(QString group, QString name, QVariant value);
	static void WindowsShellValueSetter(QString group, QString name, QVariant value);
#endif
};
