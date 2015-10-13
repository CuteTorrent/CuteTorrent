#include "TorrentGroup.h"
#include <QDebug>
TorrentGroup::TorrentGroup(QString name, QStringList extentions, QString savePath)
{
	m_uid = QUuid::createUuid();
	setName(name);
	setExtentions(extentions);
	setSavePath(savePath);
}

TorrentGroup::~TorrentGroup()
{
	qDeleteAll(m_children);
}

void TorrentGroup::setUid(const QUuid& m_uid)
{
	this->m_uid = m_uid;
}

void TorrentGroup::setName(const QString& m_name)
{
	this->m_name = m_name;
}

void TorrentGroup::setExtentions(const QStringList& m_extentions)
{
	this->m_extentions = m_extentions;
}

void TorrentGroup::addExtetion(QString extention)
{
	m_extentions.append(extention);
}

void TorrentGroup::removeExtetion(QString extention)
{
	m_extentions.removeAll(extention);
}

QString TorrentGroup::savePath() const
{
	return m_savePath;
}

void TorrentGroup::setSavePath(const QString& m_save_path)
{
	m_savePath = m_save_path;
}

QUuid TorrentGroup::uid() const
{
	return m_uid;
}

QString TorrentGroup::name() const
{
	return m_name;
}

QStringList TorrentGroup::extentions() const
{
	return m_extentions;
}

void TorrentGroup::addChild(TorrentGroup* group)
{
	m_children.append(group);
}

void TorrentGroup::removeChild(TorrentGroup* group)
{
	m_children.removeAll(group);
}

bool TorrentGroup::contains(TorrentGroup* group)
{
	return contains(group->uid());
}

bool TorrentGroup::contains(QUuid groupUid)
{
	for (size_t i = 0; i < m_children.size(); i++)
	{
		if (m_children[i]->uid() == groupUid || m_children[i]->contains(groupUid))
		{
			return true;
		}
	}
	return false;
}

QList<TorrentGroup*> TorrentGroup::Children() const
{
	return m_children;
}

bool TorrentGroup::isNull()
{
	return m_uid.isNull();
}

bool operator==(const TorrentGroup& lhs, const TorrentGroup& rhs)
{
	bool propertiesEquals = lhs.m_uid == rhs.m_uid
		&& lhs.m_name == rhs.m_name
		&& lhs.m_extentions == rhs.m_extentions;
	bool listEqauls = true;
	if (lhs.m_children.size() != rhs.m_children.size())
	{
		listEqauls = false;
	}
	else
	{
		for (int i = 0; i < lhs.m_children.size(); i++)
		{
			if (*lhs.m_children[i] != *rhs.m_children[i])
			{
				listEqauls = false;
				break;
			}
		}
	}
	qDebug() << "Group" << lhs.m_name << "Children count:" << lhs.m_children.size() << (propertiesEquals && listEqauls ? "==" : "!=") << rhs.m_name << "Children count:" << rhs.m_children.size();
	return propertiesEquals && listEqauls;
}

bool operator!=(const TorrentGroup& lhs, const TorrentGroup& rhs)
{
	return !(lhs == rhs);
}

QDataStream& operator<<(QDataStream& out, const TorrentGroup& any)
{
	out << any.m_uid;
	out << any.m_name;
	out << any.m_savePath;
	out << any.m_extentions;
	out << any.m_children.size();
	for (int i = 0; i < any.m_children.size(); i++)
	{
		out << *any.m_children[i];
	}
	return out;
}

QDataStream& operator>>(QDataStream& out, TorrentGroup& any)
{
	out >> any.m_uid;
	out >> any.m_name;
	out >> any.m_savePath;
	out >> any.m_extentions;
	int childrenSize = 0;
	out >> childrenSize;
	for (int i = 0; i < childrenSize; i++)
	{
		TorrentGroup* pGroup = new TorrentGroup();
		out >> *pGroup;
		any.addChild(pGroup);
	}
	return out;
}