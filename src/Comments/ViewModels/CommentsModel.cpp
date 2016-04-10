#include "CommentsModel.h"
#include <CommentsWebClient.h>
#include <QDebug>

CommentsModel::CommentsModel(QObject* parent)
	: m_pWebClient(CommentsWebClient::getInstance())
{
	connect(m_pWebClient.get(), SIGNAL(CommentAdded(QString)), SLOT(OnCommentAdded(QString)));
}

void CommentsModel::setTorrentHash(QString infoHash)
{
	if (m_torrentInfoHash == infoHash)
		return;

	m_torrentInfoHash = infoHash;
	beginResetModel();
	m_comments = m_pWebClient->GetComments(infoHash);
	qDebug() << "Comments count:" << m_comments.count();
	endResetModel();
}

void CommentsModel::fetchMore(const QModelIndex& parent)
{
	qDebug() << "fetchMore:" << m_comments.count();
	beginResetModel();
	m_comments.append(m_pWebClient->GetComments(m_torrentInfoHash, m_pWebClient->currentPageNumber() + 1));
	endResetModel();
	qDebug() << "after fetchMore:" << m_comments.count();
}

float CommentsModel::AverageRating() const
{
	return m_pWebClient->averageRating();
}

void CommentsModel::Clear()
{
	beginResetModel();
	m_torrentInfoHash = "";
	m_comments.clear();
	endResetModel();
}

bool CommentsModel::canFetchMore(const QModelIndex& parent) const
{
	return m_pWebClient->haveMore();
}

int CommentsModel::rowCount(const QModelIndex& parent) const
{
	if (parent.isValid())
		return 0;

	return m_comments.count();
}

QVariant CommentsModel::data(const QModelIndex& index, int role) const
{
	if (index.isValid() && role == CommentRole)
	{
		int row = index.row();
		if (row < m_comments.count())
		{
			return QVariant::fromValue(m_comments[row]);
		}
	}
	return QVariant();
}

Qt::ItemFlags CommentsModel::flags(const QModelIndex& index) const
{
	if (index.isValid())
	{
		int row = index.row();
		if (row < m_comments.count())
		{
			if (m_comments[row].isEditable)
			{
				return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
			}
			return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
		}
	}
	return Qt::NoItemFlags;
}

void CommentsModel::OnCommentAdded(QString infoHash)
{
	if (m_torrentInfoHash == infoHash)
	{
		beginResetModel();
		m_comments = m_pWebClient->GetComments(infoHash);
		qDebug() << "Comments count:" << m_comments.count();
		endResetModel();
	}
}

