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
#include <stddef.h>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QFileDialog>
#include <QFormLayout>
#include <QIntValidator>
#include <QUrl>
#include "application.h"
#include "messagebox.h"
#include "GroupForFileFiltering.h"
#include "QApplicationSettings.h"
#include "SchedulerTask.h"
#include "Scheduller.h"
#include "SettingsDialog.h"
#include "StyleEngene.h"
#include "TorrentManager.h"
#include "qkeyedit.h"
#include "torrentracker.h"
#include "RconWebService.h"
#include "NotificationSystem.h"
#include "AddRssDwonloadRuleDialog.h"
#include "RssManager.h"
#include <smtp/smtpclient.h>
#include <gui/Utils/UIPropertySetters.h>
#include <gui/Utils/UIPropertyGetters.h>
#include <gui/Utils/ValueGetters.h>
#include <gui/Utils/ValueSetters.h>
#include "torrentracker.h"
#include "RconWebService.h"

SettingsDialog::SettingsDialog(QWidget* parent, int flags)
	: BaseWindow(OnlyCloseButton, NoResize, parent)
	  , m_propertyMapper(new SettingsPropertyMapper(this))
	  , m_pTorrentGroupManager(TorrentGroupsManager::getInstance())
	  , editRssRule(NULL)
	  , deleteRssRule(NULL)
	  , m_filterGroupsHaveChanges(false)
	  , m_rssDownloadRulesHaveChanges(false)
	  , m_schedulerTasksHaveChanges(false)
{
	setupUi(this);
	m_pSettings = QApplicationSettings::getInstance();
	m_pRcon = RconWebService::getInstance();
	m_pTracker = TorrentTracker::getInstance();
	FillDTTab();
	FillRestrictionTab();
	FillFilteringGroups();
	FillGeneralTab();
	FillHDDTab();
	FillWebUITab();
	FillSchedullerTab();
	FillKeyMapTab();
	FillNetworkTab();
	FillRssTab();
	setupCustomWindow();
	QPushButton* applyButton = buttonBox->button(QDialogButtonBox::Apply);

	if (applyButton != NULL)
	{
		applyButton->setEnabled(false);
	}

	setupWindowIcons();
	StyleEngene* style = StyleEngene::getInstance();
	connect(style, SIGNAL(styleChanged()), this, SLOT(setupWindowIcons()));
	connect(m_propertyMapper.get(), SIGNAL(GotChanges()), SLOT(EnableApplyButton()));
	connect(m_propertyMapper.get(), SIGNAL(NoChanges()), SLOT(DisableApplyButton()));
}

void SettingsDialog::chooseAction(QAbstractButton* button)
{
	switch (buttonBox->standardButton(button))
	{
		case QDialogButtonBox::Ok:
			ApplyAndClose();
			break;

		case QDialogButtonBox::Cancel:
			reject();
			break;

		case QDialogButtonBox::Apply:
			ApplySettings();
			break;
	}
}

void SettingsDialog::setupWindowIcons()
{
	StyleEngene* style = StyleEngene::getInstance();
	BaseWindow::setupWindowIcons();
	RunningLabel->setPixmap(style->getIcon("active").pixmap(32, 32));
	listWidget->item(0)->setIcon(style->getIcon("settings_torrent"));
	listWidget->item(1)->setIcon(style->getIcon("settings_limits"));
	listWidget->item(2)->setIcon(style->getIcon("settings_network"));
	listWidget->item(3)->setIcon(style->getIcon("settings_hdd"));
	listWidget->item(4)->setIcon(style->getIcon("settings_filter"));
	listWidget->item(5)->setIcon(style->getIcon("settings_dt"));
	listWidget->item(6)->setIcon(style->getIcon("settings_scheduler"));
	listWidget->item(7)->setIcon(style->getIcon("settings_web_controll"));
	listWidget->item(8)->setIcon(style->getIcon("settings_rss"));
	listWidget->item(9)->setIcon(style->getIcon("settings_hotkeys"));
	QToolButton* prevMonthBtn = calendarWidget->findChild<QToolButton*>("qt_calendar_prevmonth");

	if (prevMonthBtn != NULL)
	{
		prevMonthBtn->setIcon(style->getIcon("calendar_prev"));
	}

	QToolButton* nextMonthBtn = calendarWidget->findChild<QToolButton*>("qt_calendar_nextmonth");

	if (nextMonthBtn != NULL)
	{
		nextMonthBtn->setIcon(style->getIcon("calendar_next"));
	}
}

void SettingsDialog::FillHDDTab()
{
	m_propertyMapper->AddMapping("Torrent", "lock_files", SettingsPropertyMapper::BOOL, lockFilesCheckBox, SettingsPropertyMapper::CHECKBOX);
	m_propertyMapper->AddMapping("Torrent", "cache_size", SettingsPropertyMapper::INT, casheSizeLineEdit, SettingsPropertyMapper::SPINBOX, 0, UIPropertySetters::ChacheSizeSetter,
	                             UIPropertyGetters::CacheSizeGetter);
	m_propertyMapper->AddMapping("Torrent", "disk_io_write_mode", SettingsPropertyMapper::INT, diskIOCasheModeComboBox, SettingsPropertyMapper::COMBOBOX);
	m_propertyMapper->AddMapping("Torrent", "use_disk_read_ahead", SettingsPropertyMapper::BOOL, useDiskReadAheadCheckBox, SettingsPropertyMapper::CHECKBOX);
	m_propertyMapper->AddMapping("Torrent", "allow_reordered_disk_operations", SettingsPropertyMapper::BOOL, alowReorderedOpsCheckBox, SettingsPropertyMapper::CHECKBOX);
	m_propertyMapper->AddMapping("Torrent", "low_prio_disk", SettingsPropertyMapper::BOOL, lowPrioDiskCheckBox, SettingsPropertyMapper::CHECKBOX);
	m_propertyMapper->AddMapping("Torrent", "use_read_cache", SettingsPropertyMapper::BOOL, useReadCasheCheckBox, SettingsPropertyMapper::CHECKBOX);
	m_propertyMapper->AddMapping("Torrent", "file_allocation_mode", SettingsPropertyMapper::INT, diskAllocationComboBox, SettingsPropertyMapper::COMBOBOX);	
}

