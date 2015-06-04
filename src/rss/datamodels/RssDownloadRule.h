#ifndef _RSS_DWONLOAD_RULE_INCLUDED_
#define _RSS_DWONLOAD_RULE_INCLUDED_

#include <QtCore>
#include <xstring>
#include "RssCommon.h"
class RssItem;

class RssDownloadRule
{
	friend QDataStream& operator <<(QDataStream& out, const RssDownloadRule& any);
	friend QDataStream& operator >>(QDataStream& out, RssDownloadRule& any);
public:
	enum DownloadRuleType
	{
		EMPTY = 0,
		SELECT_FILE_RULE = 1,
		DOWNLOAD_RULE = 2
	};
	RssDownloadRule();
	bool UseStaticSavePath() const;

	void setUseStaticSavePath(bool mUseStaticSavePath);

	bool UseGroupFilters() const;

	void setUseGroupFilters(bool mUseGroupFilters);

	QString StaticSavePath() const;

	void setStaticSavePath(const QString& mStaticSavePath);

	QUuid Uid() const;

	QString Name() const;

	void setName(const QString& mName);

	QString Pattern() const;

	void setPattern(const QString& mPattern);

	QRegExp RegExp() const;

	void setRegExp(const QRegExp& mRegExp);

	QRegExp::PatternSyntax PatternType() const;

	void setPatternType(QRegExp::PatternSyntax mPatternType);

	DownloadRuleType RuleType() const;

	void setRuleType(DownloadRuleType mRuleType);

	QSet<QUuid> FeedUids() const;

	void setFeedUids(const QSet<QUuid>& mFeedUids);
	QString validate(bool& ok);
	bool Match(RssFeed* pFeed) const;
	bool Match(RssItem* rssItem) const;
	bool MatchFile(QString fileName);
	bool MatchFeed(QUuid uid);
	void addFeed(QUuid feedUid);
	void removeFeed(QUuid feedUid);
private:
	bool m_useStaticSavePath;
	bool m_useGroupFilters;
	QString m_staticSavePath;
	QUuid m_uid;
	QString m_name;
	QString m_pattern;
	QRegExp m_regExp;
	QRegExp::PatternSyntax m_patternType;
	DownloadRuleType m_ruleType;
	QSet<QUuid> m_feedUids;

protected:
};

Q_DECLARE_METATYPE(RssDownloadRule::DownloadRuleType)

#endif//_RSS_DWONLOAD_RULE_INCLUDED_

