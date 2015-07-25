#include "SettingsPropertyMapper.h"
#include <QDebug>
#include <QCheckBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QTimeEdit>
#include "UIPropertyGetters.h"
#include "UIPropertySetters.h"
#include "ValueSetters.h"
#include "ValueGetters.h"

SettingsPropertyMapper::SettingsPropertyMapper(QObject* parent) 
	: QObject(parent)
	, m_hasChanges(false)
{
}

SettingsPropertyMapper::~SettingsPropertyMapper()
{
	m_propertyInfos.clear();
}

bool SettingsPropertyMapper::HasChanges()
{
	return m_hasChanges;
}

void SettingsPropertyMapper::subscribeToChanges(QWidget* editor, WidgetType editorType)
{
	switch (editorType)
	{
		case CHECKBOX:
		{
			QCheckBox* pCheckBox = qobject_cast<QCheckBox*>(editor);
			if (pCheckBox == NULL)
			{
				qCritical() << "Invalid editor given";
				return;
			}
			connect(pCheckBox, SIGNAL(stateChanged(int)), SLOT(OnEditorCommit()));
			break;
		}
		case RADIOBUTTON:
		{
			QRadioButton* pRadioButton = qobject_cast<QRadioButton*>(editor);
			if (pRadioButton == NULL)
			{
				qCritical() << "Invalid editor given";
				return;
			}
			connect(pRadioButton, SIGNAL(toggled(bool)), SLOT(OnEditorCommit()));
			break;
		}
		case CHECKABLE_GROUPBOX:
		{
			QGroupBox* pGroupBox = qobject_cast<QGroupBox*>(editor);
			if (pGroupBox == NULL)
			{
				qCritical() << "Invalid editor given";
				return;
			}
			if (!pGroupBox->isCheckable())
			{
				qCritical() << "Given QGroupBox is not checkable";
			}
			connect(pGroupBox, SIGNAL(toggled(bool)), SLOT(OnEditorCommit()));
			break;
		}
		case LINE_EDIT:
		{
			QLineEdit* pLineEdit = qobject_cast<QLineEdit*>(editor);
			if (pLineEdit == NULL)
			{
				qCritical() << "Invalid editor given";
				return;
			}
			connect(pLineEdit, SIGNAL(textChanged(QString)), SLOT(OnEditorCommit()));
			break;
		}
		case TEXT_EDIT:
		{
			QTextEdit* pTextEdit = qobject_cast<QTextEdit*>(editor);
			if (pTextEdit == NULL)
			{
				qCritical() << "Invalid editor given";
				return;
			}
			connect(pTextEdit, SIGNAL(textChanged()), SLOT(OnEditorCommit()));
			break;
		}
		case COMBOBOX:
		{
			QComboBox* pComboBox = qobject_cast<QComboBox*>(editor);
			if (pComboBox == NULL)
			{
				qCritical() << "Invalid editor given";
				return;
			}
			connect(pComboBox, SIGNAL(currentIndexChanged(int)), SLOT(OnEditorCommit()));
			break;
		}
		case SPINBOX:
		{
			QSpinBox* pSpinBox = qobject_cast<QSpinBox*>(editor);
			if (pSpinBox == NULL)
			{
				qCritical() << "Invalid editor given";
				return;
			}
			connect(pSpinBox, SIGNAL(valueChanged(int)), SLOT(OnEditorCommit()));
			break;
		}
		case DOUBLE_SPINBOX:
		{
			QDoubleSpinBox* pDoubleSpinBox = qobject_cast<QDoubleSpinBox*>(editor);
			if (pDoubleSpinBox == NULL)
			{
				qCritical() << "Invalid editor given";
				return;
			}
			connect(pDoubleSpinBox, SIGNAL(valueChanged(double)), SLOT(OnEditorCommit()));
			break;
		}
		case TIME_EDIT:
		{
			QTimeEdit* pTimeEdit = qobject_cast<QTimeEdit*>(editor);
			if (pTimeEdit == NULL)
			{
				qCritical() << "Invalid editor given";
				return;
			}
			connect(pTimeEdit, SIGNAL(timeChanged(const QTime&)), SLOT(OnEditorCommit()));
			break;
		}
		case DATETIME_EDIT:
		{
			QDateTimeEdit* pDateTimeEdit = qobject_cast<QDateTimeEdit*>(editor);
			if (pDateTimeEdit == NULL)
			{
				qCritical() << "Invalid editor given";
				return;
			}
			connect(pDateTimeEdit, SIGNAL(dateTimeChanged(const QDateTime&)), SLOT(OnEditorCommit()));
			break;
		}
		default: break;
	}
}


void SettingsPropertyMapper::initilizeEditorValue(PropertyInfo& propertyInfo)
{
	QVariant propertyValue = propertyInfo.valueGetter(propertyInfo.group, propertyInfo.propertyName, propertyInfo.defaultValue);
	if (propertyInfo.uiSetter != NULL)
	{
		propertyInfo.uiSetter(propertyInfo.editor, propertyInfo.editorType, propertyInfo.propertyType, propertyValue);
	}
	else
	{
		qCritical() << "Setter not set for " << propertyInfo.group << propertyInfo.propertyName;
	}
}

