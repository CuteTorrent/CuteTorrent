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
#pragma once

#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QString>
#include <QTreeWidgetItem>

#include "CustomWindow.h"
#include "TorrentCommon.h"
#include "ui_OpenTorrentDialog.h"
#include "GroupForFileFiltering.h"
#include "defs.h"

class FileTreeModel;

class OpenTorrentDialog : public BaseWindow<QDialog> , private Ui::OpenTorrentDialog
{
	Q_OBJECT
protected:
	void changeEvent(QEvent* event) override;
public:

	OpenTorrentDialog(QWidget* parent = 0, Qt::WindowFlags flags = 0);
	~OpenTorrentDialog();
	void FillData(opentorrent_info* info);
	void SetData(QString filename);
	void setCheckedValue(bool val);
	int exec();
private:
	int64_t m_size;
	QFileSystemModel* m_compliterModel;
	bool m_bUseGroup;
	QList<GroupForFileFiltering> m_lFilters;
	bool validTorrent;
	TorrentManagerPtr m_pTorrentManager;
	QString m_torrentFilename;
	openmagnet_info m_info;
	FileTreeModel* m_pFileTreeModel;
	QPushButton* getCloseBtn() override;
	QWidget* getTitleBar() override;
	QWidget* centralWidget() override;
	QLabel* getTitleLabel() override;
	QLabel* getTitleIcon() override;
    int BuildFlags();
	bool AccepTorrent();
public slots:
	void done(int) override;
private slots:
	void reject() override;
	void OnError(QString);
	void BrowseButton();
	
	
	void ChangeGroup();
	void DownloadMetadataCompleted(openmagnet_info);
	void OnPathChanged(QString path);
	
	void OnCheckAll();
	void OnUncheckAll();

};

