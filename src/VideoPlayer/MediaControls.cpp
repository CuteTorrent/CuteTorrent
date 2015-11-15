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

#include "MediaControls.h"
#include <phonon/MediaObject>
MediaControls::MediaControls(MediaController* m_mediaController, QWidget* parrent) : QWidget(parrent), hidingEnabled(false), reverseTime(false)  /*,m_opacity(0.5)*/
{
	ui = new Ui::MediaControls();
	ui->setupUi(this);
	m_mediaControl = m_mediaController;
	ui->volumeSlider->setAudioOutput(m_mediaController->audioOutput());
	ui->seekSlider->setMediaObject(m_mediaController->mediaObject());
	bg = new QPixmap(":/PlayerControlImages/ControlBar.png");
	setMask(bg->scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation).mask());
	setupConnections();
	setMouseTracking(true);
	ui->currentTimeLabel->installEventFilter(this);
}

MediaControls::~MediaControls(void)
{
	delete ui;
	delete bg;
}



void MediaControls::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	QStyleOption opt;
	opt.init(this);
	painter.drawPixmap(rect(), bg->scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	setMask(bg->scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation).mask());
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
	QWidget::paintEvent(event);
}

void MediaControls::setupConnections()
{
	connect(ui->playButton, SIGNAL(clicked()), this, SIGNAL(play()));
	connect(ui->pauseButton, SIGNAL(clicked()), this, SIGNAL(pause()));
	connect(ui->reverseButton, SIGNAL(clicked()), this, SIGNAL(reverse()));
	connect(ui->forwardButton, SIGNAL(clicked()), this, SIGNAL(forvard()));
	connect(ui->openFileButton, SIGNAL(clicked()), this, SIGNAL(openFile()));
	connect(ui->openUrlButton, SIGNAL(clicked()), this, SIGNAL(openURL()));
	connect(ui->fullScreenButton, SIGNAL(clicked()), this, SIGNAL(toggleFullScreen()));
	connect(m_mediaControl->mediaObject(), SIGNAL(tick(qint64)), SLOT(updateTime(qint64)));
	connect(m_mediaControl->mediaObject(), SIGNAL(totalTimeChanged(qint64)), SLOT(updateTotalTime(qint64)));
}

void MediaControls::updateMedaiObject()
{
	ui->seekSlider->setMediaObject(m_mediaControl->mediaObject());
}


void MediaControls::updateTime(qint64 time)
{
	QTime t(0, 0, 0);
	t = t.addMSecs(reverseTime ? totalTime - time : time);
	if (totalTime != 0)
	{
		emit timeChanged(time * 100ll / totalTime);
	}
	ui->currentTimeLabel->setText(t.toString("hh:mm:ss"));
}
void MediaControls::updateTotalTime(qint64 time)
{
	totalTime = time;
	QTime t(0, 0, 0);
	t = t.addMSecs(time);
	ui->totalTimeLabel->setText(t.toString("hh:mm:ss"));
}
bool MediaControls::eventFilter(QObject* obj, QEvent* event)
{
	if(obj == ui->currentTimeLabel && event->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

		if (mouseEvent->button() == Qt::LeftButton)
		{
			reverseTime = !reverseTime;
			mouseEvent->accept();
			return true;
		}
	}

	return QWidget::eventFilter(obj, event);
}



