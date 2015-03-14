#include "qkeyedit.h"
#include <QDebug>
QKeyEdit::QKeyEdit(QWidget* parent) :
	QLineEdit(parent)
{
	setMouseTracking(true);
	setToolTip(tr("KEY_EDIT_CLEAR_HINT"));
}

void QKeyEdit::keyPressEvent(QKeyEvent* keyEvent)
{
	int keyInt = keyEvent->key();
	Qt::KeyboardModifiers modifiers = keyEvent->modifiers();
	Qt::Key key = static_cast<Qt::Key>(keyInt);

	if(key == Qt::Key_unknown)
	{
		qDebug() << "Unknown key from a macro probably";
		return;
	}

	if(key == Qt::Key_Control ||
	        key == Qt::Key_Shift ||
	        key == Qt::Key_Alt ||
	        key == Qt::Key_Meta)
	{
		return;
	}

	if(key == Qt::Key_Escape)
	{
		setText("");
		return;
	}

	QString keyStr = QKeySequence(keyEvent->key() | keyEvent->modifiers()).toString();
	setText(keyStr);
}

void QKeyEdit::changeEvent(QEvent *event)
{
	if (event->type() == QEvent::LanguageChange)
	{
		setToolTip(tr("KEY_EDIT_CLEAR_HINT"));
	}
}
