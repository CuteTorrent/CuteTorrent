#include "IpToCountryResolver.h"
#include <QUrl>
#include <QFile>
#include "StaticHelpers.h"
#include <QDesktopServices>
#include "filedownloader.h"



IpToCountryResolver::IpToCountryResolver()
	: m_pIpDb(new MMDB_s)
	, m_pFileDownloader(new FileDownloader())
{
	QString dataDir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
	QUrl geoIpUrl = "http://geolite.maxmind.com/download/geoip/database/GeoLite2-Country.mmdb.gz";
	QString geoIpPath = StaticHelpers::CombinePathes(dataDir, "GeoIP.dat");

	connect(m_pFileDownloader.get(), SIGNAL(DownloadReady(QUrl, QTemporaryFilePtr)), SLOT(OnGeoIpDbDownloaded(QUrl, QTemporaryFilePtr)));
	if (QFile::exists(geoIpPath))
	{
		QFileInfo fileInfo(geoIpPath);

		if (QDateTime::currentDateTime() >= fileInfo.lastModified().addMonths(1))
		{
			m_pFileDownloader->download(geoIpUrl);
		}
		else
		{
			int status = MMDB_open(geoIpPath.toAscii().data(), MMDB_MODE_MMAP, m_pIpDb);

			if (MMDB_SUCCESS != status) {
				qDebug("Can't open %s - %s",
					filename, MMDB_strerror(status));

				if (MMDB_IO_ERROR == status) {
					qDebug("IO error: %s\n", std::strerror(errno));
				}
				exit(1);
			}
		}
	}
	else
	{
		m_pFileDownloader->download(geoIpUrl);
	}
}

IpToCountryResolver::~IpToCountryResolver()
{
}

QString IpToCountryResolver::GetCountryISOCode(QString ip)
{
	int gai_error, mmdb_error;
	MMDB_lookup_result_s result =
		MMDB_lookup_string(m_pIpDb, ip.toAscii().data(), &gai_error, &mmdb_error);

	if (0 != gai_error) {
		qDebug("Error from getaddrinfo for %s - %s\n\n",
			ip.toAscii().data(), gai_strerror(gai_error));
		exit(2);
	}

	if (MMDB_SUCCESS != mmdb_error) {
		qDebug("Got an error from libmaxminddb: %s\n\n",
			MMDB_strerror(mmdb_error));
		exit(3);
	}

	MMDB_entry_data_list_s *entry_data_list = NULL;

	if (result.found_entry) {
		int status = MMDB_get_entry_data_list(&result.entry,
			&entry_data_list);

		if (MMDB_SUCCESS != status) {
			qDebug("Got an error looking up the entry data - %s\n",
				MMDB_strerror(status));
		
			goto end;
		}

		if (NULL != entry_data_list) {
			MMDB_dump_entry_data_list(stdout, entry_data_list, 2);
			MMDB_entry_data_s entry_data;
			int status =
				MMDB_get_value(&result.entry, &entry_data,
				"country", "iso_code", NULL);
			if (MMDB_SUCCESS != status)
			{
				qDebug("Got MMDB_get_value error looking up the entry data - %s",
					MMDB_strerror(status));
			}
			if (entry_data.has_data)
			{
				MMDB_free_entry_data_list(entry_data_list);
				char iso_code[3] = {0};
				strncpy(iso_code, entry_data.utf8_string, 2);
				return QString(iso_code);
			}
		}
	}
	else {
		qDebug(
			"No entry for this IP address (%s) was found",
			ip.toAscii().data());
		
	}

end:
	MMDB_free_entry_data_list(entry_data_list);
		return "";
}


void IpToCountryResolver::OnGeoIpDbDownloaded(QUrl, QTemporaryFilePtr pFile)
{
	pFile->open();
	QByteArray compressed = pFile->readAll();
	QByteArray unpacked = StaticHelpers::gUncompress(compressed);

	QString dataDir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
	QString geoIpPath = StaticHelpers::CombinePathes(dataDir, "GeoIP.dat");
	QFile file(geoIpPath);
	if (file.open(QIODevice::WriteOnly))
	{
		file.write(unpacked);
		file.close();
	}
	int status = MMDB_open(geoIpPath.toAscii().data(), MMDB_MODE_MMAP, m_pIpDb);

	if (MMDB_SUCCESS != status) {
		qDebug("Can't open %s - %s",
			filename, MMDB_strerror(status));

		if (MMDB_IO_ERROR == status) {
			qDebug("IO error: %s", std::strerror(errno));
		}
		exit(1);
	}
}
