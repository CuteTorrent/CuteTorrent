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
#include <QDialog>
#include <QMouseEvent>
#include "CustomWindow.h"
#include "ui_multipleDt.h"

class DT_mounter;

class MultipleDTDialog : public BaseWindow<QDialog>, Ui::DTMountDialog
{
	Q_OBJECT
protected:
	void changeEvent(QEvent* event);
public:
	MultipleDTDialog(QStringList& _files, QWidget* parrent = 0, int flags = 0);

private:
	QPushButton* getCloseBtn() override;
	QWidget* getTitleBar() override;
	QWidget* centralWidget() override;
	QLabel* getTitleLabel() override;
	QLabel* getTitleIcon() override;
	QStringList files;
	void Mount(QString filename);
public slots:
	void MountSelectedFILE();
};

