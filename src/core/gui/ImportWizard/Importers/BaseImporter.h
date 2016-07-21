#ifndef BASE_IMPORTER_INCLUDED
#define  BASE_IMPORTER_INCLUDED
#include <QString>
#include <QMetaType>
#include <boost/function.hpp>

#include "ImporterRegistrar.h"

class BaseImporter : public QObject
{
	Q_OBJECT
public:
	virtual ~BaseImporter()
	{
	}

	virtual QString BTClientName() = 0;
	virtual bool TryDetectResumePath(QString&) = 0;
	virtual bool StartImport(QString path) = 0;
	virtual bool isRunning() = 0;
	virtual void CancelImport() = 0;
signals:
	void progress(QString, int);
	void error(QString, QString);
};


Q_DECLARE_METATYPE(BaseImporter*)
#endif

