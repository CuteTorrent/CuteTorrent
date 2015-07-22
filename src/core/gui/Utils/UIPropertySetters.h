#pragma once

class QWidget;
class QVariant;
#include "SettingsPropertyMapper.h"
class UIPropertySetters
{
public:
	static void DefaultSetter(QWidget* editor, SettingsPropertyMapper::WidgetType editorType, SettingsPropertyMapper::PropertyType propertyType, QVariant propertyValue);
	static void SpinboxKBSetter(QWidget* editor, SettingsPropertyMapper::WidgetType editorType, SettingsPropertyMapper::PropertyType propertyType, QVariant propertyValue);
	static void TimeFromIntSetter(QWidget* editor, SettingsPropertyMapper::WidgetType editorType, SettingsPropertyMapper::PropertyType propertyType, QVariant propertyValue);
	static void ChacheSizeSetter(QWidget* editor, SettingsPropertyMapper::WidgetType editorType, SettingsPropertyMapper::PropertyType propertyType, QVariant propertyValue);
};
