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

#include "VideoPlayerWindow.h"
#include "mediacontroller.h"


#include <phonon/MediaObject>
#include <phonon/MediaSource>
#include <phonon/VideoWidget>

#include "messagebox.h"
#include <QContextMenuEvent>
#include <QApplication>
#include <QFileDialog>
#include <QMenu>
#include "StyleEngene.h"
#ifdef Q_WS_WIN
#include <Windows.h>
#endif
VideoPlayerWindow::VideoPlayerWindow(QWidget* parent) :	QMainWindow(parent),
	m_bIsFullScr(false),
	m_bMouseHideNeeded(false),
	m_pSubtitlesMapper(new QSignalMapper(this)),
	m_pAudioChannelsMapper(new QSignalMapper(this)),
	m_pAudioActions(new QActionGroup(this)),
	m_pSubtitlesActions(new QActionGroup(this))
{
	try
	{
		m_pVideoWidget = new Phonon::VideoWidget(this);
		setCentralWidget(m_pVideoWidget);
		m_pVideoWidget->setContextMenuPolicy(Qt::CustomContextMenu);
		m_mediaControl = new MediaController(m_pVideoWidget);
		m_pMediaController = m_mediaControl->mediaController();
		Phonon::createPath(m_mediaControl->mediaObject(), m_pVideoWidget);
		controls = new MediaControls(m_mediaControl, m_pVideoWidget);
		controls->installEventFilter(this);
		setWindowIcon(QIcon(":/icons/app.ico"));
		SetupConnections();
		m_windowActiveTimer = new QTimer(this);
		connect(m_windowActiveTimer, SIGNAL(timeout()), this, SLOT(updateWindowActiveState()));
		m_windowActiveTimer->start(59000);
		setAttribute(Qt::WA_DeleteOnClose);
		setMouseTracking(true);
		resize(600, 400);
		m_pVideoWidget->installEventFilter(this);
		m_pVideoWidget->setAspectRatio(Phonon::VideoWidget::AspectRatioAuto);
		BuildMenu();
		controls->move((m_pVideoWidget->width() - controls->width()) / 2, m_pVideoWidget->height() - controls->height());
		m_pAudioActions->setExclusive(true);
		m_pSubtitlesActions->setExclusive(true);
	}
	catch(...)
	{
		CustomMessageBox::warning(this, "Exception", "VideoPlayerWindow::VideoPlayerWindow()\n");
	}
}



void VideoPlayerWindow::OnAvailableSubtitlesChanged()
{
	QList<Phonon::SubtitleDescription> subtitles = m_pMediaController->availableSubtitles();
	qDebug() << sender() << "availableSubtitles" << subtitles;
	Phonon::SubtitleDescription current = m_pMediaController->currentSubtitle();
	QList<QAction*> actions = m_cSubtitleStreams.actions();

	foreach(QAction* action, actions)
	{
		m_pSubtitlesMapper->removeMappings(action);
		m_pSubtitlesActions->removeAction(action);
	}

	qDeleteAll(actions);
	m_cSubtitleStreams.setEnabled(subtitles.length() != 0);

	for(int i = 0; i < subtitles.length(); i++)
	{
		QAction* menuAction = m_cSubtitleStreams.addAction(subtitles[i].name());
		menuAction->setCheckable(true);
		menuAction->setChecked(current == subtitles[i]);
		QObject::connect(menuAction, SIGNAL(triggered()), m_pSubtitlesMapper, SLOT(map()));
		m_pSubtitlesMapper->setMapping(menuAction, i);
		m_pSubtitlesActions->addAction(menuAction);
	}
}

