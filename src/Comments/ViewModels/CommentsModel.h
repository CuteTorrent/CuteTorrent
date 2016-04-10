#ifndef COMMENTSMODEL_INCLUDED
#define COMMENTSMODEL_INCLUDED
#include <QAbstractListModel>
#include <boost/smart_ptr.hpp>
#include <Comment.h>
class CommentsWebClient;

class CommentsModel : public QAbstractListModel
{
	Q_OBJECT
	QString m_torrentInfoHash;
	boost::shared_ptr<CommentsWebClient> m_pWebClient;
	QList<Comment> m_comments;
public:
	enum Role
	{
		CommentRole = Qt::UserRole,
	};

	CommentsModel(QObject* parent = 0);
	void setTorrentHash(QString infoHash);
	void fetchMore(const QModelIndex& parent) override;
	float AverageRating() const;
	void Clear();
	bool canFetchMore(const QModelIndex& parent) const override;
	int rowCount(const QModelIndex& parent) const override;
	QVariant data(const QModelIndex& index, int role) const override;
	Qt::ItemFlags flags(const QModelIndex& index) const override;
private slots:
	void OnCommentAdded(QString);
};


#endif

