#include "searchlineedit.h"
#include <QToolButton>
#include <QStyle>
#include "StyleEngene.h"

SearchLineEdit::SearchLineEdit(QWidget* parent) :
	QLineEdit(parent)
{
	// Create the search button and set its icon, cursor, and stylesheet
	mSearchButton = new QToolButton(this);
	mSearchButton->setFixedSize(16, 16);
	mSearchButton->setCursor(Qt::ArrowCursor);
	StyleEngene* pStyleEngene = StyleEngene::getInstance();
	mSearchButton->setIcon(pStyleEngene->getIcon("settings_search"));
	mSearchButton->setStyleSheet("border: none; margin: 0; padding: 0;");
	connect(mSearchButton, SIGNAL(clicked()), SIGNAL(returnPressed()));

	setPlaceholderText(tr("Search"));
}

void SearchLineEdit::resizeEvent(QResizeEvent* event)
{
	Q_UNUSED(event);
	QSize buttonSize = mSearchButton->sizeHint();
	QSize textBoxSize = event->size();
	int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
	mSearchButton->move(rect().right() - frameWidth - buttonSize.width() - 1, (rect().top() + (textBoxSize.height() - buttonSize.height()) / 2 + 1));
}

