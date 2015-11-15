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
#ifndef MediaControls_H
#define MediaControls_H
#include <QtGui>
#include "ui_MediaControls.h"
#include "mediacontroller.h"
class MediaControls : public QWidget
{
	Q_OBJECT
public:
	MediaControls(MediaController* m_mediaController, QWidget* parrent = NULL);
	~MediaControls(void);


public slots:
	void updateTime(qint64 time);
	void updateTotalTime(qint64 time);
	void updateMedaiObject();
signals:
	void forvard();
	void reverse();
	void play();
	void openFile();
	void openURL();
	void pause();
	void setVolume(int value);
	void timeChanged(int value);
	void toggleFullScreen();
private:
	qint64 totalTime;
	bool reverseTime;
	bool hidingEnabled;
	Ui::MediaControls* ui;
	MediaController* m_mediaControl;
	QPixmap* bg;
	void setupConnections();

protected:
	void paintEvent(QPaintEvent* event) override;
	bool eventFilter(QObject* obj, QEvent* event) override;

};

#endif
