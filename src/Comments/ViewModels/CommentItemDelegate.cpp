#include "CommentItemDelegate.h"
#include <Comment.h>
#include "CommentsModel.h"
#include <gui/Controls/RaitingWidget.h>
#include <gui/Dialogs/AddCommentDialog.h>
#include <QPushButton>

bool CommentItemDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
	if (event->type() == QEvent::MouseButtonPress ||
		event->type() == QEvent::MouseButtonRelease)
	{
		
	}
	else
	{
		//ignoring other mouse event and reseting button's state
		m_buttonState = QStyle::State_Raised;
		m_pressedIndex = QModelIndex();
		return false;
	}
	QStyle* style;
	QStyleOptionViewItemV4 opt = option;
	initStyleOption(&opt, index);
	if (opt.widget != NULL)
	{
		style = opt.widget->style();
	}
	else
	{
		style = QApplication::style();
	}

	QRect fillArea(option.rect);
	QSize m(4, 4);
	fillArea.adjust(m.width(), m.height(), -m.width(), -m.height());

	const int iconSize(style->pixelMetric(QStyle::PM_SmallIconSize));
	QRect buttonRect(fillArea.x() + fillArea.width() - iconSize, fillArea.y(), iconSize, iconSize);

	QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
	if (!buttonRect.contains(mouseEvent->pos())) {
		m_buttonState = QStyle::State_Raised;
		m_pressedIndex = QModelIndex();
		return false;
	}

	if (event->type() == QEvent::MouseButtonPress) {
		m_pressedIndex = index;
		m_buttonState = QStyle::State_Sunken;
	}
	else if (event->type() == QEvent::MouseButtonRelease) {
		m_buttonState = QStyle::State_Raised;
		m_pressedIndex = QModelIndex();
		emit buttonClicked(index);
	}
	return true;
}

CommentItemDelegate::CommentItemDelegate(QObject* parent) : QStyledItemDelegate(parent), button(new QPushButton)
{
	m_removeIcon = StyleEngene::getInstance()->getIcon("delete");
}

void CommentItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	try
	{
		Comment comment(index.data(CommentsModel::CommentRole).value<Comment>());
		painter->save();
		painter->setClipRect(option.rect);
		drawComment(painter, option, comment, index);
	}
	catch (...)
	{
		qDebug() << "Excepion in CommentItemDelegate::paint";
	}

	painter->restore();
}


void CommentItemDelegate::drawComment(QPainter* painter, const QStyleOptionViewItem& option, const Comment& comment, const QModelIndex& index) const
{
	QStyleOptionViewItemV4 opt = option;
	initStyleOption(&opt, index);
	QStyle* style;

	if (opt.widget != NULL)
	{
		style = opt.widget->style();
	}
	else
	{
		style = QApplication::style();
	}

	QFont authorNameFont(option.font);
	authorNameFont.setPixelSize(10);
	authorNameFont.setWeight(QFont::Bold);
	const QFontMetrics authorNameFM(authorNameFont);
	QString authorNameStr(comment.authorName);
	//qDebug() << "authorNameStr " << authorNameStr;
	QSize authorNameSize(authorNameFM.size(0, authorNameStr));

	QFont commentDateFont(option.font);
	commentDateFont.setPixelSize(10);
	const QFontMetrics commentDateFM(commentDateFont);
	QString commentDateStr(comment.date);
	QSize commentDateSize(authorNameFM.size(0, commentDateStr));

	QFont commentFont(option.font);
	commentFont.setPixelSize(14);
	commentFont.setWeight(QFont::DemiBold);
	const QFontMetrics commentFM(commentFont);
	QString commentStr(comment.message);

	QPalette::ColorGroup cg = QPalette::Normal;

	if (!(option.state & QStyle::State_Enabled))
	{
		cg = QPalette::Disabled;
	}

	if (cg == QPalette::Normal && !(option.state & QStyle::State_Active))
	{
		cg = QPalette::Inactive;
	}

	QPalette::ColorRole cr = QPalette::Text;

	QRect fillArea(option.rect);
	QSize m(4, 4);
	fillArea.adjust(m.width(), m.height(), -m.width(), -m.height());

	QRect authorNameArea(fillArea.x(), fillArea.y(), authorNameSize.width(), authorNameSize.height());
	const int iconSize(style->pixelMetric(QStyle::PM_ListViewIconSize));
	QRect removeIconArea(fillArea.x() + fillArea.width() - iconSize, fillArea.y(), iconSize, iconSize);
	QRect commentDateArea(fillArea.x() + authorNameArea.width() + m.width(), fillArea.y(), commentDateSize.width(), commentDateSize.height());

	QTextDocument doc;
	QTextOption docOptions;
	docOptions.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
	doc.setDefaultTextOption(docOptions);
	doc.setHtml(commentStr);
	doc.setTextWidth(fillArea.width() - m.width()*2);
	QRect commentArea(fillArea.x(), fillArea.y() + m.height() + authorNameArea.height(), doc.idealWidth(), doc.size().height());

	QRect ratingArea(fillArea.x(), fillArea.y() + authorNameSize.height() + 2 * m.height() + commentArea.height(),
	                 fillArea.width(), fillArea.height() - (authorNameSize.height() + 2 * m.height() + commentArea.height()));

	painter->setFont(authorNameFont);
	style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, opt.widget);
	style->drawItemText(painter, authorNameArea, Qt::AlignLeft, opt.palette,
	                    option.state & QStyle::State_Enabled, authorNameStr);
	
	if (comment.isEditable)
	{
		
		//painter->fillRect(removeIconArea, option.palette.button());
		QStyleOptionButton styleOptionButton;
		
		
		styleOptionButton.rect = removeIconArea.adjusted(-2, -2, 2, 2);
		styleOptionButton.icon = m_removeIcon;
		styleOptionButton.iconSize = QSize(iconSize, iconSize);
		styleOptionButton.state =  QStyle::State_Enabled;
		if (m_buttonState == QStyle::State_Sunken && m_pressedIndex.isValid() && m_pressedIndex == index)
		
		{ 
			styleOptionButton.state |= m_buttonState;
		}
		style->drawControl(QStyle::CE_PushButton, &styleOptionButton, painter,
				button.data());
		//removeIcon.paint(painter, removeIconArea);
	}
	painter->setFont(commentDateFont);
	style->drawItemText(painter, commentDateArea, Qt::AlignLeft, opt.palette,
	                    option.state & QStyle::State_Enabled, commentDateStr);
	QAbstractTextDocumentLayout::PaintContext ctx;
	/*if (opt.state & QStyle::State_Selected)
		ctx.palette.setColor(QPalette::Text, opt.palette.color(QPalette::Active, QPalette::HighlightedText));*/
	painter->setFont(commentFont);
	/*style->drawItemText(painter, commentArea, Qt::AlignJustify | Qt::TextWordWrap,
		opt.palette, option.state & QStyle::State_Enabled, commentStr);*/

	painter->save();
	painter->translate(commentArea.topLeft());
	painter->setClipRect(commentArea.translated(-commentArea.topLeft()));
	//doc.setPageSize(commentArea.size());
	doc.documentLayout()->draw(painter, ctx);
	painter->restore();
	qDebug() << "Rating Area" << ratingArea;
	RatingWidget widget;
	widget.setFixedHeight(ratingArea.height());
	widget.setRating(comment.rating);
	painter->translate(ratingArea.topLeft());
	widget.render(painter);
}


