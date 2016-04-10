#include "StyledProgressBar.h"
#include <QStyle>
#include <QStyleOption>

void StyledProgressBar::setValue(int val)
{
	QProgressBar::setValue(val);
	style()->unpolish(this);
	style()->polish(this);
	update();
}

void StyledProgressBar::initilizeStyleOption(QStyleOptionProgressBarV2* option)
{
	initStyleOption(qstyleoption_cast<QStyleOptionProgressBar*>(option));
}