void SettingsDialog::FillGeneralTab()
{
	m_propertyMapper->AddMapping("TorrentTracker", "enabled", SettingsPropertyMapper::BOOL, trackerGroupBox, SettingsPropertyMapper::CHECKABLE_GROUPBOX, false);
	m_propertyMapper->AddMapping("TorrentTracker", "port", SettingsPropertyMapper::INT, trackerPortEdit, SettingsPropertyMapper::LINE_EDIT, 6996);
	StyleEngene* styleEngine = StyleEngene::getInstance();
	QList<StyleInfo> styleInfos = styleEngine->getAvaliableStyles();
	styleComboBox->clear();

	for (int i = 0; i < styleInfos.size(); i++)
	{
		styleComboBox->addItem(styleInfos[i].DisplayName, styleInfos[i].InternalName);
	}

	m_propertyMapper->AddMapping("System", "Style", SettingsPropertyMapper::INT, styleComboBox, SettingsPropertyMapper::COMBOBOX, "CuteTorrent", NULL, NULL, ValueSetters::StyleValueSetter,
	                             ValueGetters::StyleValueGetter);
	localeComboBox->clear();
	QStringList availableLanguages = Application::availableLanguages();

	for (int i = 0; i < availableLanguages.size(); i++)
	{
		QLocale language = QLocale(availableLanguages[i]);
		localeComboBox->addItem(QLocale::languageToString(language.language()), availableLanguages[i]);
	}

	m_propertyMapper->AddMapping("System", "Lang", SettingsPropertyMapper::INT, localeComboBox, SettingsPropertyMapper::COMBOBOX, "en_US", NULL, NULL, ValueSetters::LanguageValueSetter,
	                             ValueGetters::LanguageValueGetter);
	m_propertyMapper->AddMapping("Window", "close_on_hide", SettingsPropertyMapper::BOOL, closeOnHideCheckBox, SettingsPropertyMapper::CHECKBOX);
	m_propertyMapper->AddMapping("Notifications", "use_notification_sys", SettingsPropertyMapper::BOOL, useNotificationsCheckBox, SettingsPropertyMapper::CHECKABLE_GROUPBOX, true);
	m_propertyMapper->AddMapping("Notifications", "report_tracker_errors", SettingsPropertyMapper::BOOL, showTrackerErrorsCheckBox, SettingsPropertyMapper::CHECKBOX, false);
	m_propertyMapper->AddMapping("Notifications", "report_disk_errors", SettingsPropertyMapper::BOOL, showDiskErrorsCheckBox, SettingsPropertyMapper::CHECKBOX);
	m_propertyMapper->AddMapping("Notifications", "report_rss_errors", SettingsPropertyMapper::BOOL, showRssErrorsCheckBox, SettingsPropertyMapper::CHECKBOX);
	m_propertyMapper->AddMapping("WatchDir", "enabled", SettingsPropertyMapper::BOOL, watchDirEnabledCheckbox, SettingsPropertyMapper::CHECKABLE_GROUPBOX, false);
	m_propertyMapper->AddMapping("WatchDir", "dir_to_watch", SettingsPropertyMapper::STRING, watchDirPathEdit, SettingsPropertyMapper::LINE_EDIT, "");
	m_propertyMapper->AddMapping("WatchDir", "show_doalog_on_new_torrent", SettingsPropertyMapper::BOOL, showDialogRadioButton, SettingsPropertyMapper::RADIOBUTTON, false);
	m_propertyMapper->AddMapping("WatchDir", "use_torrent_filtering", SettingsPropertyMapper::BOOL, usetTorrentFilteringRadioButton, SettingsPropertyMapper::RADIOBUTTON, false);
	m_propertyMapper->AddMapping("WatchDir", "use_static_save_path", SettingsPropertyMapper::BOOL, useStaticPathRadioButton, SettingsPropertyMapper::RADIOBUTTON, false);
	m_propertyMapper->AddMapping("WatchDir", "use_static_save_path", SettingsPropertyMapper::BOOL, useStaticPathRadioButton, SettingsPropertyMapper::RADIOBUTTON, false);
	m_propertyMapper->AddMapping("WatchDir", "static_save_path", SettingsPropertyMapper::STRING, staticSavePathEdit, SettingsPropertyMapper::LINE_EDIT, "");
	m_propertyMapper->AddMapping("System", "open_magent_links", SettingsPropertyMapper::BOOL, magnetAssociationCheckBox, SettingsPropertyMapper::CHECKBOX, QVariant(), NULL, NULL,
	                             ValueSetters::MagnetAssociationValueSetter, ValueGetters::MagnetAssociationValueGetter);
	m_propertyMapper->AddMapping("System", "open_torrent_files", SettingsPropertyMapper::BOOL, asociationCheckBox, SettingsPropertyMapper::CHECKBOX, QVariant(), NULL, NULL,
	                             ValueSetters::TorrentAssociationValueSetter, ValueGetters::TorrentAssociationValueGetter);
	m_propertyMapper->AddMapping("System", "run_on_boot", SettingsPropertyMapper::BOOL, runOnbootCheckBox, SettingsPropertyMapper::CHECKBOX, QVariant(), NULL, NULL, ValueSetters::RunOnBootValueSetter,
	                             ValueGetters::RunOnBootValueGetter);
	m_propertyMapper->AddMapping("Search", "script_debuging_enabled", SettingsPropertyMapper::BOOL, scriptDebugingCheckBox, SettingsPropertyMapper::CHECKBOX, false);
	//OS_SPECIFICK////
#ifdef Q_WS_WIN
	m_propertyMapper->AddMapping("System", "run_on_boot_minimaized", SettingsPropertyMapper::BOOL, startMinimizedCheckBox, SettingsPropertyMapper::CHECKBOX, QVariant(), NULL, NULL,
	                             ValueSetters::RunOnBootMinimizedValueSetter, ValueGetters::RunOnBootMinimizedValueGetter);
	m_propertyMapper->AddMapping("System", "win_explorer_integration_enabled", SettingsPropertyMapper::BOOL, winShelItegrationCheckBox, SettingsPropertyMapper::CHECKBOX, QVariant(), NULL, NULL,
	                             ValueSetters::WindowsShellValueSetter, ValueGetters::WindowsShellValueGetter);
#endif
#ifdef Q_WS_X11
	winShelItegrationCheckBox->setVisible(false);
	startMinimizedCheckBox->setVisible(false);
#endif
	//OS_SPECIFICK////
}

void SettingsDialog::AppendChildren(QTreeWidgetItem* item, QList<TorrentGroup*>& children)
{
	StyleEngene* pStyleEngine = StyleEngene::getInstance();
	for (int i = 0; i < children.length(); i++)
	{
		TorrentGroup* group = children[i];
		m_torrentGroupsToUid.insert(group->uid(), group);
		QTreeWidgetItem* subIitem = new QTreeWidgetItem(QStringList() << group->name());
		subIitem->setIcon(0, pStyleEngine->guessMimeIcon(group->extentions()[0]));
		subIitem->setData(0, Qt::UserRole, QVariant::fromValue(group->uid()));
		item->addChild(subIitem);
		QList<TorrentGroup*> subChildren = group->Children();
		if (subChildren.length() > 0)
		{
			AppendChildren(subIitem, subChildren);
		}
	}
}

void SettingsDialog::FillFilteringGroups()
{
	m_filterGroups.clear();
	auto groups = m_pTorrentGroupManager->GetTorrentGroups();
	for (int i = 0; i < groups.size(); i++)
	{
		m_filterGroups.append(new TorrentGroup(*groups[i]));
	}

	m_torrentGroupsToUid.clear();
	connect(&m_filterGroups, SIGNAL(CollectionChanged(CollectionChangedInfo)), SLOT(onFilteringGroupsChanged()), Qt::UniqueConnection);
	GroupsTreeWidget->clear();
	StyleEngene* pStyleEngine = StyleEngene::getInstance();
	for (int i = 0; i < m_filterGroups.count(); i++)
	{
		TorrentGroup* group = m_filterGroups.at(i);
		QTreeWidgetItem* item = new QTreeWidgetItem(QStringList() << group->name());
		item->setIcon(0, pStyleEngine->guessMimeIcon(group->extentions()[0]));
		item->setData(0, Qt::UserRole, QVariant::fromValue(group->uid()));
		GroupsTreeWidget->addTopLevelItem(item);
		m_torrentGroupsToUid.insert(group->uid(), group);
		QList<TorrentGroup*> children = group->Children();
		if (children.length() > 0)
		{
			AppendChildren(item, children);
		}
	}
	GroupsTreeWidget->expandToDepth(1);
	GroupsTreeWidget->setSelectionMode(QAbstractItemView::ContiguousSelection);
}

void SettingsDialog::FillDTTab()
{
	m_propertyMapper->AddMapping("DT", "Executable", SettingsPropertyMapper::STRING, DTPathEdit, SettingsPropertyMapper::LINE_EDIT);
	m_propertyMapper->AddMapping("DT", "Drive", SettingsPropertyMapper::INT, driveNumberComboBox, SettingsPropertyMapper::COMBOBOX);
	m_propertyMapper->AddMapping("DT", "UseCustomCommand", SettingsPropertyMapper::BOOL, customMoutGroupBox, SettingsPropertyMapper::CHECKABLE_GROUPBOX);
	m_propertyMapper->AddMapping("DT", "CustomtCommand", SettingsPropertyMapper::STRING, customCommandEdit, SettingsPropertyMapper::LINE_EDIT, "-mount dt,%1,\"%2\"");
}

