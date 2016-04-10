#ifndef UTORRENT_IMPORTER_INCLUDED
#define UTORRENT_IMPORTER_INCLUDED
#include "BaseImporter.h"
#include <QObject>

class UTorrentImporter : public BaseImporter
{
	Q_OBJECT
public:
	UTorrentImporter(QObject* parent = 0);
	~UTorrentImporter() override;
	QString BTClientName() override;
	bool TryDetectResumePath(QString&) override;
	bool StartImport(QString path) override;
	void CancelImport() override;
	bool isRunning() override;
private:

	QThread* m_pWorkerThread;
	QString m_resumeFilePath;

	bool m_bIsRunning;
private slots:
	void runImport();
	void onFinished();
};


#endif

