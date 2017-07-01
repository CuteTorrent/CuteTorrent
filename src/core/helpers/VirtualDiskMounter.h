#ifndef _DiscMounter_INCLUDED_
#define _DiscMounter_INCLUDED_


#include <QString>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <Singleton.h>

DEFINE_PTR_CALSS(VirtualDiskMounter)

class VirtualDiskMounter : public Singleton<VirtualDiskMounter>
{
	friend class Singleton<VirtualDiskMounter>;
#ifdef Q_OS_WIN 
	static bool IsWin8OrGreater();
	static void MountVirualDiskImageWin8OrAbove(QString path);
#endif
public:
	static bool MountVirualDiskImage(QString path);
};

#endif

