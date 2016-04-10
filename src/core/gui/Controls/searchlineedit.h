#ifndef SEARCHLINEEDIT_H
#define SEARCHLINEEDIT_H

#include <QLineEdit>

class QToolButton;

class SearchLineEdit : public QLineEdit
{
	Q_OBJECT

public:
	explicit SearchLineEdit(QWidget* parent = NULL);

protected:
	void resizeEvent(QResizeEvent* event) override;

private:
	QToolButton* mSearchButton;
};

#endif // SEARCHLINEEDIT_H


