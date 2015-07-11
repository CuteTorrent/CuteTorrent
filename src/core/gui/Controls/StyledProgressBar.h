#pragma once
#include <QProgressBar>
class QStyleOptionProgressBarV2;

class StyledProgressBar : public QProgressBar
{


public:
	StyledProgressBar(QWidget* parent = 0) : QProgressBar(parent)
	{
	}
	void setValue(int val);
	void initilizeStyleOption(QStyleOptionProgressBarV2* option);
};
