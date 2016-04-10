#pragma once
#include <QVariant>
#include "SettingsPropertyMapper.h"

class UIPropertyGetters
{
public:
	static QVariant DefaultGetter(QWidget* editor, SettingsPropertyMapper::WidgetType editorType, SettingsPropertyMapper::PropertyType);
	static QVariant SpinboxKBGetter(QWidget* editor, SettingsPropertyMapper::WidgetType editorType, SettingsPropertyMapper::PropertyType);
	static QVariant IntFromTimeGetter(QWidget* editor, SettingsPropertyMapper::WidgetType editorType, SettingsPropertyMapper::PropertyType);
	static QVariant CacheSizeGetter(QWidget* editor, SettingsPropertyMapper::WidgetType editorType, SettingsPropertyMapper::PropertyType);
};