void SettingsDialog::UpdateWebUILaunchButtons()
{
	bool enabled = m_pSettings->valueBool("WebControl", "webui_enabled");
	bool isRunning = m_pRcon->isRunning();
	RunningLabel->setEnabled(isRunning && enabled);
	startRconButton->setEnabled(!isRunning && enabled);
	stopRconButton->setEnabled(isRunning && enabled);
}

void SettingsDialog::FillWebUITab()
{
	m_propertyMapper->AddMapping("WebControl", "webui_enabled", SettingsPropertyMapper::BOOL, webUIGroupBox, SettingsPropertyMapper::CHECKABLE_GROUPBOX, false);
	m_propertyMapper->AddMapping("WebControl", "webui_login", SettingsPropertyMapper::STRING, loginLineEdit, SettingsPropertyMapper::LINE_EDIT);
	m_propertyMapper->AddMapping("WebControl", "webui_password", SettingsPropertyMapper::STRING, passwordLineEdit, SettingsPropertyMapper::LINE_EDIT);
	m_propertyMapper->AddMapping("WebControl", "port", SettingsPropertyMapper::STRING, webPortLineEdit, SettingsPropertyMapper::LINE_EDIT, 8080);
	m_propertyMapper->AddMapping("WebControl", "enable_upnp", SettingsPropertyMapper::BOOL, upnpCheckBox, SettingsPropertyMapper::CHECKBOX, false);
	m_propertyMapper->AddMapping("WebControl", "enable_loggin", SettingsPropertyMapper::BOOL, webUILogginGroupBox, SettingsPropertyMapper::CHECKABLE_GROUPBOX, false);
	m_propertyMapper->AddMapping("WebControl", "log_name", SettingsPropertyMapper::STRING, logLineEdit, SettingsPropertyMapper::LINE_EDIT);
	m_propertyMapper->AddMapping("WebControl", "enable_ipfilter", SettingsPropertyMapper::BOOL, IPFilterGroupBox, SettingsPropertyMapper::CHECKABLE_GROUPBOX, false);
	m_propertyMapper->AddMapping("WebControl", "ipfilter", SettingsPropertyMapper::STRING, ipFilterTextEdit, SettingsPropertyMapper::TEXT_EDIT);
	UpdateWebUILaunchButtons();
}

void SettingsDialog::showSelectedGroup(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
	if (current == NULL)
	{
		return;
	}
	QUuid groupUid = current->data(0, Qt::UserRole).value<QUuid>();
	if (!groupUid.isNull() && m_torrentGroupsToUid.contains(groupUid))
	{
		TorrentGroup* currentGroup = m_torrentGroupsToUid[groupUid];
		newGroupNameEdit->setText(currentGroup->name());
		extensionsEdit->setText(currentGroup->extentions().join("|"));
		groupSavePathEdit->setText(currentGroup->savePath());
	}
}

SettingsDialog::~SettingsDialog()
{
	qDeleteAll(m_downloadRulesCopy);
	m_downloadRulesCopy.clear();
}

void SettingsDialog::ApplySettings()
{
	m_propertyMapper->ApplyChanges();
	UpdateWebUILaunchButtons();
	ApplySettingsToSession();

	if (m_filterGroupsHaveChanges)
	{
		m_pTorrentGroupManager->RefreshFilteringGroups(m_filterGroups);
	}

	NotificationSystem::getInstance()->UpdateNotificationSettings();

	if (m_rssDownloadRulesHaveChanges)
	{
		ApplyRssDownloadRulles();
	}

	if (m_schedulerTasksHaveChanges)
	{
		m_pSettings->SaveSchedullerQueue(m_schedulerTasks);
	}
}

void SettingsDialog::ApplySettingsToSession()
{
	TorrentManagerPtr pTorrentManager = TorrentManager::getInstance();
	settings_pack current = pTorrentManager->readSettings();
	pTorrentManager->updateSettings(current);
	pTorrentManager->RefreshExternalPeerSettings();
	pTorrentManager->updateMaxConnectionsPerTorrent();
}

void SettingsDialog::addGroup()
{
	QString name = newGroupNameEdit->text();

	if (name.isEmpty())
	{
		CustomMessageBox::warning(tr("STR_SETTINGS"),
		                          tr("ERROR_GROUP_NAME_NOT_SET"));
		return;
	}

	QStringList extentions = extensionsEdit->toPlainText().split('|');

	if (extentions.isEmpty())
	{
		CustomMessageBox::warning(tr("STR_SETTINGS"),
		                          tr("ERROR_NO_EXTENSIONS"));
		return;
	}

	QString dir = groupSavePathEdit->text();

	if (dir.isEmpty())
	{
		CustomMessageBox::warning( tr("STR_SETTINGS"),
		                          tr("ERROR_NO_PATH"));
		return;
	}

	if (!QDir(dir).exists())
	{
		CustomMessageBox::warning( tr("STR_SETTINGS"),
		                          tr("ERROR_PATH_NOT_EXISTS"));
		return;
	}
	QTreeWidgetItem* currentItem = GroupsTreeWidget->currentItem();
	TorrentGroup* newGroup = new TorrentGroup(name, extentions, dir);
	QTreeWidgetItem* child = new QTreeWidgetItem(QStringList() << newGroup->name());
	StyleEngene* pStyleEngine = StyleEngene::getInstance();
	child->setIcon(0, pStyleEngine->guessMimeIcon(extentions[0]));
	child->setData(0, Qt::UserRole, QVariant::fromValue(newGroup->uid()));

	if (currentItem != NULL)
	{
		QUuid groupUid = currentItem->data(0, Qt::UserRole).value<QUuid>();
		if (!groupUid.isNull())
		{
			if (m_torrentGroupsToUid.contains(groupUid))
			{
				TorrentGroup* group = m_torrentGroupsToUid[groupUid];
				group->addChild(newGroup);
				m_torrentGroupsToUid.insert(newGroup->uid(), newGroup);
			}
			else
			{
				return;
			}

			currentItem->addChild(child);
		}
	}
	else
	{
		m_filterGroups.append(newGroup);
		m_torrentGroupsToUid.insert(newGroup->uid(), newGroup);
		GroupsTreeWidget->addTopLevelItem(child);
	}

	onFilteringGroupsChanged();
}

void SettingsDialog::removeGroup()
{
	QList<QTreeWidgetItem*> selectedItems = GroupsTreeWidget->selectedItems();
	if (selectedItems.length() > 0)
	{
		for (int i = 0; i < selectedItems.length(); i++)
		{
			QTreeWidgetItem* item = selectedItems[i];
			QUuid groupUid = item->data(0, Qt::UserRole).value<QUuid>();
			QUuid parentUid;
			QTreeWidgetItem* parent = item->parent();
			if (parent != NULL)
			{
				parentUid = parent->data(0, Qt::UserRole).value<QUuid>();
			}
			if (!groupUid.isNull())
			{
				if (parentUid.isNull())
				{
					if (m_torrentGroupsToUid.contains(groupUid))
					{
						TorrentGroup* topLevelGroup = m_torrentGroupsToUid[groupUid];
						m_filterGroups.removeAll(topLevelGroup);
					}
				}
				else
				{
					if (m_torrentGroupsToUid.contains(parentUid))
					{
						m_torrentGroupsToUid[parentUid]->removeChild(m_torrentGroupsToUid[groupUid]);
					}
				}
				delete item;
			}
		}
		onFilteringGroupsChanged();
	}
}

