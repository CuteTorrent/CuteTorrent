#ifndef QKEYEDIT_H
#define QKEYEDIT_H

#include <QLineEdit>
#include <QKeyEvent>
class QKeyEdit : public QLineEdit
{
	Q_OBJECT
public:
	explicit QKeyEdit(QWidget* parent = 0);
protected:
	void keyPressEvent(QKeyEvent* event);
	void changeEvent(QEvent*);
signals:

public slots:

};

#endif // QKEYEDIT_H
