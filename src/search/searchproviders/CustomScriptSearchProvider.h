#ifndef CUSTOM_SCRIPT_SEARCH_PROVIDER_INCLUDED
#define CUSTOM_SCRIPT_SEARCH_PROVIDER_INCLUDED
#include "ISerachProvider.h"
#include <QNetworkAccessManager>
#include <QtScript/QScriptable>
#include <QtScript/QScriptValue>
#include <QIcon>
class QString;
class CustomScriptSearchProvider : public ISerachProvider
{
	Q_OBJECT
	Q_PROPERTY(int responseType READ ResponseType WRITE setResponseType)
	Q_PROPERTY(int requestType READ RequestType WRITE setRequestType)
	Q_PROPERTY(QString url READ Url WRITE setUrl)
	Q_PROPERTY(QString name READ Name WRITE setName)
	Q_PROPERTY(int supportedCategories READ SupportedCategories WRITE setSupportedCategories)
public:
	enum SearchCategories_t
	{
		Software = 0,
		Books = 1,
		Porn = 2,
		Games = 4,
		Music = 8,
		TV = 16,
		Movie = 32,
		Anime = 64,
		All = Music | TV | Porn | Software | Games | Anime | Books | Movie
	};
	Q_ENUMS(SearchCategories_t)

	enum RequestType_t
	{
		GET = 322,
		POST = 323
	};
	Q_ENUMS(RequestType_t)
	enum ResponseType_t
	{
		HTML = 522,
		JSON = 523
	};
	Q_ENUMS(ResponseType_t)

	CustomScriptSearchProvider();
	QString Name() override;
	QString Url() override;
	int SupportedCategories() override;
	int ResponseType();
	int RequestType();
	void setName(QString val);
	void setUrl(QString val);
	void setSupportedCategories(int val);
	void setResponseType(int val);
	void setRequestType(int val);
	bool isBusy() override;
	void setBusy(bool res);
	QString BuildUrl(QString token, int category, int page);
	QString BuildPostData(QString token, int category, int page);
	void setScriptObject(QScriptValue val);
	void PeformSearch(QString token, SearchCategories category, int page) override;
	QIcon getIcon() override;
	void setIcon(QIcon ico);
	void fillAdditionalHeaders(QNetworkRequest& networkRequest, QScriptValue mScriptVal);
public slots:
	void PeformSearch(QString token, int category, int page);
signals:
	void BusyChanged(bool prev, bool current, QString name);
	void SearchReady(QList<SearchResult*> result) override;
	void Error(QString error) override;
	void JsonResultReady();
	void HtmlResultReady();
private:
	Q_DISABLE_COPY(CustomScriptSearchProvider);
	QScriptValue m_scriptVal;
	QString m_name;
	QString m_url;
	SearchCategories_t m_supportedCategories;
	ResponseType_t m_responseType;
	RequestType_t m_requestType;
	QNetworkAccessManager* m_pNetworkManager;
	QScriptEngine* m_pEngine;
	QIcon m_icon;
	bool m_busy;
	void parseAsHtml(QNetworkReply* pReply);
	void parseAsJson(QNetworkReply* pReply);
	QString detectEncoding(QString contentType);
private slots:
	void OnReplyReady(QNetworkReply*);
};
Q_DECLARE_METATYPE(const CustomScriptSearchProvider*);
Q_DECLARE_METATYPE(CustomScriptSearchProvider*);

#endif