void SettingsDialog::editGroup()
{
	QString name = newGroupNameEdit->text();

	if (name.isEmpty())
	{
		CustomMessageBox::warning( tr("STR_SETTINGS"),
		                          tr("ERROR_GROUP_NAME_NOT_SET"));
		return;
	}

	QStringList extentions = extensionsEdit->toPlainText().split('|');

	if (extentions.isEmpty())
	{
		CustomMessageBox::warning( tr("STR_SETTINGS"),
		                          tr("ERROR_NO_EXTENSIONS"));
		return;
	}

	QString dir = groupSavePathEdit->text();

	if (dir.isEmpty())
	{
		CustomMessageBox::warning( tr("STR_SETTINGS"),
		                          tr("ERROR_NO_PATH"));
		return;
	}

	if (!QDir(dir).exists())
	{
		CustomMessageBox::warning( tr("STR_SETTINGS"),
		                          tr("ERROR_PATH_NOT_EXISTS"));
		return;
	}
	QTreeWidgetItem* currentItem = GroupsTreeWidget->currentItem();

	if (currentItem != NULL)
	{
		QUuid groupUid = currentItem->data(0, Qt::UserRole).value<QUuid>();
		if (!groupUid.isNull())
		{
			if (m_torrentGroupsToUid.contains(groupUid))
			{
				TorrentGroup* group = m_torrentGroupsToUid[groupUid];
				group->setName(name);
				group->setExtentions(extentions);
				group->setSavePath(dir);
				currentItem->setText(0, name);
				StyleEngene* pStyleEngine = StyleEngene::getInstance();
				currentItem->setIcon(0, pStyleEngine->guessMimeIcon(extentions[0]));
			}
		}
		onFilteringGroupsChanged();
	}
}

void SettingsDialog::browseSavepathGroup()
{
	QString lastDir = m_pSettings->valueString("System", "LastSaveTorrentDir");
	QString savaPathForCurrentGroup = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
	                                                                    lastDir,
	                                                                    QFileDialog::ShowDirsOnly
	                                                                    | QFileDialog::DontResolveSymlinks);

	if (!savaPathForCurrentGroup.isEmpty())
	{
		savaPathForCurrentGroup.append(QDir::separator());
		groupSavePathEdit->setText(QDir::toNativeSeparators(savaPathForCurrentGroup));
	}
}

void SettingsDialog::browseDTPath()
{
	QString lastDir = m_pSettings->valueString("System", "LastSaveTorrentDir");
	QString DTPath = QFileDialog::getOpenFileName(this,
	                                              tr("WHERE_DT"), lastDir, tr("DaemonTools Lite (DTLite.exe);;DaemonTools PRO (DTAgent.exe);;Any File (*.*)"));
	DTPathEdit->setText(QDir::toNativeSeparators(DTPath));
}

void SettingsDialog::ApplyAndClose()
{
	ApplySettings();
	close();
}


void SettingsDialog::DeleteTask()
{
	int index = tasksComboBox->currentIndex();
	SchedulerTask currentTask = tasksComboBox->itemData(index).value<SchedulerTask>();
	tasksComboBox->removeItem(index);
	m_schedulerTasks.removeAt(index);
}

void SettingsDialog::AddTask()
{
	SchedulerTask::TaskType type = SchedulerTask::UNKNOWN;
	int limit = -1;
	QString name = taskNameLineEdit->text();
	QVariant limitVal;

	if (pauseAllRadioButton->isChecked())
	{
		type = SchedulerTask::PAUSE_ALL;
	}
	else if (startAllRadioButton->isChecked())
	{
		type = SchedulerTask::START_ALL;
	}
	else if (limitDlRadioButton->isChecked())
	{
		type = SchedulerTask::LIMIT_DOWNLOAD;
		limit = dlLimitEdit->text().toInt();
	}
	else if (limitUlRadioButton->isChecked())
	{
		type = SchedulerTask::LIMIT_UPLOAD;
		limit = ulLimitEdit->text().toInt();
	}

	if (limit > 0)
	{
		limitVal = qVariantFromValue(limit);
	}

	if (SchedulerTask::UNKNOWN == type)
	{
		CustomMessageBox::warning( tr("ERROR_SRT"), tr("SCHEDULLER_UNKNOWN_TYPE"));
		return;
	}

	if (name.length() == 0)
	{
		CustomMessageBox::warning( tr("ERROR_SRT"), tr("SCHEDULLER_NO_NAME"));
		return;
	}

	SchedulerTask newTask(name, type, limitVal, beginDateTimeEdit->dateTime());
	m_schedulerTasks.push_back(newTask);
	tasksComboBox->addItem(newTask.name(), qVariantFromValue(newTask));
	emit tasksChanged();
}

void SettingsDialog::FillSchedullerTab()
{
	connect(calendarWidget, SIGNAL(clicked(QDate)), beginDateTimeEdit, SLOT(setDate(QDate)));
	beginDateTimeEdit->setDateTime(QDateTime::currentDateTime().addSecs(120));
	connect(tasksComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateSchedullerTab(int)));
	m_schedulerTasks.append(m_pSettings->GetSchedullerQueue());
	connect(&m_schedulerTasks, SIGNAL(CollectionChanged(CollectionChangedInfo)), SLOT(onSchedulerTaskChanged()));

	for (int i = 0; i < m_schedulerTasks.count(); i++)
	{
		tasksComboBox->addItem(m_schedulerTasks.at(i).name(), qVariantFromValue(m_schedulerTasks.at(i)));
	}

	QString currentLanguage = Application::currentLocale();
	calendarWidget->setLocale(QLocale(currentLanguage));
	Scheduller* scheduller = Scheduller::getInstance();
	connect(this, SIGNAL(tasksChanged()), scheduller, SLOT(UpdateTasks()));
	Scheduller::freeInstance();
}

void SettingsDialog::FillNetworkTab()
{
	QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
	QString portRange = "(6553[0-5]|655[0-2]\\d|65[0-4]\\d{2}|6[0-4]\\d{3}|[1-5]\\d{4}|[1-9]\\d{0,3})";
	QRegExp ipRegex("^" + ipRange
		+ "\\." + ipRange
		+ "\\." + ipRange
		+ "\\." + ipRange + "\\:" + portRange + "$");
	QRegExpValidator* ipValidator = new QRegExpValidator(ipRegex, this);
	proxyHostEdit->setValidator(ipValidator);
	QRegExp portRegex("^" + portRange + "$");
	QRegExpValidator* portValidator = new QRegExpValidator(portRegex, this);
	portEdit->setValidator(portValidator);
	m_propertyMapper->AddMapping("Torrent", "random_listen_port", SettingsPropertyMapper::BOOL, useRandomPortCheckBox, SettingsPropertyMapper::CHECKBOX);
	m_propertyMapper->AddMapping("Torrent", "listen_port", SettingsPropertyMapper::INT, portEdit, SettingsPropertyMapper::LINE_EDIT);
	m_propertyMapper->AddMapping("Torrent", "useProxy", SettingsPropertyMapper::BOOL, proxyGroupBox, SettingsPropertyMapper::CHECKABLE_GROUPBOX);
	m_propertyMapper->AddMapping("Torrent", "proxy_hostname", SettingsPropertyMapper::STRING, proxyHostEdit, SettingsPropertyMapper::LINE_EDIT);
	m_propertyMapper->AddMapping("Torrent", "proxy_port", SettingsPropertyMapper::STRING, proxyPortEdit, SettingsPropertyMapper::LINE_EDIT);
	m_propertyMapper->AddMapping("Torrent", "proxy_username", SettingsPropertyMapper::STRING, proxyUsernameEdit, SettingsPropertyMapper::LINE_EDIT);
	m_propertyMapper->AddMapping("Torrent", "proxy_password", SettingsPropertyMapper::STRING, proxyPwdEdit, SettingsPropertyMapper::LINE_EDIT);
	m_propertyMapper->AddMapping("Torrent", "in_enc_policy", SettingsPropertyMapper::INT, inEncPolicyComboBox, SettingsPropertyMapper::COMBOBOX);
	m_propertyMapper->AddMapping("Torrent", "out_enc_policy", SettingsPropertyMapper::INT, outEncPolicyComboBox, SettingsPropertyMapper::COMBOBOX);
	m_propertyMapper->AddMapping("Torrent", "allowed_enc_level", SettingsPropertyMapper::INT, encLevelComboBox, SettingsPropertyMapper::COMBOBOX);
	m_propertyMapper->AddMapping("Torrent", "prefer_rc4", SettingsPropertyMapper::BOOL, preferFullEncCheckBox, SettingsPropertyMapper::CHECKBOX);
	m_propertyMapper->AddMapping("Torrent", "use_port_forwarding", SettingsPropertyMapper::BOOL, portMappingsCheckBox, SettingsPropertyMapper::CHECKBOX);
}

