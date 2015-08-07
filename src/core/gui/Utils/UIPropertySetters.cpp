#include "UIPropertySetters.h"
#include <QDateTimeEdit>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QTextEdit>
#include <QGroupBox>
#include <QLineEdit>
#include <QRadioButton>
#include <QCheckBox>
#include <QDebug>
#include "defs.h"
#include "StaticHelpers.h"

void UIPropertySetters::DefaultSetter(QWidget* editor, SettingsPropertyMapper::WidgetType editorType, SettingsPropertyMapper::PropertyType propertyType, QVariant propertyValue)
{
	switch (editorType)
	{
		case SettingsPropertyMapper::CHECKBOX:
		{
			QCheckBox* pCheckBox = qobject_cast<QCheckBox*>(editor);

			if (pCheckBox == NULL)
			{
				qCritical() << "Invalid editor given";
				return;
			}

			if (propertyType != SettingsPropertyMapper::BOOL)
			{
				qCritical() << "Unable to set value with default setter for property using value" << propertyValue;
				return;
			}

			pCheckBox->setChecked(propertyValue.toBool());
			break;
		}

		case SettingsPropertyMapper::RADIOBUTTON:
		{
			QRadioButton* pRadioButton = qobject_cast<QRadioButton*>(editor);

			if (pRadioButton == NULL)
			{
				qCritical() << "Invalid editor given";
				return;
			}

			if (propertyType != SettingsPropertyMapper::BOOL)
			{
				qCritical() << "Unable to set value with default setter for property using value" << propertyValue;
				return;
			}

			pRadioButton->setChecked(propertyValue.toBool());
			break;
		}

		case SettingsPropertyMapper::CHECKABLE_GROUPBOX:
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

			if (propertyType != SettingsPropertyMapper::BOOL)
			{
				qCritical() << "Unable to set value with default setter for property using value" << propertyValue;
				return;
			}

			pGroupBox->setChecked(propertyValue.toBool());
			break;
		}

		case SettingsPropertyMapper::LINE_EDIT:
		{
			QLineEdit* pLineEdit = qobject_cast<QLineEdit*>(editor);

			if (pLineEdit == NULL)
			{
				qCritical() << "Invalid editor given";
				return;
			}

			if (propertyType == SettingsPropertyMapper::STRING || propertyType == SettingsPropertyMapper::INT)
			{
				pLineEdit->setText(propertyValue.toString());
			}
			else
			{
				qCritical() << "Unable to set value with default setter for property using value" << propertyValue;
			}

			break;
		}

		case SettingsPropertyMapper::TEXT_EDIT:
		{
			QTextEdit* pTextEdit = qobject_cast<QTextEdit*>(editor);

			if (pTextEdit == NULL)
			{
				qCritical() << "Invalid editor given";
				return;
			}

			if (propertyType != SettingsPropertyMapper::STRING)
			{
				qCritical() << "Unable to set value with default setter for property using value" << propertyValue;
				return;
			}

			pTextEdit->setText(propertyValue.toString());
			break;
		}

		case SettingsPropertyMapper::COMBOBOX:
		{
			QComboBox* pComboBox = qobject_cast<QComboBox*>(editor);

			if (pComboBox == NULL)
			{
				qCritical() << "Invalid editor given";
				return;
			}

			if (propertyType != SettingsPropertyMapper::INT)
			{
				qCritical() << "Unable to set value with default setter for property using value" << propertyValue;
				return;
			}

			pComboBox->setCurrentIndex(propertyValue.toInt());
			break;
		}

		case SettingsPropertyMapper::SPINBOX:
		{
			QSpinBox* pSpinBox = qobject_cast<QSpinBox*>(editor);

			if (pSpinBox == NULL)
			{
				qCritical() << "Invalid editor given";
				return;
			}

			if (propertyType != SettingsPropertyMapper::INT)
			{
				qCritical() << "Unable to set value with default setter for property using value" << propertyValue;
				return;
			}

			pSpinBox->setValue(propertyValue.toInt());
			break;
		}

		case SettingsPropertyMapper::DOUBLE_SPINBOX:
		{
			QDoubleSpinBox* pDoubleSpinBox = qobject_cast<QDoubleSpinBox*>(editor);

			if (pDoubleSpinBox == NULL)
			{
				qCritical() << "Invalid editor given";
				return;
			}

			if (propertyType != SettingsPropertyMapper::DOUBLE)
			{
				qCritical() << "Unable to set value with default setter for property using value" << propertyValue;
				return;
			}

			pDoubleSpinBox->setValue(propertyValue.toDouble());
			break;
		}

		case SettingsPropertyMapper::TIME_EDIT:
		{
			QTimeEdit* pTimeEdit = qobject_cast<QTimeEdit*>(editor);

			if (pTimeEdit == NULL)
			{
				qCritical() << "Invalid editor given";
				return;
			}

			if (propertyType != SettingsPropertyMapper::TIME)
			{
				qCritical() << "Unable to set value with default setter for property using value" << propertyValue;
				return;
			}

			pTimeEdit->setTime(propertyValue.toTime());
			break;
		}

		case SettingsPropertyMapper::DATETIME_EDIT:
		{
			QDateTimeEdit* pDateTimeEdit = qobject_cast<QDateTimeEdit*>(editor);

			if (pDateTimeEdit == NULL)
			{
				qCritical() << "Invalid editor given";
				return;
			}

			if (propertyType != SettingsPropertyMapper::DATETIME)
			{
				qCritical() << "Unable to set value with default setter for property using value" << propertyValue;
				return;
			}

			pDateTimeEdit->setDateTime(propertyValue.toDateTime());
			break;
		}

		default:
			break;
	}
}

