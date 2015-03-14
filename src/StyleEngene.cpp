#include "StyleEngene.h"
#include <QSettings>
#include <QApplication>

StyleEngene* StyleEngene::instance = NULL;
const char* StyleEngene::EnumStrings[] = { "iso", "doc", "picture", "movie", "archive", "audio", "app" };
QIcon StyleEngene::fallback = QIcon();
QSet<QString> StyleEngene::suffixes[TYPE_COUNT];
QIcon StyleEngene::fileIcons[TYPE_COUNT];
StyleEngene::StyleEngene(QObject* parent) : QObject(parent)
{
	init();
}

StyleEngene::~StyleEngene()
{
}

StyleEngene* StyleEngene::getInstance()
{
	if(instance == NULL)
	{
		instance = new StyleEngene();
	}

	return instance;
}

QList<StyleInfo> StyleEngene::getAvaliableStyles()
{
	return _avaliableStyles;
}

void StyleEngene::setStyle(QString internalName)
{
	if(_styleMap.contains(internalName))
	{
		_currentStyle = _styleMap[internalName];
		m_iconCache.clear();
	}
	else
	{
		qDebug() << "No style found";
		return;
	}

	loadStyleSheet(_currentStyle.rootPath + "/style.qss");
	initIcons();
	emit styleChanged();
}

QIcon StyleEngene::guessMimeIcon(const QString& suffix, QString& type)
{
	// qDebug() << "StyleEngene::guessMimeIcon" << suffix;
	QString lSuffix = suffix.toLower();

	if(fileIcons[0].isNull())
	{
		initIcons();
	}

	if(!suffix.isEmpty())
	{
		for(int i = 0; i < TYPE_COUNT; ++i)
		{
			if(suffixes[i].contains(lSuffix))
			{
				type = EnumStrings[i];
				return fileIcons[i];
			}
		}
	}

	return fallback;
}

StyleInfo StyleEngene::getCuurentStyle()
{
	return _currentStyle.styleInfo;
}

QIcon StyleEngene::getIcon(QString name)
{
	if(m_iconCache.contains(name))
	{
		return *m_iconCache[name];
	}

	QString iconsRoot = _currentStyle.rootPath + "/icons/";

	if(iconNamesMap.contains(name))
	{
		QIcon* icon = new QIcon(iconsRoot + iconNamesMap[name]);
		m_iconCache.insert(name, icon);
		return *icon;
	}
	else
	{
		qDebug() << "No icon found for " << name;
		return QIcon();
	}
}

void StyleEngene::init()
{
	qDebug() << "StyleEngene::init";
	/*  _avaliableStyles.clear();
	  _styleMap.clear();*/
	QString rootPath = QApplication::applicationDirPath() + "/styles/";
	QDir rootDir = QDir(rootPath);

	if(rootDir.exists())
	{
		QStringList styleDirs = rootDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

		foreach(QString styleDir, styleDirs)
		{
			qDebug() << "loading style from" << rootPath + styleDir;
			loadStyleInfo(rootPath + styleDir);
		}
	}
	else
	{
		qDebug() << "NO_STYLE_DIR_FOUND";
	}
}