void SettingsDialog::FillKeyMapTab()
{
	QMap<QString, QVariant> keyMappings = m_pSettings->getGroupValues("KeyMap");
	QWidget* mainWindow = NULL;
	QWidgetList widgets = qApp->topLevelWidgets();
	for (int i = 0; i < widgets.size(); i++)
	{
		if (widgets[i]->objectName() == "CustomWindow")
		{
			mainWindow = widgets[i];
			break;
		}
	}
	qDeleteAll(keyMapContainer->findChildren<QGroupBox*>());
	QLayout* origLayout = keyMapContainer->layout();
	if (origLayout != NULL)
		qDeleteAll(origLayout->findChildren<QVBoxLayout*>());
	QHBoxLayout* layout = origLayout ? static_cast<QHBoxLayout*>(origLayout) : new QHBoxLayout(keyMapContainer);

	QVBoxLayout *rightRow = new QVBoxLayout(keyMapContainer), *leftRow = new QVBoxLayout(keyMapContainer);
	layout->addLayout(rightRow);
	layout->addLayout(leftRow);
	QMap<QString, QMap<QString, QString>> grouppedKeyMap;
	int index = 0;
	QStringList itemsToDelete;
	for (QMap<QString, QVariant>::iterator i = keyMappings.begin();
	     i != keyMappings.end(); ++i , index++)
	{
		QString propertyName = i.key();
		QAction* action = mainWindow->findChild<QAction*>(propertyName);
		if (action != NULL)
		{
			QString groupName = i.key().split('_')[1];
			grouppedKeyMap[groupName][i.key()] = i.value().toString();
		}
		else
		{
			itemsToDelete << propertyName;
		}
	}

	for (int i = 0; i < itemsToDelete.size(); i++)
	{
		keyMappings.remove(itemsToDelete[i]);
	}
	int size = keyMappings.size() + grouppedKeyMap.size();
	int rightColumns = size / 2;

	if (size & 1)
	{
		rightColumns++;
	}

	int leftColumns = size / 2;
	int rightIndex = 0, leftIndex = 0;
	QStringList keys = grouppedKeyMap.keys();

	for (int i = 0; i < keys.length(); i++)
	{
		for (int j = 0; j < keys.length(); j++)
		{
			if (keys[i] != keys[j])
			{
				if (grouppedKeyMap[keys[i]].size() > grouppedKeyMap[keys[j]].size())
				{
					qSwap(keys[i], keys[j]);
				}
			}
		}
	}

	foreach(QString groupName, keys)
		{
			QMap<QString, QString> keyMap = grouppedKeyMap[groupName];
			QGroupBox* _groupBox = new QGroupBox(keyMapContainer);
			_groupBox->setTitle(trUtf8(groupName.toUpper().toUtf8()));
			QFormLayout* groupLayout = new QFormLayout(_groupBox);

			for (QMap<QString, QString>::iterator i = keyMap.begin();
			     i != keyMap.end(); ++i , ++index)
			{
				QString propertyName = i.key();
				QAction* action = mainWindow->findChild<QAction*>(propertyName);
				if (action != NULL)
				{
					QKeyEdit* keyEdit = new QKeyEdit(keyMapContainer);
					keyEdit->setObjectName(propertyName);
					m_propertyMapper->AddMapping("KeyMap", propertyName, SettingsPropertyMapper::STRING, keyEdit, SettingsPropertyMapper::LINE_EDIT);
					groupLayout->addRow(action->text(), keyEdit);
				}
			}


			_groupBox->setLayout(groupLayout);
			int keMapSize = keyMap.size() + 1;
			qDebug() << "Group:" << groupName << "Row Count:" << groupLayout->rowCount() << "KeysSize" << keMapSize << "Left:" << leftColumns << "Right:" << rightColumns;
			if (rightColumns - keMapSize >= 0)
			{
				rightColumns -= keMapSize;
				rightRow->addWidget(_groupBox/*, rightIndex, 0, keMapSize , 1*/);
				rightIndex += keMapSize;
			}
			else
			{
				leftColumns -= keMapSize;
				leftRow->addWidget(_groupBox/*, leftIndex, 1, keMapSize, 1*/);
				leftIndex += keMapSize;
			}
		}

	keyMapContainer->setLayout(layout);
}

void SettingsDialog::UpdateSchedullerTab(int index)
{
	SchedulerTask currentTask = tasksComboBox->itemData(index).value<SchedulerTask>();
	taskNameLineEdit->setText(currentTask.name());
	beginDateTimeEdit->setDateTime(currentTask.startTime());

	switch (currentTask.type())
	{
		case SchedulerTask::START_ALL:
			startAllRadioButton->setChecked(true);
			break;

		case SchedulerTask::PAUSE_ALL:
			pauseAllRadioButton->setChecked(true);
			break;

		case SchedulerTask::LIMIT_UPLOAD:
			limitUlRadioButton->setChecked(true);
			ulLimitEdit->setText(QString::number(currentTask.limit()));
			break;

		case SchedulerTask::LIMIT_DOWNLOAD:
			limitDlRadioButton->setChecked(true);
			dlLimitEdit->setText(QString::number(currentTask.limit()));
			break;
	}
}

void SettingsDialog::StartRcon()
{
	m_pRcon->Start();
	bool isRunning = m_pRcon->isRunning();
	RunningLabel->setEnabled(isRunning);
	startRconButton->setEnabled(!isRunning);
	stopRconButton->setEnabled(isRunning);
}

void SettingsDialog::StopRcon()
{
	m_pRcon->Stop();
	bool isRunning = m_pRcon->isRunning();
	RunningLabel->setEnabled(isRunning);
	startRconButton->setEnabled(!isRunning);
	stopRconButton->setEnabled(isRunning);
}

void SettingsDialog::changeEvent(QEvent* event)
{
	if (event->type() == QEvent::LanguageChange)
	{
		retranslateUi(this);
		FillFilteringGroups();
		FillGeneralTab();
		FillKeyMapTab();
		m_propertyMapper->ResetToCurrentValues();
		QString choosenLanguage = m_pSettings->valueString("System", "Lang");
		calendarWidget->setLocale(QLocale(choosenLanguage));
		editRssRule->setText(tr("ACTION_SETTINGS_EDIT_RSS_RULE"));
		deleteRssRule->setText(tr("ACTION_SETTINGS_DELETE_RSS_RULE"));
	}
}

void SettingsDialog::OpenWebUI()
{
	if (m_pRcon->isRunning())
	{
		QDesktopServices::openUrl(QUrl("http://localhost:" + webPortLineEdit->text() + "/"));
	}
}

QPushButton* SettingsDialog::getCloseBtn()
{
	return pbClose;
}

QWidget* SettingsDialog::getTitleBar()
{
	return titleBar;
}

