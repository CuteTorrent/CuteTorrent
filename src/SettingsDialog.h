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


#include <QDialog>
#include <QMouseEvent>
#include <QStackedWidget>

#include "CustomWindow.h"
#include "GroupForFileFiltering.h"
#include "QApplicationSettings.h"
#include "searchitem.h"
#include "tracker/torrentracker.h"
#include "ui_SettingsDialog.h"
#include "webControll/RconWebService.h"

class QApplicationSettings;
class RconWebService;
class TorrentTracker;

class SettingsDialog : public BaseWindow<QDialog>, private Ui::SettingsDialog
{
	Q_OBJECT
protected:
	void changeEvent(QEvent* event);
	virtual QPushButton* getCloseBtn() override;
	virtual QWidget* getTitleBar() override;
	virtual QWidget* centralWidget() override;
	virtual QLabel* getTitleLabel() override;
	virtual QLabel* getTitleIcon() override;
private:
	QApplicationSettings* settings;
	QList<SearchItem> searchSources;
	QList<GroupForFileFiltering> filterGroups;
	QList<SchedulerTask> tasks;
	QDateTimeEdit* previousFocuse;
	TorrentTracker* tracker;
	RconWebService* rcon;

	void FillFilteringGroups();
	void FillTorrentTab();
	void FillHDDTab();
	void FillDTTab();
	void FillWebUITab();
	void FillKeyMapTab();
	void SetupSchedullerTab();
	void FillSearchTab();
	void FillNetworkTab();
public:
	SettingsDialog(QWidget* parent = 0, int flags = 0);
	~SettingsDialog();
	void ApplySettingsToSession();
	void instance(QString currentStyle);
signals:
	void needRetranslate();
	void tasksChanged();
	private slots:
	void chooseAction(QAbstractButton* button);
	void ApplyAndClose();
	void ApplySettings();
	void addGroup();
	void removeGroup();
	void browseSavepathGroup();
	void browseDTPath();
	void showSelectedGroup(int);
	void SetDate(QDate date);
	void AddTask();
	void DeleteTask();
	void UpdateSchedullerTab(int index);
	void StartRcon();
	void StopRcon();
	void searchItemChanged(int index);
	void addSearchitem();
	void removeSearchItem();
	void setupWindowIcons();
	void OpenWebUI();
	void FillRestrictionTab();
};

#endif // !_SETTINGS_DLG_H

