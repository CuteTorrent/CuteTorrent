#ifndef COMMENT_ITEM_DELEGATE_INCLUDED
#define COMMENT_ITEM_DELEGATE_INCLUDED
#include <QStyledItemDelegate>
#include <Comment.h>
#include <QPushButton>

class CommentItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT
	QStyle::State m_buttonState;
	QModelIndex m_pressedIndex;
	QIcon m_removeIcon;
	QScopedPointer<QPushButton> button;
protected:
	bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) override;
public:
	explicit CommentItemDelegate(QObject* parent);
	void drawComment(QPainter* painter, const QStyleOptionViewItem& option, const Comment& comment, const QModelIndex& index) const;
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	QSize sizeHint(const QStyleOptionViewItem& option, const Comment& comment) const;
	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
signals:
	void buttonClicked(const QModelIndex& index);
};

#endif

