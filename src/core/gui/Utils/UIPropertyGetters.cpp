#include "UIPropertyGetters.h"
#include <QDateTimeEdit>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QTextEdit>
#include <QGroupBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QDebug>
#include <QLineEdit>
#include "defs.h"
#include <helpers/StaticHelpers.h>

QVariant UIPropertyGetters::DefaultGetter(QWidget* editor, SettingsPropertyMapper::WidgetType editorType, SettingsPropertyMapper::PropertyType)
{
	switch (editorType)
	{
		case SettingsPropertyMapper::CHECKBOX:
		{
			QCheckBox* pCheckBox = qobject_cast<QCheckBox*>(editor);

			if (pCheckBox == NULL)
			{
				qCritical() << "Invalid editor given";
				return QVariant();
			}

			return QVariant::fromValue(pCheckBox->isChecked());
		}

		case SettingsPropertyMapper::RADIOBUTTON:
		{
			QRadioButton* pRadioButton = qobject_cast<QRadioButton*>(editor);

			if (pRadioButton == NULL)
			{
				qCritical() << "Invalid editor given";
				return QVariant();
			}

			return QVariant::fromValue(pRadioButton->isChecked());
		}

		case SettingsPropertyMapper::CHECKABLE_GROUPBOX:
		{
			QGroupBox* pGroupBox = qobject_cast<QGroupBox*>(editor);

			if (pGroupBox == NULL)
			{
				qCritical() << "Invalid editor given";
				return QVariant();
			}

			if (!pGroupBox->isCheckable())
			{
				qCritical() << "Given QGroupBox is not checkable";
			}

			return QVariant::fromValue(pGroupBox->isChecked());
		}

		case SettingsPropertyMapper::LINE_EDIT:
		{
			QLineEdit* pLineEdit = qobject_cast<QLineEdit*>(editor);

			if (pLineEdit == NULL)
			{
				qCritical() << "Invalid editor given";
				return QVariant();
			}

			return QVariant::fromValue(pLineEdit->text());
		}

		case SettingsPropertyMapper::TEXT_EDIT:
		{
			QTextEdit* pTextEdit = qobject_cast<QTextEdit*>(editor);

			if (pTextEdit == NULL)
			{
				qCritical() << "Invalid editor given";
				return QVariant();
			}

			return QVariant::fromValue(pTextEdit->toPlainText());
		}

		case SettingsPropertyMapper::COMBOBOX:
		{
			QComboBox* pComboBox = qobject_cast<QComboBox*>(editor);

			if (pComboBox == NULL)
			{
				qCritical() << "Invalid editor given";
				return QVariant();
			}

			return QVariant::fromValue(pComboBox->currentIndex());
		}

		case SettingsPropertyMapper::SPINBOX:
		{
			QSpinBox* pSpinBox = qobject_cast<QSpinBox*>(editor);

			if (pSpinBox == NULL)
			{
				qCritical() << "Invalid editor given";
				return QVariant();
			}

			return QVariant::fromValue(pSpinBox->value());
		}

		case SettingsPropertyMapper::DOUBLE_SPINBOX:
		{
			QDoubleSpinBox* pDoubleSpinBox = qobject_cast<QDoubleSpinBox*>(editor);

			if (pDoubleSpinBox == NULL)
			{
				qCritical() << "Invalid editor given";
				return QVariant();
			}

			return QVariant::fromValue(pDoubleSpinBox->value());
		}

		case SettingsPropertyMapper::TIME_EDIT:
		{
			QTimeEdit* pTimeEdit = qobject_cast<QTimeEdit*>(editor);

			if (pTimeEdit == NULL)
			{
				qCritical() << "Invalid editor given";
				return QVariant();
			}

			return QVariant::fromValue(pTimeEdit->time());
		}

		case SettingsPropertyMapper::DATETIME_EDIT:
		{
			QDateTimeEdit* pDateTimeEdit = qobject_cast<QDateTimeEdit*>(editor);

			if (pDateTimeEdit == NULL)
			{
				qCritical() << "Invalid editor given";
				return QVariant();
			}

			return QVariant::fromValue(pDateTimeEdit->dateTime());
		}

		default:
		{
			return QVariant();
		}
	}
}

QVariant UIPropertyGetters::SpinboxKBGetter(QWidget* editor, SettingsPropertyMapper::WidgetType editorType, SettingsPropertyMapper::PropertyType)
{
	if (editorType == SettingsPropertyMapper::SPINBOX)
	{
		QSpinBox* pSpinBox = qobject_cast<QSpinBox*>(editor);

		if (pSpinBox == NULL)
		{
			qCritical() << "SpinboxKBGetter support only QSpinBox. Unable to cast to QSpinBox.";
			return QVariant();
		}

		return pSpinBox->value() * KbInt;
	}

	qCritical() << "SpinboxKBSetter support only QSpinBox";
	return QVariant();
}

QVariant UIPropertyGetters::IntFromTimeGetter(QWidget* editor, SettingsPropertyMapper::WidgetType editorType, SettingsPropertyMapper::PropertyType)
{
	if (editorType == SettingsPropertyMapper::TIME_EDIT)
	{
		QTimeEdit* pTimeEdit = qobject_cast<QTimeEdit*>(editor);

		if (pTimeEdit == NULL)
		{
			qCritical() << "IntFromTimeGetter support only QTimeEdit. Unable to cast to QTimeEdit.";
			return QVariant();
		}

		return StaticHelpers::QTimeToSecs(pTimeEdit->time());
	}

	qCritical() << "IntFromTimeGetter support only QTimeEdit";
	return QVariant();
}

QVariant UIPropertyGetters::CacheSizeGetter(QWidget* editor, SettingsPropertyMapper::WidgetType editorType, SettingsPropertyMapper::PropertyType)
{
	if (editorType == SettingsPropertyMapper::SPINBOX)
	{
		QSpinBox* pSpinBox = qobject_cast<QSpinBox*>(editor);

		if (pSpinBox == NULL)
		{
			qCritical() << "SpinboxKBGetter support only CacheSizeGetter. Unable to cast to CacheSizeGetter.";
			return QVariant();
		}

		return pSpinBox->value() / 16;
	}

	qCritical() << "SpinboxKBSetter support only CacheSizeGetter";
	return QVariant();
}

