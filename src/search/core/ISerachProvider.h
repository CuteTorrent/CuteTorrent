#pragma once
#include <QtCore>
class SearchResult;

class ISerachProvider : public QObject
{
	Q_OBJECT
public:
	enum SearchCategories
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

	virtual QString Name() = 0;
	virtual QString Url() = 0;
	virtual QIcon getIcon() = 0;
	virtual bool isBusy() = 0;
	virtual int SupportedCategories() = 0;
	virtual void PeformSearch(QString token, SearchCategories category, int page) = 0;
	signals:
	virtual void SearchReady(QList<SearchResult*> result) = 0;
	virtual void Error(QString error) = 0;
};

Q_DECLARE_INTERFACE(ISerachProvider, "ISerachProvider")

