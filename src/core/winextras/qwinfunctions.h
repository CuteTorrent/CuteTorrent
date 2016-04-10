/****************************************************************************
**
** Copyright (C) 2013 Ivan Vizir <define-true-false@yandex.com>
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

#ifndef QWINFUNCTIONS_H
#define QWINFUNCTIONS_H

#if 0
#pragma qt_class(QtWin)
#endif

#include <QtCore/qobject.h>
#include <QtCore/qt_windows.h>
#include "qwinextrasglobal.h"
#ifdef QT_WIDGETS_LIB
#include <QtWidgets/qwidget.h>
#endif


QT_BEGIN_NAMESPACE

class QPixmap;
class QImage;
class QBitmap;
class QColor;
class QWidget;
class QString;
class QMargins;
class QRegion;

namespace QtWin
{
	enum HBitmapFormat
	{
		HBitmapNoAlpha,
		HBitmapPremultipliedAlpha,
		HBitmapAlpha
	};

	enum WindowFlip3DPolicy
	{
		FlipDefault,
		FlipExcludeBelow,
		FlipExcludeAbove
	};

	HBITMAP createMask(const QBitmap& bitmap);
	HBITMAP toHBITMAP(const QPixmap& p, HBitmapFormat format = HBitmapNoAlpha);
	QPixmap fromHBITMAP(HBITMAP bitmap, HBitmapFormat format = HBitmapNoAlpha);
	HICON toHICON(const QPixmap& p);
	QImage imageFromHBITMAP(HDC hdc, HBITMAP bitmap, int width, int height);
	QPixmap fromHICON(HICON icon);
	HRGN toHRGN(const QRegion& region);
	QRegion fromHRGN(HRGN hrgn);

	QString stringFromHresult(HRESULT hresult);
	QString errorStringFromHresult(HRESULT hresult);

	QColor colorizationColor(bool* opaqueBlend = 0);
	QColor realColorizationColor();

	void setWindowExcludedFromPeek(QWidget* window, bool exclude);
	bool isWindowExcludedFromPeek(QWidget* window);
	void setWindowDisallowPeek(QWidget* window, bool disallow);
	bool isWindowPeekDisallowed(QWidget* window);
	void setWindowFlip3DPolicy(QWidget* window, WindowFlip3DPolicy policy);
	WindowFlip3DPolicy windowFlip3DPolicy(QWidget*);

	void extendFrameIntoClientArea(QWidget* window, int left, int top, int right, int bottom);
	void extendFrameIntoClientArea(QWidget* window, const QMargins& margins);
	void resetExtendedFrame(QWidget* window);

	void enableBlurBehindWindow(QWidget* window, const QRegion& region);
	void enableBlurBehindWindow(QWidget* window);
	void disableBlurBehindWindow(QWidget* window);

	bool isCompositionEnabled();
	void setCompositionEnabled(bool enabled);
	bool isCompositionOpaque();

	void setCurrentProcessExplicitAppUserModelID(const QString& id);

	void markFullscreenWindow(QWidget*, bool fullscreen = true);

	void taskbarActivateTab(QWidget*);
	void taskbarActivateTabAlt(QWidget*);
	void taskbarAddTab(QWidget*);
	void taskbarDeleteTab(QWidget*);

#ifdef QT_WIDGETS_LIB
inline void setWindowExcludedFromPeek(QWidget* window, bool exclude)
{
	window->createWinId();
	setWindowExcludedFromPeek(window->windowHandle(), exclude);
}

inline bool isWindowExcludedFromPeek(QWidget* window)
{
	if (!window->windowHandle())
	{
		return false;
	}
	else
	{
		return isWindowExcludedFromPeek(window->windowHandle());
	}
}

inline void setWindowDisallowPeek(QWidget* window, bool disallow)
{
	window->createWinId();
	setWindowDisallowPeek(window->windowHandle(), disallow);
}

inline bool isWindowPeekDisallowed(QWidget* window)
{
	if (!window->windowHandle())
	{
		return false;
	}
	else
	{
		return isWindowPeekDisallowed(window->windowHandle());
	}
}

inline void setWindowFlip3DPolicy(QWidget* window, WindowFlip3DPolicy policy)
{
	window->createWinId();
	setWindowFlip3DPolicy(window->windowHandle(), policy);
}

inline WindowFlip3DPolicy windowFlip3DPolicy(QWidget* window)
{
	if (!window->windowHandle())
	{
		return FlipDefault;
	}
	else
	{
		return windowFlip3DPolicy(window->windowHandle());
	}
}

inline void extendFrameIntoClientArea(QWidget* window, const QMargins& margins)
{
	window->createWinId();
	extendFrameIntoClientArea(window->windowHandle(), margins);
}

inline void extendFrameIntoClientArea(QWidget* window, int left, int top, int right, int bottom)
{
	window->createWinId();
	extendFrameIntoClientArea(window->windowHandle(), left, top, right, bottom);
}

inline void resetExtendedFrame(QWidget* window)
{
	if (window->windowHandle())
	{
		resetExtendedFrame(window->windowHandle());
	}
}

inline void enableBlurBehindWindow(QWidget* window, const QRegion& region)
{
	window->createWinId();
	enableBlurBehindWindow(window->windowHandle(), region);
}

inline void enableBlurBehindWindow(QWidget* window)
{
	window->createWinId();
	enableBlurBehindWindow(window->windowHandle());
}

inline void disableBlurBehindWindow(QWidget* window)
{
	if (window->windowHandle())
	{
		disableBlurBehindWindow(window->windowHandle());
	}
}

inline void markFullscreenWindow(QWidget* window, bool fullscreen = true)
{
	window->createWinId();
	markFullscreenWindow(window->windowHandle(), fullscreen);
}

inline void taskbarActivateTab(QWidget* window)
{
	window->createWinId();
	taskbarActivateTab(window->windowHandle());
}

inline void taskbarActivateTabAlt(QWidget* window)
{
	window->createWinId();
	taskbarActivateTabAlt(window->windowHandle());
}

inline void taskbarAddTab(QWidget* window)
{
	window->createWinId();
	taskbarAddTab(window->windowHandle());
}

inline void taskbarDeleteTab(QWidget* window)
{
	window->createWinId();
	taskbarDeleteTab(window->windowHandle());
}
#endif // QT_WIDGETS_LIB

}

QT_END_NAMESPACE

#endif // QWINFUNCTIONS_H


