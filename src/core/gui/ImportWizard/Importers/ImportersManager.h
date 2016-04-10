#ifndef ImportersManager_INCLUDED
#define ImportersManager_INCLUDED
#include <QList>
class BaseImporter;
#include <Singleton.h>

class ImportersManager : public Singleton<ImportersManager>
{
	friend class Singleton<ImportersManager>;
	QList<BaseImporter*> m_importers;
public:
	ImportersManager();
	void RegisterImporter(BaseImporter* importer);
	QList<BaseImporter*> getImporters() const;
};

DEFINE_PTR_CALSS(ImportersManager)
#endif