QWidget* SettingsDialog::centralWidget()
{
	return m_centralWidget;
}

QLabel* SettingsDialog::getTitleLabel()
{
	return LTitle;
}

QLabel* SettingsDialog::getTitleIcon()
{
	return tbMenu;
}

void SettingsDialog::FillRestrictionTab()
{
	m_propertyMapper->AddMapping("Torrent", "upload_rate_limit", SettingsPropertyMapper::INT, uploadLimitEdit, SettingsPropertyMapper::SPINBOX, 0, UIPropertySetters::SpinboxKBSetter,
	                             UIPropertyGetters::SpinboxKBGetter);
	m_propertyMapper->AddMapping("Torrent", "download_rate_limit", SettingsPropertyMapper::INT, downloadLimitEdit, SettingsPropertyMapper::SPINBOX, 0, UIPropertySetters::SpinboxKBSetter,
	                             UIPropertyGetters::SpinboxKBGetter);
	m_propertyMapper->AddMapping("Torrent", "dht_upload_rate_limit", SettingsPropertyMapper::INT, dhtLimitEdit, SettingsPropertyMapper::SPINBOX, 0, UIPropertySetters::SpinboxKBSetter,
	                             UIPropertyGetters::SpinboxKBGetter);
	m_propertyMapper->AddMapping("Torrent", "active_limit", SettingsPropertyMapper::INT, activeLimitEdit, SettingsPropertyMapper::SPINBOX);
	m_propertyMapper->AddMapping("Torrent", "active_seeds", SettingsPropertyMapper::INT, activeSeedLimitEdit, SettingsPropertyMapper::SPINBOX);
	m_propertyMapper->AddMapping("Torrent", "active_downloads", SettingsPropertyMapper::INT, activeDownloadLimitEdit, SettingsPropertyMapper::SPINBOX);
	m_propertyMapper->AddMapping("Torrent", "use_dht", SettingsPropertyMapper::BOOL, useDHTCheckBox, SettingsPropertyMapper::CHECKBOX);
	m_propertyMapper->AddMapping("Torrent", "use_lsd", SettingsPropertyMapper::BOOL, useLSDCheckBox, SettingsPropertyMapper::CHECKBOX);
	m_propertyMapper->AddMapping("Torrent", "use_pex", SettingsPropertyMapper::BOOL, usePExCheckBox, SettingsPropertyMapper::CHECKBOX);
	m_propertyMapper->AddMapping("Torrent", "ignore_limits_on_local_network", SettingsPropertyMapper::BOOL, limitLocalConnectionCheckBox, SettingsPropertyMapper::CHECKABLE_GROUPBOX);
	m_propertyMapper->AddMapping("Torrent", "local_upload_rate_limit", SettingsPropertyMapper::INT, localUploadLimitEdit, SettingsPropertyMapper::SPINBOX, 0, UIPropertySetters::SpinboxKBSetter,
	                             UIPropertyGetters::SpinboxKBGetter);
	m_propertyMapper->AddMapping("Torrent", "local_download_rate_limit", SettingsPropertyMapper::INT, localDownloadLimitEdit, SettingsPropertyMapper::SPINBOX, 0, UIPropertySetters::SpinboxKBSetter,
	                             UIPropertyGetters::SpinboxKBGetter);
	m_propertyMapper->AddMapping("Torrent", "rate_limit_utp", SettingsPropertyMapper::BOOL, limitUtpCheckBox, SettingsPropertyMapper::CHECKBOX);
	m_propertyMapper->AddMapping("Torrent", "seed_time_limit", SettingsPropertyMapper::INT, seedTimeLimitEdit, SettingsPropertyMapper::TIME_EDIT, 0, UIPropertySetters::TimeFromIntSetter,
	                             UIPropertyGetters::IntFromTimeGetter);
	m_propertyMapper->AddMapping("Torrent", "share_ratio_limit", SettingsPropertyMapper::DOUBLE, seedGlobalRatioEdit, SettingsPropertyMapper::DOUBLE_SPINBOX);
	m_propertyMapper->AddMapping("Torrent", "max_connections_per_torrent", SettingsPropertyMapper::INT, maxConnectionsPerTorrentEdit, SettingsPropertyMapper::SPINBOX, 50);
	m_propertyMapper->AddMapping("Torrent", "connections_limit", SettingsPropertyMapper::INT, maxConnectionsEdit, SettingsPropertyMapper::SPINBOX);
}

void SettingsDialog::updateRulesWidget(ObservableList<RssDownloadRule*>& downloadRules)
{
	rssRulesListWidget->clear();

	for (int i = 0; i < downloadRules.size(); i++)
	{
		RssDownloadRule* rssDownloadRule = downloadRules.at(i);
		QListWidgetItem* listWidgetItem = new QListWidgetItem(rssDownloadRule->Name());
		listWidgetItem->setData(Qt::UserRole, qVariantFromValue(rssDownloadRule->Uid()));
		rssRulesListWidget->addItem(listWidgetItem);
	}
}

void SettingsDialog::FillRssTab()
{
	RssManagerPtr pRssManager = RssManager::getInstance();
	QList<RssDownloadRule*> downloadRules = pRssManager->downloadRules();
	qDeleteAll(m_downloadRulesCopy);
	m_downloadRulesCopy.clear();

	for (int i = 0; i < downloadRules.size(); i++)
	{
		RssDownloadRule* rssDownloadRule = downloadRules.at(i);
		m_downloadRulesCopy.append(new RssDownloadRule(*rssDownloadRule));
	}

	StyleEngene* pStyleEngine = StyleEngene::getInstance();
	updateRulesWidget(m_downloadRulesCopy);

	if (editRssRule == NULL)
	{
		editRssRule = new QAction(pStyleEngine->getIcon("move_folder"), tr("ACTION_SETTINGS_EDIT_RSS_RULE"), this);
		connect(editRssRule, SIGNAL(triggered()), SLOT(onEditRssRule()));
		rssRulesListWidget->addAction(editRssRule);
	}

	if (deleteRssRule == NULL)
	{
		deleteRssRule = new QAction(pStyleEngine->getIcon("delete"), tr("ACTION_SETTINGS_DELETE_RSS_RULE"), this);
		connect(deleteRssRule, SIGNAL(triggered()), SLOT(onDeleteRssRule()));
		rssRulesListWidget->addAction(deleteRssRule);
	}

	m_propertyMapper->AddMapping("rss", "default_refresh_rate", SettingsPropertyMapper::INT, rssRefrashRateEdit, SettingsPropertyMapper::TIME_EDIT, 30 * 60, UIPropertySetters::TimeFromIntSetter,
	                             UIPropertyGetters::IntFromTimeGetter);
	m_propertyMapper->AddMapping("rss", "auto_download_emeail_notify", SettingsPropertyMapper::BOOL, autosrtEmailNotificationCheckBox, SettingsPropertyMapper::CHECKABLE_GROUPBOX);
	m_propertyMapper->AddMapping("rss", "smtp_host", SettingsPropertyMapper::STRING, rssSmtpServerEdit, SettingsPropertyMapper::LINE_EDIT);
	m_propertyMapper->AddMapping("rss", "smtp_port", SettingsPropertyMapper::STRING, rssSmtpPortEdit, SettingsPropertyMapper::LINE_EDIT);
	char const* smtpConnectionTypes[] =
		{
			QT_TR_NOOP("RSS_TCP_CONNECTION"),
			QT_TR_NOOP("RSS_SSL_CONNECTION"),
			QT_TR_NOOP("RSS_TLS_CONNECTION"),
		};

	for (int i = SmtpClient::TcpConnection; i <= SmtpClient::TlsConnection; i++)
	{
		rssSmtpConnTypeCombobox->addItem(tr(smtpConnectionTypes[i]), i);
	}

	m_propertyMapper->AddMapping("rss", "smtp_conn_type", SettingsPropertyMapper::INT, rssSmtpConnTypeCombobox, SettingsPropertyMapper::COMBOBOX);
	char const* smtpAuthTypes[] =
		{
			QT_TR_NOOP("RSS_PLAIN_AUTH"),
			QT_TR_NOOP("RSS_LOGIN_AUTH"),
		};

	for (int i = SmtpClient::AuthPlain; i <= SmtpClient::AuthLogin; i++)
	{
		rssAuthTypeComboBox->addItem(tr(smtpAuthTypes[i]), i);
	}

	m_propertyMapper->AddMapping("rss", "smtp_auth_type", SettingsPropertyMapper::INT, rssAuthTypeComboBox, SettingsPropertyMapper::COMBOBOX);
	m_propertyMapper->AddMapping("rss", "smtp_user", SettingsPropertyMapper::STRING, rssSmtpLoginEdit, SettingsPropertyMapper::LINE_EDIT);
	m_propertyMapper->AddMapping("rss", "smtp_password", SettingsPropertyMapper::STRING, rssSmtpPasswordEdit, SettingsPropertyMapper::LINE_EDIT, QVariant(), NULL, NULL,
	                             ValueSetters::SettingsEncryptedValueSetter, ValueGetters::SettingsEncryptedValueGetter);
	m_propertyMapper->AddMapping("rss", "rss_send_to", SettingsPropertyMapper::STRING, rssRecepientEmailEdit, SettingsPropertyMapper::LINE_EDIT);
	connect(&m_downloadRulesCopy, SIGNAL(CollectionChanged(CollectionChangedInfo)), SLOT(onRssDownloadRuleesChanged()));
	connect(&m_deletedRules, SIGNAL(CollectionChanged(CollectionChangedInfo)), SLOT(onRssDownloadRuleesChanged()));
}

