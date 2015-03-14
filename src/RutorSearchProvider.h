#pragma once
#include "ISerachProvider.h"
#include <QtNetwork>
#include "SGML/QSgml.h"

class RutorSearchProvider : public ISerachProvider
{
	Q_OBJECT
public:
	RutorSearchProvider();
	~RutorSearchProvider();
	QString Name() override;
	QString Url() override;
	int SupportedCategories() override;
	void PeformSearch(QString token, SearchCategories category, int page) override;
signals:
	void SearchReady(QList<SearchResult*> result) override;
	void Error(QString error) override;


private:
	QNetworkAccessManager* m_pNetworAccessManager;
	QMap<SearchCategories, QString> m_categoryMap;
	QString BuildUrl(QString token, SearchCategories category, int page);
	void ParseTableBody(QSgmlTag::QSgmlTaglist tableBody, QSgml* sgml);
private slots:
	void replyReady(QNetworkReply*);
	QString detectEncoding(QString contentType);
};