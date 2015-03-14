#pragma once
#include "ISerachProvider.h"
#include <QtNetwork>
class KickassSearchProvider : public ISerachProvider
{
	Q_OBJECT
	Q_INTERFACES(ISerachProvider)
public:
	KickassSearchProvider();
	QString Name() override;
	QString Url() override;
	int SupportedCategories() override;
	void PeformSearch(QString token, SearchCategories category, int page) override;
signals:
	void SearchReady(QList<SearchResult*> result) override;
	void Error(QString error) override;
private:
	QNetworkAccessManager* m_pNetworkManager;
	QString BuildUrl(QString token, SearchCategories category, int page);
	QMap<SearchCategories, QString> m_categoryMap;
	QString detectEncoding(QString contentType);
private slots:
	void replyReady(QNetworkReply*);
};