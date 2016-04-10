#ifndef IMPORTER_REGISTRAR_INCLUDED
#define IMPORTER_REGISTRAR_INCLUDED

#include "ImportersManager.h"
#include "BaseImporter.h"

class BaseImporter;

template <class TImporter>
class ImporterRegistrar
{
public:
	ImporterRegistrar()
	{
		ImportersManager::getInstance()->RegisterImporter(new TImporter);
	}
};

#endif

