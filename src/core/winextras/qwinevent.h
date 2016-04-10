/****************************************************************************
 **
 ** Copyright (C) 2013 Ivan Vizir <define-true-false@yandex.com>
 ** Copyright (C) 2015 The Qt Company Ltd.
 ** Contact: http://www.qt.io/licensing/
 **
 ** This file is part of the QtWinExtras module of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:LGPL21$
 ** Commercial License Usage
 ** Licensees holding valid commercial Qt licenses may use this file in
 ** accordance with the commercial license agreement provided with the
 ** Software or, alternatively, in accordance with the terms contained in
 ** a written agreement between you and The Qt Company. For licensing terms
 ** and conditions see http://www.qt.io/terms-conditions. For further
 ** information use the contact form at http://www.qt.io/contact-us.
 **
 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 or version 3 as published by the Free
 ** Software Foundation and appearing in the file LICENSE.LGPLv21 and
 ** LICENSE.LGPLv3 included in the packaging of this file. Please review the
 ** following information to ensure the GNU Lesser General Public License
 ** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
 ** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** As a special exception, The Qt Company gives you certain additional
 ** rights. These rights are described in The Qt Company LGPL Exception
 ** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
 **
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

#ifndef QWINEVENT_H
#define QWINEVENT_H

#include <QtGui/qrgb.h>
#include <QtCore/qcoreevent.h>
#include <QSize>
#include <qt_windows.h>


QT_BEGIN_NAMESPACE

class QWinEvent : public QEvent
{
public:
	static const int ColorizationChange;
	static const int CompositionChange;
	static const int TaskbarButtonCreated;
	static const int TaskbarButtonClicked;
	static const int RequestThumbnailBitmap;
	static const int RequestLivePreviewBitmap;
	static const int ThemeChange;

	explicit QWinEvent(int type);
	~QWinEvent();
};

class QWinColorizationChangeEvent : public QWinEvent
{
public:
	QWinColorizationChangeEvent(QRgb color, bool opaque);
	~QWinColorizationChangeEvent();

	inline QRgb color() const
	{
		return rgb;
	}

	inline bool opaqueBlend() const
	{
		return opaque;
	}

private:
	QRgb rgb;
	bool opaque;
};

class QWinCompositionChangeEvent : public QWinEvent
{
public:
	explicit QWinCompositionChangeEvent(bool enabled);
	~QWinCompositionChangeEvent();

	inline bool isCompositionEnabled() const
	{
		return enabled;
	}

private:
	bool enabled;
};


class QWinTaskBarButtonClicked : public QWinEvent
{
public:
	explicit QWinTaskBarButtonClicked(unsigned short buttonId);
	unsigned short getClickedButtonId() const;

private:
	unsigned short buttonId;
};

class QWinRequestThumbnailBitmap : public QWinEvent
{
public:
	explicit QWinRequestThumbnailBitmap(QSize maxSize, HWND hwnd);

	QSize maxSize() const;
	HWND hwnd() const;

private:
	QSize m_maxSize;
	HWND m_hwnd;
};

class QWinRequestLivePreviewBitmap : public QWinEvent
{
public:
	explicit QWinRequestLivePreviewBitmap(HWND hwnd);


	HWND hwnd() const;

private:
	HWND m_hwnd;
};

QT_END_NAMESPACE

#endif // QWINEVENT_H


