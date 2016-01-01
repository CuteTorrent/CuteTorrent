#include "ImportersManager.h"
#include "UTorrentImporter.h"

ImportersManager::ImportersManager()
{
	RegisterImporter(new UTorrentImporter);
}

void ImportersManager::RegisterImporter(BaseImporter* importer)
{
	if (m_importers.contains(importer))
		return;

	m_importers.append(importer);
}

QList<BaseImporter*> ImportersManager::getImporters() const
{
	return m_importers;
}
