#include "CommentsWebClient.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDebug>
#include "json.h"
#include <QCryptographicHash>
#include "messagebox.h"
#include "User.h"
#include <QApplicationSettings.h>

float CommentsWebClient::averageRating() const
{
	return m_awgRating;
}

CommentsWebClient::CommentsWebClient(QObject* parent)
	: m_pNetworkAccessManager(new QNetworkAccessManager(this))
	  , m_commentsPageCount(0)
	  , m_currentPageNumber(0)
	  , m_awgRating(0)
	  , m_pTokenKeepAliveTimer(new QTimer(this))
{
	QApplicationSettingsPtr settings = QApplicationSettings::getInstance();
	QString login = settings->securedValueString("Comments", "login");
	QString password = settings->securedValueString("Comments", "password");
	if (!login.isEmpty() && !password.isEmpty())
	{
		Login(login, password);
	}
	connect(m_pTokenKeepAliveTimer, SIGNAL(timeout()), SLOT(AuthTokenKeepAlive()));
}

void CommentsWebClient::StartTokenKeepAlive()
{
	m_pTokenKeepAliveTimer->setInterval(2000 * 1000);
	m_pTokenKeepAliveTimer->start();
}

bool CommentsWebClient::haveMore() const
{
	return m_currentPageNumber < m_commentsPageCount;
}

bool CommentsWebClient::isLoggedIn() const
{
	return !m_authToken.isEmpty();
}

