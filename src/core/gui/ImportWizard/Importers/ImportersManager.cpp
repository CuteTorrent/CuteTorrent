#include "ImportersManager.h"
#include "UTorrentImporter.h"
#include "qBittorrentImporter.h"

ImportersManager::ImportersManager()
{
	RegisterImporter(new UTorrentImporter);
	RegisterImporter(new qBittorrentImporter);
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