void VideoPlayerWindow::OnAvailableAudioChannelsChanged()
{
	QList<Phonon::AudioChannelDescription> audioChannels = m_pMediaController->availableAudioChannels();
	qDebug() << sender() << "availableAudioChannels" << audioChannels;
	QList<QAction*> actions = m_cAudioStreamsMenu.actions();
	Phonon::AudioChannelDescription current = m_pMediaController->currentAudioChannel();

	foreach(QAction* action, actions)
	{
		m_pAudioChannelsMapper->removeMappings(action);
		m_pAudioActions->removeAction(action);
	}

	qDeleteAll(m_cAudioStreamsMenu.actions());
	m_cAudioStreamsMenu.setEnabled(audioChannels.length() != 0);

	for(int i = 0; i < audioChannels.length(); i++)
	{
		QAction* menuAction = m_cAudioStreamsMenu.addAction(audioChannels[i].name());
		menuAction->setCheckable(true);
		menuAction->setChecked(current == audioChannels[i]);
		QObject::connect(menuAction, SIGNAL(triggered()), m_pAudioChannelsMapper, SLOT(map()));
		m_pAudioChannelsMapper->setMapping(menuAction, i);
		m_pAudioActions->addAction(menuAction);
	}
}

void VideoPlayerWindow::resizeEvent(QResizeEvent* event)
{
	controls->move(QPoint((size().width() - controls->width()) / 2, size().height() - controls->height()));
	QWidget::resizeEvent(event);
}

bool VideoPlayerWindow::eventFilter(QObject* src, QEvent* event)
{
	if(src == m_pVideoWidget && event->type() == QEvent::MouseButtonDblClick)
	{
		QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
		if (mouseEvent->button() == Qt::LeftButton)
		{
			goFullScreen();
			mouseEvent->accept();
			return true;
		}
		
	}
	else if (src == controls && event->type() == QEvent::Resize)
	{
		controls->move(QPoint((size().width() - controls->width()) / 2, size().height() - controls->height()));
	}

	return false;
}

void VideoPlayerWindow::goFullScreen()
{
	m_bIsFullScr = !m_bIsFullScr;
	Qt::WindowFlags flags = windowFlags();

	if(m_bIsFullScr)
	{
		if(!isFullScreen())
		{
			//we only update that value if it is not already fullscreen
#ifdef Q_WS_X11
			// This works around a bug with Compiz
			// as the window must be visible before we can set the state
			show();
			raise();
			setWindowState(windowState() | Qt::WindowFullScreen);    // set
#else
			setWindowState(windowState() | Qt::WindowFullScreen);    // set
			show();
#endif
		}
	}
	else if(isFullScreen())
	{
		setWindowState(windowState()  ^ Qt::WindowFullScreen);    // reset
		show();
	}

	controls->move(QPoint((m_pVideoWidget->width() - controls->width()) / 2, m_pVideoWidget->height() - controls->height()));
}

void VideoPlayerWindow::openFile(QString path)
{
	try
	{
		m_mediaControl->playFile(path);
	}
	catch(...)
	{
		CustomMessageBox::warning(this, "ERROR", "VideoPlayerWindow::openFile()");
	}
}

void VideoPlayerWindow::mouseMoveEvent(QMouseEvent* event)
{
	setCursor(QCursor(Qt::ArrowCursor));
	controls->show();
	animationTimerID = startTimer(3000);
	event->accept();
}

void VideoPlayerWindow::timerEvent(QTimerEvent* event)
{
	if(event->timerId() == animationTimerID && !m_cMainMenu.isVisible())
	{
		killTimer(animationTimerID);
		controls->hide();
		setCursor(QCursor(Qt::BlankCursor));
	}
}

void VideoPlayerWindow::keyPressEvent(QKeyEvent* keyEvent)
{
	if(keyEvent->key() == Qt::Key_Space)
	{
		if(m_mediaControl->isPlaying())
		{
			m_mediaControl->pause();
		}
		else
		{
			m_mediaControl->play();
		}
	}
}

void VideoPlayerWindow::OnCustomContextMenuRequested(QPoint point)
{
	m_cMainMenu.exec(m_pVideoWidget->mapToGlobal(point));
}

