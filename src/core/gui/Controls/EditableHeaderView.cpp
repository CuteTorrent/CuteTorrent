#include "EditableHeaderView.h"
#include <QSignalMapper>
#include <QMenu>
#include <QContextMenuEvent>
#include <QDebug>

void EditableHeaderView::contextMenuEvent(QContextMenuEvent* e)
{
	QMenu menu;
	QSignalMapper mapper;
	QAbstractItemModel* m = model();

	for (int col = 0; col < model()->columnCount(); ++col)
	{
		QAction* action = new QAction(&menu);
		action->setText(model()->headerData(col, Qt::Horizontal).toString());
		action->setCheckable(true);
		action->setChecked(!isSectionHidden(col));
		connect(action, SIGNAL(triggered()), &mapper, SLOT(map()));
		mapper.setMapping(action, col);
		menu.addAction(action);
	}

	connect(&mapper, SIGNAL(mapped(int)),
	        this, SLOT(toggleSectionVisibility(int)));
	menu.exec(e->globalPos());
	e->accept();
}

EditableHeaderView::EditableHeaderView(Qt::Orientation orientation, QWidget* parent): QHeaderView(orientation, parent)
{
}

EditableHeaderView::EditableHeaderView(QHeaderViewPrivate& dd, Qt::Orientation orientation, QWidget* parent): QHeaderView(dd, orientation, parent)
{
}

void EditableHeaderView::toggleSectionVisibility(int section)
{
	setSectionHidden(section, !isSectionHidden(section));
}

