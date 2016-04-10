#ifndef _ADD_RSS_DL_RULE_DLG_INCLUDED_
#define _ADD_RSS_DL_RULE_DLG_INCLUDED_

#include "CustomWindow.h"
#include "ui_AddDownloadRuleDialog.h"
#include <QUuid>
#include "RssDownloadRule.h"
class QRssFilterModel;

class AddRssDwonloadRuleDialog : public BaseWindow<QDialog>, Ui::AddRssDownloadRuleDialog
{
	Q_OBJECT
public:
	enum DiaologMode
	{
		EDIT,
		CREATE
	};

	AddRssDwonloadRuleDialog(QWidget* parent = NULL, DiaologMode mode = CREATE);
	RssDownloadRule* getFinalRule();

	void setDownloadRule(RssDownloadRule* rule);
private slots:
	void onChange(bool directCall = false);
	void onFeedListItemChanged(QListWidgetItem*);
	void onApplyRule();
	void onCancel();
	void onUpdateRuleTypeHint();
	void onBrowseStaticPath();
private:
	bool m_inited;
	QRssFilterModel* m_pRssFilterModel;
	QStatusBar* m_pStatusBar;
	RssDownloadRule* m_currentRule;
	QWidget* centralWidget() override;
	QPushButton* getCloseBtn() override;
	QWidget* getTitleBar() override;
	QLabel* getTitleIcon() override;
	QLabel* getTitleLabel() override;
	void setupTitle(DiaologMode mode);
	void setupStatusBar();
	void FillRssFeedsList();
	void FillComboboxes();
	void setupTreeView();
	void FillDataFromDownloadRule();
};

Q_DECLARE_METATYPE(QUuid)
Q_DECLARE_METATYPE(QRegExp::PatternSyntax)
#endif//_ADD_RSS_DL_RULE_DLG_INCLUDED_