void SettingsDialog::ApplyRssDownloadRulles()
{
	RssManagerPtr pRssManager = RssManager::getInstance();

	for (int i = 0; i < m_downloadRulesCopy.size(); i++)
	{
		pRssManager->updateDownloadRule(new RssDownloadRule(*m_downloadRulesCopy.at(i)));
	}

	for (int i = 0; i < m_deletedRules.size(); i++)
	{
		pRssManager->removeDownloadRule(m_deletedRules.at(i));
	}
}


void SettingsDialog::NeverCallMe()
{
	char const* msgs[] =
		{
			QT_TRANSLATE_NOOP("SettingsDialog", "TRAY"),
			QT_TRANSLATE_NOOP("SettingsDialog", "TRACKER"),
			QT_TRANSLATE_NOOP("SettingsDialog", "TORRENTLIST"),
			QT_TRANSLATE_NOOP("SettingsDialog", "TOOLBAR"),
			QT_TRANSLATE_NOOP("SettingsDialog", "TORRENTTOOLBAR"),
			QT_TRANSLATE_NOOP("SettingsDialog", "TORRENTQUEUE"),
			QT_TRANSLATE_NOOP("SettingsDialog", "TORRENTPRIORITY"),
			QT_TRANSLATE_NOOP("SettingsDialog", "RSSTOOLBAR"),
			QT_TRANSLATE_NOOP("SettingsDialog", "SEARCHTOOLBAR"),
			QT_TRANSLATE_NOOP("SettingsDialog", "SEARCHLIST"),
			QT_TRANSLATE_NOOP("SettingsDialog", "RSSLIST"),
			QT_TRANSLATE_NOOP("SettingsDialog", "PEER"),
			QT_TRANSLATE_NOOP("SettingsDialog", "MENU"),
			QT_TRANSLATE_NOOP("SettingsDialog", "MENUFILE"),
			QT_TRANSLATE_NOOP("SettingsDialog", "MENUVIEW"),
			QT_TRANSLATE_NOOP("SettingsDialog", "MENUHELP"),
			QT_TRANSLATE_NOOP("SettingsDialog", "INFOHASH"),
			QT_TRANSLATE_NOOP("SettingsDialog", "TORRENTDESCRIPTION"),
			QT_TRANSLATE_NOOP("SettingsDialog", "FILETAB"),
			QT_TRANSLATE_NOOP("SettingsDialog", "FILEPRIORITY"),
			QT_TRANSLATE_NOOP("SettingsDialog", "TOOLS"),
			QT_TRANSLATE_NOOP("SettingsDialog", "AUTOSHUTDOWN"),
		};
}

void SettingsDialog::UpdateApplyButtonState()
{
	if (m_rssDownloadRulesHaveChanges || m_filterGroupsHaveChanges || m_rssDownloadRulesHaveChanges || m_propertyMapper->HasChanges())
	{
		EnableApplyButton();
	}
	else
	{
		DisableApplyButton();
	}
}

RssDownloadRule* SettingsDialog::findRule(QUuid uid)
{
	int len = m_downloadRulesCopy.size();

	for (int i = 0; i < len; i++)
	{
		if (m_downloadRulesCopy[i]->Uid() == uid)
		{
			return m_downloadRulesCopy[i];
		}
	}

	return NULL;
}

void SettingsDialog::addRssRule()
{
	boost::scoped_ptr<AddRssDwonloadRuleDialog> pDialog(new AddRssDwonloadRuleDialog);

	if (pDialog->exec() == Accepted)
	{
		RssDownloadRule* rule = pDialog->getFinalRule();
		m_downloadRulesCopy.append(rule);
		updateRulesWidget(m_downloadRulesCopy);
	}
}

void SettingsDialog::onEditRssRule()
{
	if (rssRulesListWidget->currentRow() != -1)
	{
		QListWidgetItem* currentItem = rssRulesListWidget->currentItem();
		QUuid uid = currentItem->data(Qt::UserRole).value<QUuid>();
		RssDownloadRule* downloadRule = findRule(uid);
		bool ok;
		downloadRule->validate(ok);

		if (ok)
		{
			boost::scoped_ptr<AddRssDwonloadRuleDialog> pDialog(new AddRssDwonloadRuleDialog(this, AddRssDwonloadRuleDialog::EDIT));
			pDialog->setDownloadRule(downloadRule);

			if (pDialog->exec() == Accepted)
			{
				RssDownloadRule* rule = pDialog->getFinalRule();
				boost::scoped_ptr<RssDownloadRule> rule2del(findRule(rule->Uid()));
				int ruleIndex = m_downloadRulesCopy.indexOf(rule2del.get());
				m_downloadRulesCopy.replace(ruleIndex, rule);
				updateRulesWidget(m_downloadRulesCopy);
			}
		}
	}
}

void SettingsDialog::onDeleteRssRule()
{
	if (rssRulesListWidget->currentRow() != -1)
	{
		QList<QListWidgetItem*> selectedItems = rssRulesListWidget->selectedItems();

		foreach(QListWidgetItem* currentItem, selectedItems)
			{
				QUuid uid = currentItem->data(Qt::UserRole).value<QUuid>();
				boost::scoped_ptr<RssDownloadRule> rule2del(findRule(uid));
				m_downloadRulesCopy.removeOne(rule2del.get());
				m_deletedRules.append(uid);
			}

		updateRulesWidget(m_downloadRulesCopy);
	}
}

