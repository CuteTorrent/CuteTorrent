#include "VirtualDiskMounter.h"
#include "DT_mounter.h"
#include <boost/smart_ptr/weak_ptr.hpp>
#ifdef Q_WS_WIN
#include <Windows.h>
#define INITGUID
#include <guiddef.h>
#include <virtdisk.h>
typedef bool (WINAPI* IsWindows8OrGreater_t)();
#else

#endif
#ifdef Q_WS_WIN
bool VirtualDiskMounter::IsWin8OrGreater()
{
	HMODULE hKernel32 = LoadLibrary(L"kernel32.dll");
	IsWindows8OrGreater_t IsWindows8OrGreater = reinterpret_cast<IsWindows8OrGreater_t>(GetProcAddress(hKernel32, "IsWindows8OrGreater"));

	if (IsWindows8OrGreater != NULL)
	{
		return IsWindows8OrGreater();
	}

	OSVERSIONINFOW currentVersion;
	GetVersionEx(&currentVersion);
	return (currentVersion.dwMajorVersion >= 6 && currentVersion.dwMinorVersion >= 2);
}

void VirtualDiskMounter::MountVirualDiskImageWin8OrAbove(QString path)
{
	VIRTUAL_STORAGE_TYPE storrageType = {VIRTUAL_STORAGE_TYPE_DEVICE_ISO, VIRTUAL_STORAGE_TYPE_VENDOR_MICROSOFT};
	wchar_t* pWStrPath = new wchar_t[path.length() + 1];
	memset(pWStrPath, 0, (path.length() + 1) * sizeof(wchar_t));
	path.toWCharArray(pWStrPath);
	HANDLE hVirtualDisk;
	DWORD dwRes = OpenVirtualDisk(&storrageType, pWStrPath, VIRTUAL_DISK_ACCESS_ATTACH_RO |
	                              VIRTUAL_DISK_ACCESS_DETACH, OPEN_VIRTUAL_DISK_FLAG_NONE, NULL, &hVirtualDisk);

	if (dwRes == ERROR_SUCCESS)
	{
		ATTACH_VIRTUAL_DISK_PARAMETERS parameters;
		parameters.Version = ATTACH_VIRTUAL_DISK_VERSION_1;
		dwRes = AttachVirtualDisk(hVirtualDisk, NULL, ATTACH_VIRTUAL_DISK_FLAG_READ_ONLY | ATTACH_VIRTUAL_DISK_FLAG_PERMANENT_LIFETIME, 0, &parameters, NULL);

		if (dwRes != ERROR_SUCCESS)
		{
		}
	}
}
#endif

bool VirtualDiskMounter::MountVirualDiskImage(QString path)
{
#ifdef Q_WS_WIN

	if (IsWin8OrGreater() && path.endsWith(".iso"))
	{
		MountVirualDiskImageWin8OrAbove(path);
	}
	else
	{
		DT_mounter::mountImage(path);
	}

#else
#endif
	return true;
}