QSize CommentItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	try
	{
		Comment comment(index.data(CommentsModel::CommentRole).value<Comment>());
		initStyleOption(const_cast<QStyleOptionViewItem*>(&option), index);
		return sizeHint(option, comment);
	}
	catch (...)
	{
		qDebug() << "Exception in CommentItemDelegate::sizeHint";
	}

	return QSize(0, 0);
}

QSize CommentItemDelegate::sizeHint(const QStyleOptionViewItem& option, const Comment& comment) const
{
	QFont authorNameFont(option.font);
	authorNameFont.setPixelSize(10);
	authorNameFont.setWeight(QFont::Bold);
	const QFontMetrics authorNameFM(authorNameFont);
	QString authorNameStr(comment.authorName);
	QSize authorNameSize(authorNameFM.size(0, authorNameStr));

	QFont commentFont(option.font);
	commentFont.setPixelSize(14);
	commentFont.setWeight(QFont::DemiBold);
	const QFontMetrics commentFM(commentFont);
	QString commentStr(comment.message);

	QRect fillArea(option.rect);
	QSize m(4, 4);
	fillArea.adjust(m.width(), m.height(), -m.width(), -m.height());

	/*QRect commentArea = commentFM.boundingRect(fillArea.x(),
		fillArea.y() + std::max(authorNameSize.height(), commentDateSize.height()) + m.height(),
		fillArea.width(), 8000, Qt::AlignJustify | Qt::TextWordWrap, commentStr);*/

	QTextDocument doc;
	QTextOption docOptions;
	docOptions.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
	doc.setDefaultTextOption(docOptions);
	doc.setHtml(comment.message);
	doc.setTextWidth(option.rect.width());
	
	QRect commentArea(0, 0, doc.idealWidth(), doc.size().height());

	int minRatingHeight = 20;

	int width = fillArea.width();
	int height = authorNameSize.height() + m.height() * 2 + commentArea.height() + minRatingHeight;
	if ((option.state & QStyle::State_Editing ) == QStyle::State_Editing)
	{
		if (height < 120)
			height = 120;
	}
	//qDebug() << "sizeHint" << width << height;
	return QSize(width, height);
}


QWidget* CommentItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	Comment comment = index.data(CommentsModel::CommentRole).value<Comment>();
	AddCommentDialog* editor = new AddCommentDialog(AddCommentDialog::Edit, comment.infoHash, parent);
	editor->setMinimumHeight(120);
	editor->setEditComment(comment);
	return editor;
}

void CommentItemDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	
	editor->setGeometry(option.rect);
}