int SettingsPropertyMapper::findPropertyInfo(QWidget* editor)
{
	for (int i = 0; i < m_propertyInfos.size(); i++)
	{
		PropertyInfo& propertyInfo = m_propertyInfos[i];
		if (propertyInfo.editor == editor)
		{
			return i;
		}
	}
	return -1;
}

void SettingsPropertyMapper::checkForDuplicates(QString propertyGroup, QString propertyName)
{
	QList<PropertyInfo> properties2Remove;
	for (int i = 0; i < m_propertyInfos.size(); i++)
	{
		PropertyInfo& propertyInfo = m_propertyInfos[i];
		if (propertyInfo.propertyName == propertyName && propertyInfo.group == propertyGroup)
		{
			qDebug() << "Find duplicate for property" << propertyGroup << propertyName;
			properties2Remove.append(propertyInfo);
		}
	}

	for (int i = 0; i < properties2Remove.size(); i++)
	{
		m_propertyInfos.removeAll(properties2Remove[i]);
	}
}

void SettingsPropertyMapper::AddMapping(QString propertyGroup, QString propertyName, PropertyType propertyType, QWidget* editor, WidgetType editorType, QVariant defaultValue, UIValueSetter uiSetter, UIValueGetter uiGetter, ValueSetter valueSetter, ValueGetter valueGetter)
{
	PropertyInfo propertyInfo =
	{
		propertyGroup,
		propertyName,
		propertyType,
		editor,
		editorType,
		(valueGetter == NULL ? ValueGetters::SettingsValueGetter : valueGetter),
		(valueSetter == NULL ? ValueSetters::SettingsValueSetter : valueSetter),
		(uiSetter == NULL ? UIPropertySetters::DefaultSetter : uiSetter),
		(uiGetter == NULL ? UIPropertyGetters::DefaultGetter : uiGetter),
		defaultValue
	};
	checkForDuplicates(propertyGroup, propertyName);
	m_propertyInfos.append(propertyInfo);
	initilizeEditorValue(propertyInfo);
	subscribeToChanges(editor, editorType);
}

void SettingsPropertyMapper::ApplyChanges()
{
	
	for (int i = 0; i < m_propertyInfos.size(); i++)
	{
		PropertyInfo& propertyInfo = m_propertyInfos[i];
		if (propertyInfo.newValue.isValid())
		{
			propertyInfo.valueSetter(propertyInfo.group, propertyInfo.propertyName, propertyInfo.newValue);
			propertyInfo.newValue = QVariant();
		}
	}
	m_hasChanges = false;
	emit NoChanges();
}

void SettingsPropertyMapper::ResetToCurrentValues()
{
	for (int i = 0; i < m_propertyInfos.size(); i++)
	{
		PropertyInfo propertyInfo = m_propertyInfos[i];
		QVariant propertyValue = propertyInfo.valueGetter(propertyInfo.group, propertyInfo.propertyName, propertyInfo.defaultValue);
		if (propertyInfo.uiSetter != NULL)
		{
			propertyInfo.uiSetter(propertyInfo.editor, propertyInfo.editorType, propertyInfo.propertyType, propertyValue);
		}
		else
		{
			qCritical() << "Setter not set for " << propertyInfo.group << propertyInfo.propertyName;
		}
	}
}

void SettingsPropertyMapper::OnEditorCommit()
{
	QWidget* editor = qobject_cast<QWidget*>(sender());
	int propertyInfoIndex = findPropertyInfo(editor);
	
	if (propertyInfoIndex > -1)
	{
		PropertyInfo& propertyInfo = m_propertyInfos[propertyInfoIndex];
		QVariant originalValue = propertyInfo.valueGetter(propertyInfo.group, propertyInfo.propertyName, propertyInfo.defaultValue);
		QVariant newValue = propertyInfo.uiGetter(propertyInfo.editor, propertyInfo.editorType, propertyInfo.propertyType);
		if (originalValue != newValue)
		{
			propertyInfo.newValue = newValue;
			if (!m_hasChanges)
			{
				emit GotChanges();
				m_hasChanges = true;
			}
		}
		else
		{
			propertyInfo.newValue = QVariant();
			bool hadChnages = m_hasChanges;
			m_hasChanges = false;
			for (int i = 0; i < m_propertyInfos.size(); i++)
			{
				if (m_propertyInfos[i].newValue.isValid())
				{
					m_hasChanges = true;
					break;
				}
			}
			if (hadChnages && !m_hasChanges)
			{
				emit NoChanges();
			}
		}
	}
	else
	{
		qCritical() << "Recived signal from unregisted editor";
	}
}

bool SettingsPropertyMapper::PropertyInfo::operator==(const PropertyInfo& other)
{
	return group == other.group
		&& propertyName == other.propertyName
		&& propertyType == other.propertyType
		&& editor == other.editor
		&& editorType == other.editorType
		&& uiGetter == other.uiGetter
		&& uiSetter == other.uiSetter
		&& valueGetter == other.valueGetter
		&& valueSetter == other.valueSetter
		&& defaultValue == other.defaultValue;
}