void UIPropertySetters::SpinboxKBSetter(QWidget* editor, SettingsPropertyMapper::WidgetType editorType, SettingsPropertyMapper::PropertyType propertyType, QVariant propertyValue)
{
	if (editorType == SettingsPropertyMapper::SPINBOX)
	{
		QSpinBox* pSpinBox = qobject_cast<QSpinBox*>(editor);

		if (pSpinBox == NULL)
		{
			qCritical() << "SpinboxKBSetter support only QSpinBox. Unable to cast to QSpinBox.";
			return;
		}

		int val = propertyValue.toInt();
		pSpinBox->setValue(val / KbInt);
	}
	else
	{
		qCritical() << "SpinboxKBSetter support only QSpinBox";
	}
}

void UIPropertySetters::TimeFromIntSetter(QWidget* editor, SettingsPropertyMapper::WidgetType editorType, SettingsPropertyMapper::PropertyType propertyType, QVariant propertyValue)
{
	if (editorType == SettingsPropertyMapper::TIME_EDIT)
	{
		QTimeEdit* pTimeEdit = qobject_cast<QTimeEdit*>(editor);

		if (pTimeEdit == NULL)
		{
			qCritical() << "TimeFromIntSetter support only QTimeEdit. Unable to cast to QTimeEdit.";
			return;
		}

		int val = propertyValue.toInt();
		pTimeEdit->setTime(StaticHelpers::SecsToQTime(val));
	}
	else
	{
		qCritical() << "SpinboxKBSetter support only QSpinBox";
	}
}

void UIPropertySetters::ChacheSizeSetter(QWidget* editor, SettingsPropertyMapper::WidgetType editorType, SettingsPropertyMapper::PropertyType propertyType, QVariant propertyValue)
{
	if (editorType == SettingsPropertyMapper::SPINBOX)
	{
		QSpinBox* pSpinBox = qobject_cast<QSpinBox*>(editor);

		if (pSpinBox == NULL)
		{
			qCritical() << "SpinboxKBSetter support only ChacheSizeSetter. Unable to cast to ChacheSizeSetter.";
			return;
		}

		int val = propertyValue.toInt();
		pSpinBox->setValue(val * 16);
	}
	else
	{
		qCritical() << "SpinboxKBSetter support only ChacheSizeSetter";
	}
}