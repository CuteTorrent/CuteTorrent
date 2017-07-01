#ifndef IP_TO_COUNTRY_RESOLVER
#define IP_TO_COUNTRY_RESOLVER
#include "Singleton.h"
#include <QString>
#include <maxminddb.h>
#include <QObject>
#include "FileViewModel.h"
#include "FaviconDownloader.h"


class IpToCountryResolver : public QObject, public Singleton<IpToCountryResolver>
{
	friend class Singleton<IpToCountryResolver>;
	Q_OBJECT
public:
	~IpToCountryResolver();
	QString GetCountryISOCode(QString ip);
protected:
	IpToCountryResolver();
private slots:
	void OnGeoIpDbDownloaded(QUrl, QTemporaryFilePtr);
private:
	MMDB_s* m_pIpDb;
	FileDownloaderPtr m_pFileDownloader;
};

#endif