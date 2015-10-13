#include "CustomScriptSearchProvider.h"
#include <QNetworkRequest>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptValueIterator>
#include <QtScript/QScriptEngine>
#include <QNetworkReply>
#include "QSgml.h"

CustomScriptSearchProvider::CustomScriptSearchProvider() : m_supportedCategories(All), m_responseType(JSON), m_requestType(GET), m_pEngine(NULL)
{
	setObjectName("CustomScriptSearchProvider");
	m_pNetworkManager = new QNetworkAccessManager(this);
	connect(m_pNetworkManager, SIGNAL(finished(QNetworkReply*)), SLOT(OnReplyReady(QNetworkReply*)));
}

QString CustomScriptSearchProvider::Name()
{
	return m_name;
}

QString CustomScriptSearchProvider::Url()
{
	return m_url;
}

int CustomScriptSearchProvider::ResponseType()
{
	return m_responseType;
}

int CustomScriptSearchProvider::RequestType()
{
	return int(m_requestType);
}

void CustomScriptSearchProvider::setName(QString val)
{
	m_name = val;
}

void CustomScriptSearchProvider::setUrl(QString val)
{
	m_url = val;
}

void CustomScriptSearchProvider::setSupportedCategories(int val)
{
	m_supportedCategories = SearchCategories_t(val);
}

void CustomScriptSearchProvider::setResponseType(int val)
{
	m_responseType = ResponseType_t(val);
}

void CustomScriptSearchProvider::setRequestType(int val)
{
	m_requestType = RequestType_t(val);
}

QString CustomScriptSearchProvider::BuildUrl(QString token, int category, int page)
{
	QScriptValue obj = m_scriptVal;

	if (obj.isObject())
	{
		m_pEngine = obj.engine();
		m_scriptVal = obj;
		QScriptValue buildUrlFunk = obj.property("BuildUrl");

		if (!buildUrlFunk.isFunction())
		{
			m_pEngine->currentContext()->throwError("No implementation for SearchPlugin.BuildUrl provided.");
			return "";
		}

		QScriptValueList args;
		args << QScriptValue(token) << category << page;
		QScriptValue ret = buildUrlFunk.call(obj, args);
		return ret.toString();
	}

	return "";
}

QString CustomScriptSearchProvider::BuildPostData(QString token, int category, int page)
{
	QScriptValue obj = m_scriptVal;

	if (obj.isObject())
	{
		m_pEngine = obj.engine();
		m_scriptVal = obj;
		QScriptValue buildUrlFunk = obj.property("BuildPostData");

		if (!buildUrlFunk.isFunction())
		{
			m_pEngine->currentContext()->throwError("No implementation for SearchPlugin.BuildPostData provided.");
			return "";
		}

		QScriptValueList args;
		args << QScriptValue(token) << category << page;
		QScriptValue ret = buildUrlFunk.call(obj, args);
		return ret.toString();
	}

	return "";
}

void CustomScriptSearchProvider::setScriptObject(QScriptValue val)
{
	m_scriptVal = val;
}

void CustomScriptSearchProvider::PeformSearch(QString token, int category, int page)
{
	QNetworkRequest request;
	QString url = BuildUrl(token, category, page);
	qDebug() << "Url:" << url;
	request.setUrl(url);

	if (m_scriptVal.isValid())
	{
		fillAdditionalHeaders(request, m_scriptVal);
	}

	switch (m_requestType)
	{
		case GET:
		{
			m_pNetworkManager->get(request);
			break;
		}

		case POST:
		{
			QByteArray postData = BuildPostData(token, category, page).toUtf8();
			m_pNetworkManager->post(request, postData);
			break;
		}

		default:
			break;
	}
}


void CustomScriptSearchProvider::PeformSearch(QString token, SearchCategories category, int page)
{
	PeformSearch(token, int(category), page);
}

QIcon CustomScriptSearchProvider::getIcon()
{
	return m_icon;
}

void CustomScriptSearchProvider::setIcon(QIcon ico)
{
	m_icon = ico;
}

void CustomScriptSearchProvider::fillAdditionalHeaders(QNetworkRequest& networkRequest, QScriptValue scriptVal)
{
	QScriptValue headers = scriptVal.property("custom_headers");

	if (headers.isValid() && headers.isObject())
	{
		QScriptValueIterator it(headers);

		while (it.hasNext())
		{
			it.next();

			if (it.flags() & QScriptValue::SkipInEnumeration)
			{
				continue;
			}

			networkRequest.setRawHeader(it.name().toUtf8(), it.value().toString().toUtf8());
		}
	}
}

QString CustomScriptSearchProvider::detectEncoding(QString contentType)
{
	QString encoding = "UTF-8";
	QStringList parts = contentType.split(';');

	for (int i = 0; i < parts.size(); i++)
	{
		QString part = parts[i];

		if (part.contains("charset"))
		{
			QStringList charsetParts = part.split('=');
			encoding = charsetParts.last();
		}
	}

	return encoding;
}
void CustomScriptSearchProvider::parseAsHtml(QNetworkReply* pReply)
{
	if (m_pEngine)
	{
		QString contentType = pReply->header(QNetworkRequest::ContentTypeHeader).toString();
		QString encoding = detectEncoding(contentType);
		QTextCodec* codec = QTextCodec::codecForName(encoding.toLocal8Bit());
		QString data = codec->makeDecoder()->toUnicode(pReply->readAll());
		QSgml* sgml = new QSgml(data);
		QScriptValue htmlResult = m_pEngine->newQObject(sgml);
		QScriptValue thisObj = m_scriptVal;

		if (thisObj.isObject() && thisObj.isValid())
		{
			thisObj.setProperty("htmlResult", htmlResult);
			emit HtmlResultReady();
		}
	}
}

void CustomScriptSearchProvider::parseAsJson(QNetworkReply* pReply)
{
	if (m_pEngine)
	{
		QScriptValue globalObject = m_pEngine->globalObject();
		QScriptValue json = globalObject.property("JSON");

		if (json.isObject())
		{
			QScriptValue parseFunk = json.property("parse");

			if (parseFunk.isFunction())
			{
				QString jsonStr = pReply->readAll();
				QScriptValue object = parseFunk.call(json, QScriptValueList() << jsonStr);
				QScriptValue thisObj = m_scriptVal;

				if (thisObj.isObject() && thisObj.isValid())
				{
					thisObj.setProperty("jsonResult", object);
					emit JsonResultReady();
				}
			}
		}
	}
}

void CustomScriptSearchProvider::OnReplyReady(QNetworkReply* pReply)
{
	if (pReply->error() == QNetworkReply::NoError)
	{
		QUrl redirectionTarget = pReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

		if (redirectionTarget.isValid() && m_requestType == GET)
		{
			m_pNetworkManager->get(QNetworkRequest(redirectionTarget));
			return;
		}
		else if (redirectionTarget.isValid() && m_requestType == POST)
		{
			qCritical() << "Post redirects not supported! Report if you see this message.";
		}
		switch (m_responseType)
		{
			case HTML:
			{
				parseAsHtml(pReply);
				break;
			}

			case JSON:
			{
				parseAsJson(pReply);
				break;
			}

			default:
			{
				break;
			}
		}
	}
	else
	{
		emit Error(pReply->errorString());
	}
}

int CustomScriptSearchProvider::SupportedCategories()
{
	return m_supportedCategories;
}
