#include "CheckableMenu.h"


CheckableMenu::CheckableMenu(QWidget* parent) : QMenu(parent)
{
}

CheckableMenu::CheckableMenu(const QString& title, QWidget* parent)
	: QMenu(title, parent)
{
}

CheckableMenu::~CheckableMenu()
{
}

void CheckableMenu::mouseReleaseEvent(QMouseEvent* event)
{
	QAction* act = menuAction();
	if (act)
	{
		QMenu* men = act->menu();
		act->setMenu(0);

		QMenu::mouseReleaseEvent(event);
		act->setMenu(men);
	}
	else
	{
		QMenu::mouseReleaseEvent(event);
	}
}

