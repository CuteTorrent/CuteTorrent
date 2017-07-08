#include "FileIconProvider.h"
#ifdef Q_OS_WIN
#include <Windows.h>
#include <Shellapi.h>
#include <QtWinExtras/QtWin>
#endif
#include <QPixmapCache>
#include <QFileIconProvider>
#include <QDir>


FileIconProvider::FileIconProvider(QObject *parent)
	: QObject(parent)
{
}

FileIconProvider::~FileIconProvider()
{
}
#ifdef  Q_OS_WIN
QIcon FileIconProvider::getWinIcon(const QFileInfo &fileInfo) const
{
	QIcon retIcon;
	const QString fileExtension = QLatin1Char('.') + fileInfo.suffix().toUpper();

	QString key;
	if (fileInfo.isFile() && !fileInfo.isExecutable() && !fileInfo.isSymLink() && fileExtension != QLatin1String(".ICO"))
		key = QLatin1String("qt_") + fileExtension;

	QPixmap pixmap;
	if (!key.isEmpty()) {
		QPixmapCache::find(key, pixmap);
	}

	if (!pixmap.isNull()) {
		retIcon.addPixmap(pixmap);
		if (QPixmapCache::find(key + QLatin1Char('l'), pixmap))
			retIcon.addPixmap(pixmap);
		return retIcon;
	}

	/* We don't use the variable, but by storing it statically, we
	* ensure CoInitialize is only called once. */
	static HRESULT comInit = CoInitialize(NULL);
//	Q_UNUSED(comInit);

	SHFILEINFO info;
	unsigned long val = 0;

	//Get the small icon

	val = SHGetFileInfo(fileInfo.absoluteFilePath().toStdWString().c_str(), FILE_ATTRIBUTE_NORMAL, &info, sizeof(info), SHGFI_ICON | SHGFI_USEFILEATTRIBUTES);


	// Even if GetFileInfo returns a valid result, hIcon can be empty in some cases
	if (val && info.hIcon) {
		if (fileInfo.isDir() && !fileInfo.isRoot()) {
			//using the unique icon index provided by windows save us from duplicate keys
			key = QString::fromLatin1("qt_dir_%1").arg(info.iIcon);
			QPixmapCache::find(key, pixmap);
			if (!pixmap.isNull()) {
				retIcon.addPixmap(pixmap);
				if (QPixmapCache::find(key + QLatin1Char('l'), pixmap))
					retIcon.addPixmap(pixmap);
				DestroyIcon(info.hIcon);
				return retIcon;
			}
		}
		if (pixmap.isNull()) {

			pixmap = QtWin::fromHICON(info.hIcon);

			if (!pixmap.isNull()) {
				retIcon.addPixmap(pixmap);
				if (!key.isEmpty())
					QPixmapCache::insert(key, pixmap);
			}
			else {
				qWarning("QFileIconProviderPrivate::getWinIcon() no small icon found");
			}
		}
		DestroyIcon(info.hIcon);
	}

	
	return retIcon;
}
#endif
#ifdef Q_OS_MAC
QIcon FileIconProvider::getMacIcon(const QFileInfo &fi) const
{
	QIcon retIcon;
	QString fileExtension = fi.suffix().toUpper();
	fileExtension.prepend(QLatin1String("."));

	const QString keyBase = QLatin1String("qt_") + fileExtension;

	QPixmap pixmap;
	if (fi.isFile() && !fi.isExecutable() && !fi.isSymLink()) {
		QPixmapCache::find(keyBase + QLatin1String("16"), pixmap);
	}

	if (!pixmap.isNull()) {
		retIcon.addPixmap(pixmap);
		if (QPixmapCache::find(keyBase + QLatin1String("32"), pixmap)) {
			retIcon.addPixmap(pixmap);
			if (QPixmapCache::find(keyBase + QLatin1String("64"), pixmap)) {
				retIcon.addPixmap(pixmap);
				if (QPixmapCache::find(keyBase + QLatin1String("128"), pixmap)) {
					retIcon.addPixmap(pixmap);
					return retIcon;
				}
			}
		}
	}


	FSRef macRef;
	OSStatus status = FSPathMakeRef(reinterpret_cast<const UInt8*>(fi.canonicalFilePath().toUtf8().constData()),
		&macRef, 0);
	if (status != noErr)
		return retIcon;
	FSCatalogInfo info;
	HFSUniStr255 macName;
	status = FSGetCatalogInfo(&macRef, kIconServicesCatalogInfoMask, &info, &macName, 0, 0);
	if (status != noErr)
		return retIcon;
	IconRef iconRef;
	SInt16 iconLabel;
	status = GetIconRefFromFileInfo(&macRef, macName.length, macName.unicode,
		kIconServicesCatalogInfoMask, &info, kIconServicesNormalUsageFlag,
		&iconRef, &iconLabel);
	if (status != noErr)
		return retIcon;
	qt_mac_constructQIconFromIconRef(iconRef, 0, &retIcon);
	ReleaseIconRef(iconRef);

	if (fi.isFile() && !fi.isExecutable() && !fi.isSymLink()) {
		pixmap = retIcon.pixmap(16);
		QPixmapCache::insert(keyBase + QLatin1String("16"), pixmap);
		pixmap = retIcon.pixmap(32);
		QPixmapCache::insert(keyBase + QLatin1String("32"), pixmap);
		pixmap = retIcon.pixmap(64);
		QPixmapCache::insert(keyBase + QLatin1String("64"), pixmap);
		pixmap = retIcon.pixmap(128);
		QPixmapCache::insert(keyBase + QLatin1String("128"), pixmap);
	}

	return retIcon;
}
#endif
QIcon FileIconProvider::getIcon(QFileInfo info) const
{
#if defined(Q_OS_UNIX) && !defined(QT_NO_STYLE_GTK)
	if (X11->desktopEnvironment == DE_GNOME) {
		QIcon gtkIcon = getGtkFilesystemIcon(info);
		if (!gtkIcon.isNull())
			return gtkIcon;
	}
#endif

#ifdef Q_OS_MAC
	QIcon retIcon = getMacIcon(info);
	if (!retIcon.isNull())
		return retIcon;
#elif defined Q_OS_WIN
	QIcon icon = getWinIcon(info);
	if (!icon.isNull())
		return icon;
#endif
	return QIcon();
}
