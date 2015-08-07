#ifndef EditableHeaderView_INCLUDED
#define EditableHeaderView_INCLUDED
#include <QHeaderView>

class EditableHeaderView : public QHeaderView
{
	Q_OBJECT
protected:
	void contextMenuEvent(QContextMenuEvent*) override;
public:
	EditableHeaderView(Qt::Orientation orientation, QWidget* parent);
	EditableHeaderView(QHeaderViewPrivate& dd, Qt::Orientation orientation, QWidget* parent);
private slots:
	void toggleSectionVisibility(int section);
};

#endif