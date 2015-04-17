#ifndef _DiscMounter_INCLUDED_
#define _DiscMounter_INCLUDED_


#include <QString>
#include <boost/smart_ptr/shared_ptr.hpp>

class VirtualDiskMounter;
typedef boost::shared_ptr<VirtualDiskMounter> VirtualDiskMounterPtr;
class VirtualDiskMounter
{
#ifdef Q_WS_WIN
	bool IsWin8OrGreater();
	void MountVirualDiskImageWin8OrAbove(QString path);
#endif
	static boost::weak_ptr<VirtualDiskMounter> m_pInstance;
public:
	static VirtualDiskMounterPtr getInstance();
	bool MountVirualDiskImage(QString path);
};

#endif
