#include "SpeedLimitDialog.h"
#include <QDialog>

SpeedLimitDialog::SpeedLimitDialog(DialogMode dialogMode, QWidget* parent)
	: BaseWindow<QDialog>(OnlyCloseButton, NoResize, parent)
	  , m_chengingSpeed(false)
{
	setupUi(this);
	setupCustomWindow();
	setupWindowIcons();
	switch (dialogMode)
	{
		case UPLOAD:
		{
			setTitle(tr("UPLOAD_LIMIT"));
			label->setText(tr("UPLOAD_LIMIT"));
			break;
		}
		case DOWNLOAD:
		{
			setTitle(tr("DOWNLOAD_LIMIT"));
			label->setText(tr("DOWNLOAD_LIMIT"));
			break;
		}
	}
}

int SpeedLimitDialog::SpeedValue()
{
	return m_speedValue;
}

void SpeedLimitDialog::OnSpeedChanged(int value)
{
	if (m_chengingSpeed)
		return;

	m_chengingSpeed = true;
	if (sender() == spinBox)
	{
		horizontalSlider->setValue(value);
	}
	else if (sender() == horizontalSlider)
	{
		spinBox->setValue(value);
	}
	m_chengingSpeed = false;
}

