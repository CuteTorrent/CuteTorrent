#pragma once
#include <QUuid>
#include <QStringList>

class TorrentGroup
{
	QUuid m_uid;
	QString m_savePath;
	QString m_name;
	QStringList m_extentions;
	QList<TorrentGroup*> m_children;
public:
	friend bool operator==(const TorrentGroup& lhs, const TorrentGroup& rhs);


	friend bool operator!=(const TorrentGroup& lhs, const TorrentGroup& rhs);
	friend QDataStream& operator <<(QDataStream& out, const TorrentGroup& any);
	friend QDataStream& operator >>(QDataStream& out, TorrentGroup& any);
	TorrentGroup(QString name, QStringList extentions, QString savePath);

	TorrentGroup(const TorrentGroup& other);

	TorrentGroup()
	{
	};

	~TorrentGroup();
	void setUid(const QUuid& m_uid);

	void setName(const QString& m_name);

	void setExtentions(const QStringList& m_extentions);

	void addExtetion(QString extention);

	void removeExtetion(QString extention);
	QString savePath() const;

	void setSavePath(const QString& m_save_path);

	QUuid uid() const;

	QString name() const;

	QStringList extentions() const;

	void addChild(TorrentGroup* group);

	void removeChild(TorrentGroup* group);
	bool contains(TorrentGroup* group);
	bool contains(QUuid groupUid);
	QList<TorrentGroup*> Children() const;
	bool isNull();
};

