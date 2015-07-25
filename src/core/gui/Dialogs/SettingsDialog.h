/*
CuteTorrent BitTorrent Client with dht support, userfriendly interface
and some additional features which make it more convenient.
Copyright (C) 2012 Ruslan Fedoseyenko

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _SETTINGS_DLG_H
#define _SETTINGS_DLG_H


#include "CustomWindow.h"
#include "GroupForFileFiltering.h"
#include "QApplicationSettings.h"
#include "torrentracker.h"
#include "ui_SettingsDialog.h"
#include "RconWebService.h"
#include "RssDownloadRule.h"
#include "Utils/SettingsPropertyMapper.h"
#include <collections/ObservableList.h>
class QApplicationSettings;
class RconWebService;
class TorrentTracker;

class SettingsDialog : public BaseWindow<QDialog>, Ui::SettingsDialog
{
	Q_OBJECT
protected:
	void changeEvent(QEvent* event) override;
	virtual QPushButton* getCloseBtn() override;
	virtual QWidget* getTitleBar() override;
	virtual QWidget* centralWidget() override;
	virtual QLabel* getTitleLabel() override;
	virtual QLabel* getTitleIcon() override;

private:
	enum RssDownloadRuleSerializtionParts
	{
		UID,
		NAME,
		RULE_TYPE,
		SEARCH_TYPE,
		SEARCH_STR,
		USE_GROUP_FILTERS,
		USE_STATIC_SAVE_PATH,
		STATIC_SAVE_PATH,
		NUMBER_OF_FEEDS
	};
	boost::scoped_ptr<SettingsPropertyMapper> m_propertyMapper;
	ObservableList<RssDownloadRule*> m_downloadRulesCopy;
	ObservableList<QUuid> m_deletedRules;
	QApplicationSettingsPtr m_pSettings;
	ObservableList<GroupForFileFiltering> m_filterGroups;
	ObservableList<SchedulerTask> m_schedulerTasks;
	TorrentTrackerPtr m_pTracker;
	RconWebServicePtr m_pRcon;
	QAction* editRssRule, *deleteRssRule;
	bool m_filterGroupsHaveChanges;
	bool m_schedulerTasksHaveChanges;
	bool m_rssDownloadRulesHaveChanges;
	void FillFilteringGroups();
	void FillGeneralTab();
	void FillHDDTab();
	void FillDTTab();
	void UpdateWebUILaunchButtons();
	void FillWebUITab();
	void FillKeyMapTab();
	void FillSchedullerTab();
	void FillNetworkTab();
	void FillRestrictionTab();
	void updateRulesWidget(ObservableList<RssDownloadRule*>& downloadRules);
	void FillRssTab();
	void ApplyRssDownloadRulles();
	void NeverCallMe();
	void UpdateApplyButtonState();
	RssDownloadRule* findRule(QUuid uid);
public:
	SettingsDialog(QWidget* parent = NULL, int flags = 0);
	~SettingsDialog();
	void ApplySettingsToSession();

signals:
	void needRetranslate();
	void tasksChanged();
private slots:
	void setupWindowIcons() override;
	void chooseAction(QAbstractButton* button);
	void ApplyAndClose();
	void ApplySettings();
	void addGroup();
	void removeGroup();
	void browseSavepathGroup();
	void browseDTPath();
	void showSelectedGroup(int);
	void AddTask();
	void DeleteTask();
	void UpdateSchedullerTab(int index);
	void StartRcon();
	void StopRcon();
	void OpenWebUI();
	void addRssRule();
	void onEditRssRule();
	void onDeleteRssRule();
	void onExportRssRules();
	void onImportRssRules();
	void EnableApplyButton();
	void DisableApplyButton();
	void onBrowseWatchDir();
	void onBrowseWatchStaticPath();
	void onFilteringGroupsChanged();
	void onRssDownloadRuleesChanged();
	void onSchedulerTaskChanged();
};

#endif // !_SETTINGS_DLG_H