bool CommentsWebClient::Login(QString login, QString password)
{
	QtJson::JsonObject credentials;
	credentials["username"] = login;
	QCryptographicHash hash(QCryptographicHash::Md5);
	hash.addData(password.toUtf8());
	credentials["password"] = QString::fromUtf8(hash.result().toHex());
	QString requestUrl = QString("http://api.cutetorrent.info/v1/login/");
	//QString credentialsJson = QtJson::serializeStr(credentials);
	QNetworkRequest request(requestUrl);
	//qDebug() << "Login Json" << credentialsJson;
	request.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
	QNetworkReply* networkReply = m_pNetworkAccessManager->post(request, QtJson::serialize(credentials));
	QEventLoop loop;
	connect(networkReply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();
	if (networkReply->error() == QNetworkReply::NoError)
	{
		QString reply = QString::fromUtf8(networkReply->readAll());
		QtJson::JsonObject json = QtJson::parse(reply).toMap();
		if (json.contains("token"))
		{
			m_authToken = json["token"].toString();
			StartTokenKeepAlive();
			return true;
		}
		qDebug() << "No token field found" << json;
	}
	else
	{
		QByteArray reply = networkReply->readAll();
		qDebug() << "Network error occured" << networkReply->errorString() << reply;
		QtJson::JsonObject apiError = QtJson::parse(reply).toMap();
		if (apiError.contains("message"))
		{
			CustomMessageBox::critical(NULL, tr("LOGIN_ERROR"), tr("LOGIN_ERROR %1").arg(apiError["message"].toString()));
		}
		else
		{
			CustomMessageBox::critical(NULL, tr("NETWORK_ERROR"), tr("NETWORK_ERROR %1\n %2").arg(networkReply->errorString(), QString(reply)));
		}
	}
	return false;
}

bool CommentsWebClient::Register(const User& user)
{
	QString requestUrl("http://api.cutetorrent.info/v1/register/");
	QtJson::JsonObject registerUserJson;
	QtJson::JsonObject userJson;
	QCryptographicHash hash(QCryptographicHash::Md5);
	hash.addData(user.password.toUtf8());

	userJson["username"] = user.name;
	userJson["password"] = QString::fromUtf8(hash.result().toHex());
	userJson["email"] = user.mail;
	registerUserJson["user"] = userJson;
	QNetworkRequest request(requestUrl);
	qDebug() << "Register Json" << QtJson::serializeStr(registerUserJson);
	request.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
	QNetworkReply* networkReply = m_pNetworkAccessManager->post(request, QtJson::serialize(registerUserJson));
	QEventLoop loop;
	connect(networkReply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();
	if (networkReply->error() == QNetworkReply::NoError)
	{
		QString reply = QString::fromUtf8(networkReply->readAll());
		QtJson::JsonObject json = QtJson::parse(reply).toMap();
		if (json.contains("token"))
		{
			m_authToken = json["token"].toString();
			StartTokenKeepAlive();
			return true;
		}
		qDebug() << "No token field found" << json;
	}
	else
	{
		QByteArray reply = networkReply->readAll();
		qDebug() << "Network error occured" << networkReply->errorString() << reply;
		QtJson::JsonObject apiError = QtJson::parse(reply).toMap();
		if (apiError.contains("message"))
		{
			CustomMessageBox::critical(NULL, tr("REGISTER_ERROR"), tr("REGISTER_ERROR %1").arg(apiError["message"].toString()));
		}
		else
		{
			CustomMessageBox::critical(NULL, tr("NETWORK_ERROR"), tr("NETWORK_ERROR %1\n %2").arg(networkReply->errorString(), QString(reply)));
		}
	}
	return false;
}

QList<Comment> CommentsWebClient::GetComments(QString infoHash, int page)
{
	QList<Comment> result;
	if (infoHash.isEmpty())
	{
		m_currentPageNumber = 1;
		m_commentsPageCount = 1;
		m_awgRating = -1;
		return result;
	}
	QString requestUrl = QString("http://api.cutetorrent.info/v1/comments/%1/page/%2?userToken=%3").arg(infoHash, QString::number(page), m_authToken);
	QNetworkReply* networkReply = m_pNetworkAccessManager->get(QNetworkRequest(requestUrl));
	QEventLoop loop;
	connect(networkReply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();
	if (networkReply->error() == QNetworkReply::NoError)
	{
		m_currentPageNumber = page;
		QString reply = QString::fromUtf8(networkReply->readAll());
		QtJson::JsonObject json = QtJson::parse(reply).toMap();
		if (json.contains("pageCount"))
		{
			m_commentsPageCount = json["pageCount"].toInt();
		}
		else
		{
			m_commentsPageCount = 1;
		}
		if (json.contains("rating"))
		{
			m_awgRating = json["rating"].toFloat();
			if (m_awgRating == 0.0)
			{
				m_awgRating = -1.0f;
			}
		}
		else
		{
			m_awgRating = -1.0f;
		}
		if (json.contains("comments"))
		{
			QtJson::JsonArray comments = json["comments"].toList();
			for (int i = 0; i < comments.count(); i++)
			{
				QtJson::JsonObject jsonComment = comments[i].toMap();
				Comment comment;
				comment.id = jsonComment["id"].toUInt();
				comment.rating = jsonComment["rating"].toDouble();
				comment.message = jsonComment["data"].toString();
				comment.authorName = jsonComment["user_id"].toString();
				comment.date = jsonComment["comment_date"].toString();
				comment.infoHash = jsonComment["infohash"].toString();
				comment.isEditable = jsonComment["can_edit"].toBool();
				qDebug() << "Comment:" << comment.id << " " << comment.rating << " " << comment.message << " " << comment.authorName << " " << comment.date;
				result.append(comment);
			}
		}
		else
		{
			qDebug() << "No comments field found" << json;
		}
	}
	else
	{
		qDebug() << "Network error occured" << networkReply->errorString();
		QByteArray reply = networkReply->readAll();
		QtJson::JsonObject apiError = QtJson::parse(reply).toMap();
		if (apiError.contains("message"))
		{
			CustomMessageBox::critical(NULL, tr("GET_COMMENTS_ERROR"), tr("GET_COMMENTS_ERROR %1").arg(apiError["message"].toString()));
		}
		else
		{
			CustomMessageBox::critical(NULL, tr("NETWORK_ERROR"), tr("NETWORK_ERROR %1\n %2").arg(networkReply->errorString(), QString(reply)));
		}
	}

	return result;
}

void CommentsWebClient::AddComment(QString infoHash, const Comment& comment)
{
	if (!isLoggedIn())
	{
		qCritical() << "Not logged in client tried to write comments";
		return;
	}
	QString requestUrl = QString("http://api.cutetorrent.info/v1/comments/%1/").arg(infoHash);
	QtJson::JsonObject addCommentRequestJson;
	addCommentRequestJson["user_token"] = m_authToken;
	QtJson::JsonObject commentJson;
	commentJson["data"] = comment.message;
	commentJson["rating"] = static_cast<double>(comment.rating);
	addCommentRequestJson["comment"] = commentJson;

	QByteArray data = QtJson::serialize(addCommentRequestJson);
	qDebug() << "Sending Json " << QtJson::serializeStr(addCommentRequestJson);
	QNetworkReply* networkReply = m_pNetworkAccessManager->post(QNetworkRequest(requestUrl), data);
	QEventLoop loop;
	connect(networkReply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();
	if (networkReply->error() != QNetworkReply::NoError)
	{
		QString reply = QString::fromUtf8(networkReply->readAll());
		QtJson::JsonObject apiError = QtJson::parse(reply).toMap();
		if (apiError.contains("message"))
		{
			CustomMessageBox::critical(NULL, tr("ADD_COMMENTS_ERROR"), tr("ADD_COMMENTS_ERROR %1").arg(apiError["message"].toString()));
			return;
		}
		else
		{
			CustomMessageBox::critical(NULL, tr("NETWORK_ERROR"), tr("NETWORK_ERROR %1\n%2").arg(networkReply->errorString(), QString(reply)));
			return;
		}
	}
	emit CommentAdded(infoHash);
}

void CommentsWebClient::EditComment(const Comment& comment)
{
	if (!isLoggedIn())
	{
		qCritical() << "Not logged in client tried to write comments";
		return;
	}
	QString requestUrl = QString("http://api.cutetorrent.info/v1/comment/edit/%1").arg(QString::number(comment.id));
	QtJson::JsonObject addCommentRequestJson;
	addCommentRequestJson["user_token"] = m_authToken;
	QtJson::JsonObject commentJson;
	commentJson["data"] = comment.message;
	commentJson["rating"] = static_cast<double>(comment.rating);
	addCommentRequestJson["comment"] = commentJson;

	QByteArray data = QtJson::serialize(addCommentRequestJson);
	qDebug() << "Sending Json " << QtJson::serializeStr(addCommentRequestJson);
	QNetworkReply* networkReply = m_pNetworkAccessManager->post(QNetworkRequest(requestUrl), data);
	QEventLoop loop;
	connect(networkReply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();
	if (networkReply->error() != QNetworkReply::NoError)
	{
		QString reply = QString::fromUtf8(networkReply->readAll());
		QtJson::JsonObject apiError = QtJson::parse(reply).toMap();
		if (apiError.contains("message"))
		{
			CustomMessageBox::critical(NULL, tr("ADD_COMMENTS_ERROR"), tr("ADD_COMMENTS_ERROR %1").arg(apiError["message"].toString()));
			return;
		}
		else
		{
			CustomMessageBox::critical(NULL, tr("NETWORK_ERROR"), tr("NETWORK_ERROR %1\n%2").arg(networkReply->errorString(), QString(reply)));
			return;
		}
	}
	emit CommentAdded(comment.infoHash);
}

void CommentsWebClient::DeleteComment(const Comment& comment)
{
	if (!isLoggedIn())
	{
		qCritical() << "Not logged in client tried to write comments";
		return;
	}
	QString requestUrl = QString("http://api.cutetorrent.info/v1/comment/delete/%1").arg(QString::number(comment.id));
	QtJson::JsonObject addCommentRequestJson;
	addCommentRequestJson["user_token"] = m_authToken;
	
	QByteArray data = QtJson::serialize(addCommentRequestJson);
	qDebug() << "Sending Json " << QtJson::serializeStr(addCommentRequestJson);
	QNetworkRequest request(requestUrl);
	request.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
	QNetworkReply* networkReply = m_pNetworkAccessManager->post(request, data);
	QEventLoop loop;
	connect(networkReply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();
	if (networkReply->error() != QNetworkReply::NoError)
	{
		QString reply = QString::fromUtf8(networkReply->readAll());
		QtJson::JsonObject apiError = QtJson::parse(reply).toMap();
		if (apiError.contains("message"))
		{
			CustomMessageBox::critical(NULL, tr("DELETE_COMMENTS_ERROR"), tr("DELETE_COMMENTS_ERROR %1").arg(apiError["message"].toString()));
			return;
		}
		else
		{
			CustomMessageBox::critical(NULL, tr("NETWORK_ERROR"), tr("NETWORK_ERROR %1\n%2").arg(networkReply->errorString(), QString(reply)));
			return;
		}
	}
	emit CommentAdded(comment.infoHash);
}

int CommentsWebClient::currentPageNumber() const
{
	return m_currentPageNumber;
}

void CommentsWebClient::AuthTokenKeepAlive()
{
	qDebug() << "Keeping token alive" << m_authToken << QDateTime::currentDateTime();
	QString requestUrl = QString("http://api.cutetorrent.info/v1/login/keep-alive/%1").arg(m_authToken);
	QByteArray emptyData;
	QNetworkReply* networkReply = m_pNetworkAccessManager->post(QNetworkRequest(requestUrl), emptyData);
	QEventLoop loop;
	connect(networkReply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();
	if (networkReply->error() != QNetworkReply::NoError)
	{
		QString reply = QString::fromUtf8(networkReply->readAll());
		QtJson::JsonObject apiError = QtJson::parse(reply).toMap();
		if (apiError.contains("message"))
		{
			CustomMessageBox::critical(NULL, tr("KEEP_ALIVE"), tr("KEEP_ALIVE %1").arg(apiError["message"].toString()));
		}
		else
		{
			CustomMessageBox::critical(NULL, tr("NETWORK_ERROR"), tr("NETWORK_ERROR %1\n%2").arg(networkReply->errorString(), QString(reply)));
		}
	}
}

