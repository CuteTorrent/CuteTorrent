/*
    Copyright (C) 2010 Jean-Baptiste Kempf <jb@videolan.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), Nokia Corporation
    (or its successors, if any) and the KDE Free Qt Foundation, which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

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
#ifndef VideoPlayerWindow_H
#define VideoPlayerWindow_H

#include <QMainWindow>
#include <QSignalMapper>
#include "MediaControls.h"
#include <phonon/audiooutput.h>
namespace Phonon
{
class VideoWidget;
}
class MediaController;
class QWinThumbnailToolBar;
class QWinThumbnailToolButton;
class QWinTaskbarButton;
class VideoPlayerWindow : public QMainWindow
{
	Q_OBJECT
public:
	
	explicit VideoPlayerWindow(QWidget* parent = 0);
	~VideoPlayerWindow();
	void SetupConnections() const;
	void openFile(QString path);
signals:

public slots:
	void goFullScreen();
	void OnAvailableSubtitlesChanged();
	void OnAvailableAudioChannelsChanged();
	void OnCustomContextMenuRequested(QPoint);
	void OnSubtitleChosen(int) const;
	void OnAudioChannelChosen(int) const;
	static void updateWindowActiveState();

protected:
	void mouseMoveEvent(QMouseEvent* event) override;
	void timerEvent(QTimerEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;
	bool eventFilter(QObject* src, QEvent* event) override;
	void keyPressEvent(QKeyEvent* keyEvent) override;
	void BuildMenu();

private:
	bool m_bIsFullScr;
	bool m_bMouseHideNeeded;
	QTimer* m_windowActiveTimer;
	MediaControls* controls;
	MediaController* m_mediaControl;
	Phonon::VideoWidget* m_pVideoWidget;
	int animationTimerID;
	QSignalMapper* m_pSubtitlesMapper;
	QSignalMapper* m_pAudioChannelsMapper;
	QMenu m_cMainMenu;
	QMenu m_cAudioStreamsMenu;
	QActionGroup* m_pAudioActions;
	QMenu m_cSubtitleStreams;
	QActionGroup* m_pSubtitlesActions;
	Phonon::MediaController* m_pMediaController;
	QWinTaskbarButton* m_pTaskBarButton;
#ifdef Q_WS_WIN
	void setupTaskBar();
#endif
};

#endif // VideoPlayerWindow_H
