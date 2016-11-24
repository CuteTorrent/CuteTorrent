#ifndef COMMENTS_WEBCLIENT_INCLUDED
#define COMMENTS_WEBCLIENT_INCLUDED
#include <QObject>
#include <Singleton.h>
#include "Comment.h"
#include <QTimer>


struct User;
class QNetworkAccessManager;


class CommentsWebClient : public QObject, public Singleton<CommentsWebClient>
{
	friend class Singleton<CommentsWebClient>;
	Q_OBJECT
private:
	boost::scoped_ptr<QNetworkAccessManager> m_pNetworkAccessManager;
	QString m_authToken;
	int m_commentsPageCount, m_currentPageNumber;
	float m_awgRating;
	QTimer* m_pTokenKeepAliveTimer;
	CommentsWebClient(QObject* parent = NULL);
	void StartTokenKeepAlive();

	signals:
	void CommentAdded(QString infoHash);
	void commentsChanged();
public:
	float averageRating() const;

	bool haveMore() const;
	bool isLoggedIn() const;

	bool Login(QString login, QString password);
	bool Register(const User& user);
	QList<Comment> GetComments(QString infoHash, int page = 1);
	void AddComment(QString infoHash, const Comment& comment);
	void EditComment(const Comment& comment);
	void DeleteComment(const Comment& comment);
	int currentPageNumber() const;
private slots:
	void AuthTokenKeepAlive();
};

#endif

