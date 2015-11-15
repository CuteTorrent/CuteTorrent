
#ifndef __ctkTreeComboBox_h
#define __ctkTreeComboBox_h

// Qt includes
#include <QComboBox>

class TreeBoxPrivate;
class QTreeView;

/// \ingroup Widgets
/// Description:
/// ComboBox that displays the items as a tree view.
/// See below for a use case:
///    TreeBox combo;
///    QStandardItemModel model;
///    model.appendRow(new QStandardItem("Test1"));
///    model.item(0)->appendRow(new QStandardItem("Test1.1"));
///    model.item(0)->appendRow(new QStandardItem("Test1.2"));
///    model.item(0)->appendRow(new QStandardItem("Test1.3"));
///    model.appendRow(new QStandardItem("Test2"));
///    model.appendRow(new QStandardItem("Test3"));
///    combo.setModel(&model);
///    combo.show();
/// TODO fix size of the view
class TreeBox : public QComboBox
{
	Q_OBJECT
		/// Column index visible in the view. If \sa visibleModelColumn is -1
		/// (default) then all columns are visible.
		Q_PROPERTY(int visibleModelColumn READ visibleModelColumn WRITE setVisibleModelColumn)
public:
	typedef QComboBox Superclass;
	explicit TreeBox(QWidget* parent = 0);
	virtual ~TreeBox();

	int visibleModelColumn()const;
	void setVisibleModelColumn(int index);

	QModelIndex currentIndex() const;
	void setCurrentIndex(const QModelIndex& idx);
	void setCurrentIndex(const int& idx);

	virtual bool eventFilter(QObject* object, QEvent* event) override;
	virtual void showPopup() override;
	virtual void hidePopup() override;

	/// TreeBox uses a QTreeView for its model view. treeView() is a
	/// utility function that cast QComboBox::view() into a QTreeView.
	/// \sa view()
	QTreeView* treeView()const;

protected:
	virtual void paintEvent(QPaintEvent*) override;

	protected Q_SLOTS:
	void resizePopup();

signals:
	void popupShow();
	void popupHide();

protected:
	QScopedPointer<TreeBoxPrivate> d_ptr;

private:
	Q_DECLARE_PRIVATE(TreeBox);
	Q_DISABLE_COPY(TreeBox);
};

#endif