#ifndef SPEED_LIMIT_DIALOG_H
#define SPEED_LIMIT_DIALOG_H
#include "CustomWindow.h"
#include "ui_SpeedLimitDialog.h"
class SpeedLimitDialog : public BaseWindow<QDialog>, Ui::SpeedLimitDialog
{
	Q_OBJECT
public:
	enum DialogMode
	{
		UPLOAD,
		DOWNLOAD
	};
	explicit SpeedLimitDialog(DialogMode dialogMode, QWidget* parent);
	int SpeedValue();
private slots:
	void OnSpeedChanged(int value);
private:
	bool m_chengingSpeed;
	int m_speedValue;
	
};
#endif// SPEED_LIMIT_DIALOG_H
