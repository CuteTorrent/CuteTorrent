#include <QMenu>

class CheckableMenu : public QMenu
{
	Q_OBJECT
public:
	CheckableMenu(QWidget* parent);
	CheckableMenu(const QString& title, QWidget* parent);


	~CheckableMenu();

	virtual void mouseReleaseEvent(QMouseEvent* event);
};