void SettingsDialog::onExportRssRules()
{
	QString savePath = QFileDialog::getSaveFileName(this, tr("RSS_RULES_EXPORT"), QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation), tr("CT_RSS_RULE_LIST (*.cdrl)"));

	if (savePath.isEmpty())
	{
		return;
	}

	RssManagerPtr pRssManager = RssManager::getInstance();
	QList<RssDownloadRule*> rssDownloadRules = pRssManager->downloadRules();
	QFile rulesFile(savePath);

	if (rulesFile.open(QFile::WriteOnly))
	{
		foreach(RssDownloadRule* rule, rssDownloadRules)
			{
				QSet<QUuid> feedUids = rule->FeedUids();
				QSet<QUuid>::Iterator begin = feedUids.begin()
					, end = feedUids.end();
				int size = feedUids.size();
				QString serializedRule = rule->Uid().toString() % "|" % QUrl::toPercentEncoding(rule->Name()) % "|" % QString::number(rule->RuleType()) % "|" % QString::number(
					rule->PatternType()) % "|" % QUrl::toPercentEncoding(rule->Pattern()) % "|" % QString::number(rule->UseGroupFilters()) % "|" % QString::number(
					rule->UseStaticSavePath()) % "|" % rule->StaticSavePath() % "|" % QString::number(size);

				for (QSet<QUuid>::Iterator i = begin; i != end; ++i)
				{
					serializedRule.append("|" % (*i).toString());
				}

				serializedRule.append("\n");
				rulesFile.write(serializedRule.toUtf8());
			}
	}
	else
	{
		CustomMessageBox::critical( tr("ERROR"), tr("ERROR_WRITING_RSS_RULE_LIST: %1").arg(rulesFile.errorString()));
	}
}

void SettingsDialog::onImportRssRules()
{
	QString openPath = QFileDialog::getOpenFileName(this, tr("RSS_RULES_IMPORT"), QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation), tr("CT_RSS_RULE_LIST (*.cdrl)"));

	if (openPath.isEmpty())
	{
		return;
	}

	QFile rulesFile(openPath);

	if (rulesFile.open(QFile::ReadOnly))
	{
		RssManagerPtr pRssManager = RssManager::getInstance();

		while (!rulesFile.atEnd())
		{
			QByteArray lineData = rulesFile.readLine();
			QString line = QString::fromUtf8(lineData);
			QStringList parts = line.split("|");
			int size = parts.size();
			int index = 0;

			if (size > NUMBER_OF_FEEDS)
			{
				RssDownloadRule* pRule = new RssDownloadRule();

				while (index < size)
				{
					switch (index)
					{
						case UID:
						{
							QUuid uid = QUuid(parts[index]);
							pRule->setUuid(uid);
							break;
						}

						case NAME:
						{
							pRule->setName(QUrl::fromPercentEncoding(parts[index].toUtf8()));
							break;
						}

						case RULE_TYPE:
						{
							pRule->setRuleType(static_cast<RssDownloadRule::DownloadRuleType>(parts[index].toInt()));
							break;
						}

						case SEARCH_TYPE:
						{
							pRule->setPatternType(static_cast<QRegExp::PatternSyntax>(parts[index].toInt()));
							break;
						}

						case SEARCH_STR:
						{
							pRule->setPattern(QUrl::fromPercentEncoding(parts[index].toUtf8()));
							break;
						}

						case USE_GROUP_FILTERS:
						{
							pRule->setUseGroupFilters(parts[index].toInt() > 0);
							break;
						}

						case USE_STATIC_SAVE_PATH:
						{
							pRule->setUseStaticSavePath(parts[index].toInt() > 0);
							break;
						}

						case STATIC_SAVE_PATH:
						{
							pRule->setStaticSavePath(parts[index]);
							break;
						}

						case NUMBER_OF_FEEDS:
						{
							int numFeeds = parts[index].toInt();
							QSet<QUuid> feedUids;
							feedUids.reserve(numFeeds);

							for (int i = NUMBER_OF_FEEDS + 1; i <= NUMBER_OF_FEEDS + numFeeds; i++)
							{
								feedUids.insert(QUuid(parts[i]));
							}

							pRule->setFeedUids(feedUids);
							break;
						}
					}

					index++;
				}

				bool ok;
				QString error = pRule->validate(ok);

				if (ok)
				{
					m_downloadRulesCopy.append(pRule);
					m_deletedRules.removeAll(pRule->Uuid());
				}
				else
				{
					CustomMessageBox::critical( "ERROR", tr("PARSED_RULE_IS_NOT_VALID:\n %1\nERROR: %2").arg(line, error));
				}
			}
		}

		updateRulesWidget(m_downloadRulesCopy);
	}
	else
	{
		CustomMessageBox::critical( tr("ERROR"), tr("ERROR_READING_RSS_RULE_LIST: %1").arg(rulesFile.errorString()));
	}
}

void SettingsDialog::EnableApplyButton()
{
	QPushButton* applyButton = buttonBox->button(QDialogButtonBox::Apply);

	if (applyButton != NULL)
	{
		applyButton->setEnabled(true);
	}
}

void SettingsDialog::DisableApplyButton()
{
	QPushButton* applyButton = buttonBox->button(QDialogButtonBox::Apply);

	if (applyButton != NULL)
	{
		applyButton->setEnabled(false);
	}
}

void SettingsDialog::onBrowseWatchDir()
{
	QString watchDir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
	                                                     QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation),
	                                                     QFileDialog::ShowDirsOnly
	                                                     | QFileDialog::DontResolveSymlinks);

	if (!watchDir.isEmpty())
	{
		watchDir.append("\\");
		watchDirPathEdit->setText(QDir::toNativeSeparators(watchDir));
	}
}

void SettingsDialog::onBrowseWatchStaticPath()
{
	QString watchDirStaticPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
	                                                               QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation),
	                                                               QFileDialog::ShowDirsOnly
	                                                               | QFileDialog::DontResolveSymlinks);

	if (!watchDirStaticPath.isEmpty())
	{
		watchDirStaticPath.append("\\");
		staticSavePathEdit->setText(QDir::toNativeSeparators(watchDirStaticPath));
	}
}

void SettingsDialog::onFilteringGroupsChanged()
{
	m_filterGroupsHaveChanges = false;
	QList<TorrentGroup*> originalGroups = m_pTorrentGroupManager->GetTorrentGroups();

	if (originalGroups.size() != m_filterGroups.size())
	{
		m_filterGroupsHaveChanges = true;
	}
	else
	{
		for (int i = 0; i < m_filterGroups.size(); i++)
		{
			if (*originalGroups[i] != *m_filterGroups[i])
			{
				m_filterGroupsHaveChanges = true;
				break;
			}
		}
	}

	UpdateApplyButtonState();
}

void SettingsDialog::onRssDownloadRuleesChanged()
{
	m_rssDownloadRulesHaveChanges = false;

	if (!m_deletedRules.isEmpty())
	{
		m_rssDownloadRulesHaveChanges = true;
	}
	else
	{
		RssManagerPtr pRssManager = RssManager::getInstance();
		QList<RssDownloadRule*> originalRules = pRssManager->downloadRules();

		if (originalRules.size() != m_downloadRulesCopy.size())
		{
			m_rssDownloadRulesHaveChanges = true;
		}
		else
		{
			int size = originalRules.size();

			for (int i = 0; i < size; i++)
			{
				if (*originalRules[i] != *m_downloadRulesCopy[i])
				{
					m_rssDownloadRulesHaveChanges = true;
					break;
				}
			}
		}
	}

	UpdateApplyButtonState();
}

void SettingsDialog::onSchedulerTaskChanged()
{
	m_schedulerTasksHaveChanges = false;
	QList<SchedulerTask> originalSchedulerTasks = m_pSettings->GetSchedullerQueue();

	if (originalSchedulerTasks.size() != m_schedulerTasks.size())
	{
		m_schedulerTasksHaveChanges = true;
	}
	else
	{
		int size = originalSchedulerTasks.size();

		for (int i = 0; i < size; i++)
		{
			if (originalSchedulerTasks[i] != m_schedulerTasks[i])
			{
				m_schedulerTasksHaveChanges = true;
				break;
			}
		}
	}

	UpdateApplyButtonState();
}

