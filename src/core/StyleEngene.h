#ifndef STYLEENGENE_H
#define STYLEENGENE_H
#include <QtGui>
#include <QCache>
class QuaZip;
struct StyleInfo
{
	QString DisplayName;
	QString InternalName;

	bool operator ==(StyleInfo& other) const
	{
		return DisplayName == other.DisplayName && InternalName == other.InternalName;
	}
};

class StyleEngene : public QObject
{
	Q_OBJECT
protected:
	StyleEngene(QObject* parrent = NULL);
	~StyleEngene();
	static StyleEngene* instance;
	static const char* EnumStrings[];
	static QIcon fallback;

public:
	enum
	{
		DISK = 0,
		DOCUMENT = 1,
		PICTURE = 2,
		VIDEO = 3,
		ARCHIVE = 4,
		AUDIO = 5,
		APP = 6,
		TYPE_COUNT = 7
	};

	static QSet<QString> suffixes[TYPE_COUNT];
	static QIcon fileIcons[TYPE_COUNT];
	static StyleEngene* getInstance();
	QList<StyleInfo> getAvaliableStyles() const;
	void setStyle(QString internalName);
	QIcon guessMimeIcon(const QString& suffix);
	QString gessMimeIconType(const QString& suffix);
	StyleInfo getCuurentStyle() const;
	QIcon getIcon(QString name);
private:
	void init();
	static void initFileSuffixes();
	void initIcons();
	void loadStyleInfo(QString path);
	void loadStyleSheet(QString path) const;
	QCache<QString, QIcon> m_iconCache;
	QMap<QString, QString> iconNamesMap;
	QList<StyleInfo> _avaliableStyles;
	QMutex m_mutex;
	struct InternalStyleInfo
	{
		StyleInfo styleInfo;
		QString imageDir;
		QString rootPath;
		QuaZip* iconsPack;
	};

	InternalStyleInfo _currentStyle;
	QMap<QString, InternalStyleInfo> _styleMap;
	signals:
	void styleChanged();
};

#endif // STYLEENGENE_H


