#ifndef SETTINGS_PROPERTY_MAPPER_INCLUDED
#define SETTINGS_PROPERTY_MAPPER_INCLUDED
#include <QObject>
#include <QVariant>


class SettingsPropertyMapper : public QObject
{
	Q_OBJECT
public:
	enum WidgetType
	{
		CHECKBOX,
		RADIOBUTTON,
		CHECKABLE_GROUPBOX,
		LINE_EDIT,
		TEXT_EDIT,
		COMBOBOX,
		SPINBOX,
		DOUBLE_SPINBOX,
		TIME_EDIT,
		DATETIME_EDIT
	};
	enum PropertyType
	{
		INT,
		BOOL,
		DOUBLE,
		TIME,
		DATETIME,
		STRING
	};
	typedef void(*UIValueSetter)(QWidget*, WidgetType, PropertyType, QVariant);
	typedef QVariant (*UIValueGetter)(QWidget*, WidgetType, PropertyType);
	typedef void(*ValueSetter)(QString group, QString propertyName, QVariant value);
	typedef QVariant(*ValueGetter)(QString group, QString propertyName, QVariant defaultValue);
	SettingsPropertyMapper(QObject* parent = NULL);
	~SettingsPropertyMapper();
	bool HasChanges();
	void AddMapping(QString group, QString propertyName, PropertyType propertyType, QWidget* editor, WidgetType editorType, QVariant defaultValue = QVariant(), UIValueSetter uiSetter = NULL,
	                UIValueGetter uiGetter = NULL, ValueSetter valueSetter = NULL, ValueGetter valueGetter = NULL);
	void ApplyChanges();
	void ResetToCurrentValues();
signals:
	void GotChanges();
	void NoChanges();
public slots:
	void OnEditorCommit();
private:
	struct PropertyInfo
	{
		QString group, propertyName;
		PropertyType propertyType;
		QWidget* editor;
		WidgetType editorType;
		ValueGetter valueGetter;
		ValueSetter valueSetter;
		UIValueSetter uiSetter;
		UIValueGetter uiGetter;
		QVariant defaultValue, newValue;
		bool operator==(const PropertyInfo& other);
	};

	QList<PropertyInfo> m_propertyInfos;
	bool m_hasChanges;
	void subscribeToChanges(QWidget* editor, WidgetType editorType);
	void initilizeEditorValue(PropertyInfo& propertyInfo);
	int findPropertyInfo(QWidget* editor);
	void checkForDuplicates(QString propertyGroup, QString propertyName);
};

#endif
