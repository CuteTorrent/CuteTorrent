#pragma once
#include <QVariant>

class ValueGetters
{
public:
	static QVariant SettingsValueGetter(QString group, QString name, QVariant defaultValue = QVariant());
	static QVariant SettingsEncryptedValueGetter(QString group, QString name, QVariant defaultValue = QVariant());
	static QVariant StyleValueGetter(QString group, QString name, QVariant defaultValue = QVariant());
	static QVariant LanguageValueGetter(QString group, QString name, QVariant defaultValue = QVariant());
	static QVariant MagnetAssociationValueGetter(QString group, QString name, QVariant defaultValue = QVariant());
	static QVariant TorrentAssociationValueGetter(QString group, QString name, QVariant defaultValue = QVariant());
	static QVariant RunOnBootValueGetter(QString group, QString name, QVariant defaultValue = QVariant());
#ifdef Q_WS_WIN
	static QVariant RunOnBootMinimizedValueGetter(QString group, QString name, QVariant defaultValue = QVariant());
	static QVariant WindowsShellValueGetter(QString group, QString name, QVariant defaultValue = QVariant());
#endif
};
