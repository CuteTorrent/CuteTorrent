#pragma once

#include <CustomWindow.h>
#include "ui_AddCommentDialog.h"
class Comment;
class MRichTextEdit;
class RatingWidget;

class AddCommentDialog : public BaseWindow<QDialog>, Ui::AddCommentDialog
{
public:
	enum Mode
	{
		Edit,
		Add
	};
	AddCommentDialog(Mode dialogMode, const QString& infohash, QWidget* parent = 0);
	void setEditComment(Comment comment);
	int exec() override;
	virtual ~AddCommentDialog();
protected:
	void showEvent(QShowEvent *) override;
	void closeEvent(QCloseEvent* e) override;
	QPushButton* getCloseBtn() override;
	QWidget* getTitleBar() override;
	QWidget* centralWidget() override;
	QLabel* getTitleLabel() override;
	QLabel* getTitleIcon() override;
private:
	Q_OBJECT
	void ReplaceControls();
	MRichTextEdit* m_pRichTextEdit;
	RatingWidget* m_pRatingWidget;
	QString m_torrentInfoHash;
	Mode m_dialogMode;
	int m_editCommentId;
private slots:
	void OnAccepted();

signals:
	void closed();
};

