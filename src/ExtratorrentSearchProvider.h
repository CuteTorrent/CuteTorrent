#pragma once
#include <ISerachProvider.h>
#include <QtNetwork>
#include "SGML/QSgml.h"
class ExtratorrentSearchProvider : public ISerachProvider
{
	Q_OBJECT
	Q_INTERFACES(ISerachProvider)
private:
	QNetworkAccessManager* m_pNetworkManager;
	QMap<SearchCategories, QString> m_categoryMap;
private slots:
	void replyReady(QNetworkReply*);
	const QString BuildUrl(QString token, SearchCategories category, int page);
	void ParseTableBody(QSgmlTag::QSgmlTaglist tableBody, QSgml* sgml);
	QString detectEncoding(QString contentType);
public:
	ExtratorrentSearchProvider();
	QString Name() override;
	QString Url() override;
	int SupportedCategories() override;
	void PeformSearch(QString token, SearchCategories category, int page) override;
signals:
	void SearchReady(QList<SearchResult*> result) override;
	void Error(QString error) override;
	

};