void VideoPlayerWindow::SetupConnections()
{
	QObject::connect(controls, SIGNAL(play()), m_mediaControl, SLOT(play()));
	QObject::connect(controls, SIGNAL(pause()), m_mediaControl, SLOT(pause()));
	QObject::connect(controls, SIGNAL(openFile()), m_mediaControl, SLOT(openFile()));
	QObject::connect(controls, SIGNAL(openURL()), m_mediaControl, SLOT(openURL()));
	QObject::connect(controls, SIGNAL(forvard()), m_mediaControl, SLOT(forvard()));
	QObject::connect(controls, SIGNAL(reverse()), m_mediaControl, SLOT(reverse()));
	QObject::connect(controls, SIGNAL(toggleFullScreen()), this, SLOT(goFullScreen()));
	QObject::connect(m_mediaControl, SIGNAL(newFile(QString)), this, SLOT(setWindowTitle(QString)));
	QObject::connect(m_mediaControl, SIGNAL(updateMediaObject()), controls, SLOT(updateMedaiObject()));
	QObject::connect(m_pMediaController, SIGNAL(availableSubtitlesChanged()), SLOT(OnAvailableSubtitlesChanged()));
	QObject::connect(m_pMediaController, SIGNAL(availableAudioChannelsChanged()), SLOT(OnAvailableAudioChannelsChanged()));
	QObject::connect(m_pVideoWidget, SIGNAL(customContextMenuRequested(QPoint)), SLOT(OnCustomContextMenuRequested(QPoint)));
	QObject::connect(m_pSubtitlesMapper, SIGNAL(mapped(int)), SLOT(OnSubtitleChosen(int)));
	QObject::connect(m_pAudioChannelsMapper, SIGNAL(mapped(int)), SLOT(OnAudioChannelChosen(int)));
}

void VideoPlayerWindow::BuildMenu()
{
	StyleEngene* styleEngine = StyleEngene::getInstance();
	QAction* menuAction = m_cMainMenu.addAction(tr("VP_PLAY"));
	menuAction->setIcon(styleEngine->getIcon("VP_PLAY"));
	QObject::connect(menuAction, SIGNAL(triggered()), m_mediaControl, SLOT(play()));
	menuAction = m_cMainMenu.addAction(tr("VP_PAUSE"));
	menuAction->setIcon(styleEngine->getIcon("VP_PAUSE"));
	QObject::connect(menuAction, SIGNAL(triggered()), m_mediaControl, SLOT(pause()));
	menuAction = m_cMainMenu.addAction(tr("VP_NEXT"));
	menuAction->setIcon(styleEngine->getIcon("VP_NEXT"));
	QObject::connect(menuAction, SIGNAL(triggered()), m_mediaControl, SLOT(forvard()));
	menuAction = m_cMainMenu.addAction(tr("VP_PREV"));
	menuAction->setIcon(styleEngine->getIcon("VP_PREV"));
	QObject::connect(menuAction, SIGNAL(triggered()), m_mediaControl, SLOT(reverse()));
	m_cSubtitleStreams.setTitle(tr("VP_SUBTITLES"));
	m_cMainMenu.addMenu(&m_cSubtitleStreams);
	m_cAudioStreamsMenu.setTitle(tr("VP_AUDIO_CHANNELS"));
	m_cMainMenu.addMenu(&m_cAudioStreamsMenu);
	menuAction = m_cMainMenu.addAction(tr("VP_EXIT"));
	menuAction->setIcon(styleEngine->getIcon("VP_EXIT"));
	QObject::connect(menuAction, SIGNAL(triggered()), this, SLOT(close()));
}

void VideoPlayerWindow::OnSubtitleChosen(int index)
{
	m_pMediaController->setCurrentSubtitle(m_pMediaController->availableSubtitles().at(index));
}

void VideoPlayerWindow::OnAudioChannelChosen(int index)
{
	m_pMediaController->setCurrentAudioChannel(m_pMediaController->availableAudioChannels().at(index));
}

VideoPlayerWindow::~VideoPlayerWindow()
{
	m_mediaControl->pause();
	m_windowActiveTimer->stop();
}

void VideoPlayerWindow::updateWindowActiveState()
{
#ifdef Q_WS_WIN
	SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, 1, NULL, SPIF_SENDWININICHANGE);
#endif
}