void StyleEngene::initIcons()
{
	QString confPath = _currentStyle.rootPath + QDir::separator() + "style.ini";
	QSettings* styleSettings = new QSettings(confPath, QSettings::IniFormat);
	styleSettings->beginGroup("Icons");
	iconNamesMap.clear();
	QStringList keys = styleSettings->childKeys();

	foreach(QString  key, keys)
	{
		iconNamesMap.insert(key, styleSettings->value(key).toString());
	}

	styleSettings->endGroup();
	delete styleSettings;

	if(iconNamesMap.isEmpty())
	{
		return;
	}

	fallback = getIcon("folder");
	const char* disk_types[] =
	{
		"mdx", "mds", "mdf", "iso", "b5t", "b6t", "bwt", "ccd", "cdi",
		"nrg", "pdi", "isz"
	};

	for(int i = 0, n = sizeof(disk_types) / sizeof(disk_types[0]); i < n; ++i)
	{
		suffixes[DISK] << QString::fromLatin1(disk_types[i]);
	}

	fileIcons[DISK] = getIcon("iso");
	const char* doc_types[] =
	{
		"abw", "csv", "doc", "dvi", "htm", "html", "ini", "log", "odp",
		"ods", "odt", "pdf", "ppt", "ps",  "rtf", "tex", "txt", "xml"
	};

	for(int i = 0, n = sizeof(doc_types) / sizeof(doc_types[0]); i < n; ++i)
	{
		suffixes[DOCUMENT] << QString::fromLatin1(doc_types[i]);
	}

	fileIcons[DOCUMENT] = getIcon("doc") ;
	const char* pic_types[] =
	{
		"bmp", "gif", "jpg", "jpeg", "pcx", "png", "psd", "ras", "tga", "ico", "tiff"
	};

	for(int i = 0, n = sizeof(pic_types) / sizeof(pic_types[0]); i < n; ++i)
	{
		suffixes[PICTURE] << QString::fromLatin1(pic_types[i]);
	}

	fileIcons[PICTURE]  = getIcon("picture") ;
	const char* vid_types[] =
	{
		"3gp", "asf", "avi", "mov", "mpeg", "mpg", "mp4", "mkv", "mov",
		"ogm", "ogv", "qt", "rm", "wmv" , "m2ts"
	};

	for(int i = 0, n = sizeof(vid_types) / sizeof(vid_types[0]); i < n; ++i)
	{
		suffixes[VIDEO] << QString::fromLatin1(vid_types[i]);
	}

	fileIcons[VIDEO] = getIcon("movie") ;
	const char* arc_types[] =
	{
		"7z", "ace", "bz2", "cbz", "gz", "gzip", "lzma", "rar", "sft", "tar", "zip"
	};

	for(int i = 0, n = sizeof(arc_types) / sizeof(arc_types[0]); i < n; ++i)
	{
		suffixes[ARCHIVE] << QString::fromLatin1(arc_types[i]);
	}

	fileIcons[ARCHIVE]  = getIcon("archive") ;
	const char* aud_types[] =
	{
		"aac", "ac3", "aiff", "ape", "au", "flac", "m3u", "m4a", "mid", "midi", "mp2",
		"mp3", "mpc", "nsf", "oga", "ogg", "ra", "ram", "shn", "voc", "wav", "wma"
	};

	for(int i = 0, n = sizeof(aud_types) / sizeof(aud_types[0]); i < n; ++i)
	{
		suffixes[AUDIO] << QString::fromLatin1(aud_types[i]);
	}

	fileIcons[AUDIO] = getIcon("audio") ;
	const char* exe_types[] = { "bat", "cmd", "com", "exe" };

	for(int i = 0, n = sizeof(exe_types) / sizeof(exe_types[0]); i < n; ++i)
	{
		suffixes[APP] << QString::fromLatin1(exe_types[i]);
	}

	fileIcons[APP] = QIcon::fromTheme("application-x-executable", fallback);
}

void StyleEngene::loadStyleSheet(QString path)
{
	QFile file(path);

	if(file.open(QFile::ReadOnly))
	{
		QString relativePath = _currentStyle.imageDir;
		qDebug() << "ImageDir=" << relativePath;
		//   qDebug() << "Stylesheet  dump:";
		QString styleSheet = QString(file.readAll()).replace("$[STYLE_DIR]", relativePath);
		//  qDebug() << styleSheet;
		static_cast<QApplication*>(QApplication::instance())->setStyleSheet(styleSheet);
		file.close();
	}
}

void StyleEngene::loadStyleInfo(QString path)
{
	if(QDir(path).exists())
	{
		QString confPath = path + QDir::separator() + "style.ini";
		StyleInfo* newStyleInfo = new StyleInfo;
		QSettings* styleSettings = new QSettings(confPath, QSettings::IniFormat);
		styleSettings->beginGroup("Style");
		newStyleInfo->DisplayName = styleSettings->value("DisplayName").toString();
		newStyleInfo->InternalName = styleSettings->value("InternalName").toString();
		InternalStyleInfo* internalStyleInfo = new InternalStyleInfo;
		internalStyleInfo->styleInfo = *newStyleInfo;
		internalStyleInfo->imageDir = path + "/" + styleSettings->value("ImageDir").toString();
		internalStyleInfo->rootPath = path;
		styleSettings->endGroup();
		delete styleSettings;
		_avaliableStyles.append(*newStyleInfo);
		_styleMap.insert(newStyleInfo->InternalName, *internalStyleInfo);
	}
}
