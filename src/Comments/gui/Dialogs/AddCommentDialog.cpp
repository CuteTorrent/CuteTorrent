#include "AddCommentDialog.h"
#include <gui/Controls/MRichTextEdit/mrichtextedit.h>
#include <gui/Controls/RaitingWidget.h>
#include <CommentsWebClient.h>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <QDialog>

AddCommentDialog::AddCommentDialog(Mode dialogMode, const QString& infohash, QWidget* parent)
	: BaseWindow<QDialog>(FullTitle, NoResize, parent)
	  , m_torrentInfoHash(infohash)
	  , m_dialogMode(dialogMode)
	  , m_editCommentId(0)
{
	setupUi(this);
	setupCustomWindow();
	setupWindowIcons();
	ReplaceControls();
	if (m_dialogMode == Edit)
	{
		getTitleBar()->setVisible(false);
	}
}

void AddCommentDialog::setEditComment(Comment comment)
{
	m_editCommentId = comment.id;
	m_pRatingWidget->setRating(comment.rating);
	m_pRichTextEdit->setHtml(comment.message);
}

int AddCommentDialog::exec()
{
	show();
	QEventLoop loop;
	connect(this, SIGNAL(closed()), &loop, SLOT(quit()));
	return loop.exec();
}

void AddCommentDialog::OnAccepted()
{
	boost::shared_ptr<CommentsWebClient> commentsWebClient = CommentsWebClient::getInstance();
	Comment comment;
	comment.message = m_pRichTextEdit->toHtml();
	comment.rating = m_pRatingWidget->rating();
	if (m_dialogMode == Add)
	{
		commentsWebClient->AddComment(m_torrentInfoHash, comment);
	}
	else
	{
		comment.id = m_editCommentId;
		comment.infoHash = m_torrentInfoHash;
		commentsWebClient->EditComment(comment);
	}
	close();
}

void AddCommentDialog::ReplaceControls()
{
	m_pRichTextEdit = new MRichTextEdit(this);
	rootContentLayout->addWidget(m_pRichTextEdit, 0, 0, 1, 2);
	m_pRatingWidget = new RatingWidget(this);
	rootContentLayout->addWidget(m_pRatingWidget, 1, 1, Qt::AlignVCenter);
}


AddCommentDialog::~AddCommentDialog()
{
}

void AddCommentDialog::showEvent(QShowEvent* e)
{
	
	m_pRichTextEdit->textCursor().movePosition(QTextCursor::Start);
	QTimer::singleShot(0, m_pRichTextEdit, SLOT(setFocus()));
	QWidget::showEvent(e);
}

void AddCommentDialog::closeEvent(QCloseEvent* e)
{
	emit closed();
	QWidget::closeEvent(e);
}

QPushButton* AddCommentDialog::getCloseBtn()
{
	return pbClose;
}

QWidget* AddCommentDialog::getTitleBar()
{
	return titleBar;
}

QWidget* AddCommentDialog::centralWidget()
{
	return m_centralWidget;
}

QLabel* AddCommentDialog::getTitleLabel()
{
	return LTitle;
}

QLabel* AddCommentDialog::getTitleIcon()
{
	return tbMenu;
}

