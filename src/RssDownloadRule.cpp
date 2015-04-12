#include "RssDownloadRule.h"
#include "RssFeed.h"
#include "RssItem.h"
RssDownloadRule::RssDownloadRule() : m_name(""), m_pattern(""), m_patternType(QRegExp::RegExp2), m_ruleType(EMPTY)
{
	m_uid = QUuid::createUuid();
}


QString RssDownloadRule::validate(bool& ok)
{
	ok = false;
	if (m_name.isEmpty())
	{
		return qApp->translate("AddRssDwonloadRuleDialog", "RULE_NAME_CAN_NOT_BE_EMPTY");
	}
	if (m_ruleType == EMPTY)
	{
		return qApp->translate("AddRssDwonloadRuleDialog", "RULE_TYPE_CAN_NOT_BE_EMPTY");
	}
	if (m_patternType == QRegExp::RegExp2)
	{
		return qApp->translate("AddRssDwonloadRuleDialog", "RULE_SEARCH_TYPE_CAN_NOT_BE_EMPTY");
	}
	if (m_pattern.isEmpty())
	{
		return qApp->translate("AddRssDwonloadRuleDialog", "RULE_SEARCH_STRING_CAN_NOT_BE_EMPTY");
	}
	QRegExp regexp(m_pattern, Qt::CaseInsensitive, m_patternType);
	if (!regexp.isValid())
	{
		return regexp.errorString();
	}

	if (m_useGroupFilters == m_useStaticSavePath && m_useStaticSavePath == false)
	{
		return qApp->translate("AddRssDwonloadRuleDialog", "DONT_KNOW_WHERE_TO_SAVE");
	}
	if (m_useGroupFilters == m_useStaticSavePath && m_useStaticSavePath == true)
	{
		return qApp->translate("AddRssDwonloadRuleDialog", "CHOSE_ONE_STATICK_SAVE_PATH_OR_GROUPS_FILTERING");
	}
	if (m_useStaticSavePath)
	{
		if (m_staticSavePath.isEmpty())
		{
			return qApp->translate("AddRssDwonloadRuleDialog", "STATIC_SAVE_PATH_CAN_NOT_BE_EMPTY");
		}
	}

	if (m_feedUids.isEmpty())
	{
		return qApp->translate("AddRssDwonloadRuleDialog", "YOU_SHOULD_SELECT_ATLEAST_ONE_FEED");
	}
	
	
	ok = true;
	m_regExp = regexp;
	return qApp->translate("AddRssDwonloadRuleDialog", "RULE_IS_VALID");
}

bool RssDownloadRule::Match(RssFeed* pFeed) const
{
	if (pFeed != nullptr)
	{
		return m_feedUids.contains(pFeed->uid());
	}
	return false;
}

bool RssDownloadRule::Match(RssItem* rssItem) const
{
	
	if (m_regExp.isValid() && rssItem!= nullptr)
	{
		if (!rssItem->title().isEmpty())
		{
			if (m_regExp.indexIn(rssItem->title()) != -1)
			{
				return true;
			}
		}
	}
	return false;
}

bool RssDownloadRule::MatchFile(QString fileName)
{
	if (m_ruleType == SELECT_FILE_RULE)
	{
		return m_regExp.indexIn(fileName) != -1;
	}
	return false;
}

bool RssDownloadRule::MatchFeed(QUuid uid)
{
	return m_feedUids.contains(uid);
}

void RssDownloadRule::addFeed(QUuid feedUid)
{
	m_feedUids.insert(feedUid);
}

void RssDownloadRule::removeFeed(QUuid feedUid)
{
	m_feedUids.remove(feedUid);
}

bool RssDownloadRule::UseStaticSavePath() const
{
	return m_useStaticSavePath;
}

void RssDownloadRule::setUseStaticSavePath(bool mUseStaticSavePath)
{
	m_useStaticSavePath = mUseStaticSavePath;
}

bool RssDownloadRule::UseGroupFilters() const
{
	return m_useGroupFilters;
}

void RssDownloadRule::setUseGroupFilters(bool mUseGroupFilters)
{
	m_useGroupFilters = mUseGroupFilters;
}

QString RssDownloadRule::StaticSavePath() const
{
	return m_staticSavePath;
}

void RssDownloadRule::setStaticSavePath(const QString& mStaticSavePath)
{
	m_staticSavePath = mStaticSavePath;
}

QUuid RssDownloadRule::Uid() const
{
	return m_uid;
}

QString RssDownloadRule::Name() const
{
	return m_name;
}

void RssDownloadRule::setName(const QString& mName)
{
	m_name = mName;
}

QString RssDownloadRule::Pattern() const
{
	return m_pattern;
}

void RssDownloadRule::setPattern(const QString& mPattern)
{
	m_pattern = mPattern;
}

QRegExp RssDownloadRule::RegExp() const
{
	return m_regExp;
}

void RssDownloadRule::setRegExp(const QRegExp& mRegExp)
{
	m_regExp = mRegExp;
}

QRegExp::PatternSyntax RssDownloadRule::PatternType() const
{
	return m_patternType;
}

void RssDownloadRule::setPatternType(QRegExp::PatternSyntax mPatternType)
{
	m_patternType = mPatternType;
}

RssDownloadRule::DownloadRuleType RssDownloadRule::RuleType() const
{
	return m_ruleType;
}

void RssDownloadRule::setRuleType(DownloadRuleType mRuleType)
{
	m_ruleType = mRuleType;
}

QSet<QUuid> RssDownloadRule::FeedUids() const
{
	return m_feedUids;
}

void RssDownloadRule::setFeedUids(const QSet<QUuid>& mFeedUids)
{
	m_feedUids = mFeedUids;
}

QDataStream& operator <<(QDataStream& out, const RssDownloadRule& any)
{
	out << any.m_name;
	out << any.m_pattern;
	out << static_cast<int>(any.m_patternType);
	out << any.m_regExp;
	out << static_cast<int>(any.m_ruleType);
	out << any.m_uid;
	out << any.m_useStaticSavePath;
	out << any.m_useGroupFilters;
	out << any.m_staticSavePath;
	out << any.m_feedUids;
	return out;
}
QDataStream& operator >>(QDataStream& out, RssDownloadRule& any)
{
	out >> any.m_name;
	out >> any.m_pattern;
	int iPatternType;
	out >> iPatternType;
	any.m_patternType = static_cast<QRegExp::PatternSyntax>(iPatternType);
	out >> any.m_regExp;
	int iRuleType;
	out >> iRuleType;
	any.m_ruleType = static_cast<RssDownloadRule::DownloadRuleType>(iRuleType);
	out >> any.m_uid;
	out >> any.m_useStaticSavePath;
	out >> any.m_useGroupFilters;
	out >> any.m_staticSavePath;
	out >> any.m_feedUids;
	return out;
}
