#pragma once

#define PIXELS_TO_ACT 3
#include <QtGui>
#include "StyleEngene.h"
template <class T>
class BaseWindow : public T
{
	Q_DISABLE_COPY(BaseWindow)
public:
	enum MoveType { MT_None, MT_NormalMove, MT_ShowRightHalfDesktopPreview, MT_ShowingRightHalfDesktopPreview, MT_ShowLeftHalfDesktopPreview, MT_ShowingLeftHalfDesktopPreview, MT_ShowMaximized, MT_ShowingMaximized };
	enum ResizeMode { NoResize, AllowResize};
	enum TitleMode { FullTitle = 0, MaximizeModeOff = 1, MinimizeModeOff = 2, MaxMinOff = MaximizeModeOff | MinimizeModeOff, OnlyCloseButton = MaxMinOff, FullScreenMode = 4 };
	BaseWindow(TitleMode titleMode, ResizeMode resizeMode, QWidget* parent = NULL);
	void showMaximized();
	void showNormal();
	void setGeometry(QRect geo);
	void setTitle(QString title);
	void hide();
	bool isMaximized();
protected:
	MoveType m_moveType;

	MoveType detectMoveType(const QPoint pos);
	QRect getMultyMonitorDesktopRect();
	bool m_bIsHalfDeskMode;
	bool m_bIsMaximized;
	QRect preMaximizeGeomentry;
	QHBoxLayout* centralLayout;
	/**
	* @brief dragPosition Increment of the position movement.
	*/
	QPoint dragPosition;
	ResizeMode m_resizeMode;

	/**
	* @brief m_titleMode Flags that defines the current getTitleBar() mode.
	*/
	TitleMode m_titleMode;
	/**
	* @brief moveWidget Specifies if the window is in move action.
	*/
	bool moveWidget;
	/**
	* @brief inResizeZone Specifies if the mouse is in resize zone.
	*/
	bool inResizeZone;
	/**
	* @brief allowToResize Specifies if the mouse is allowed to resize.
	*/
	bool allowToResize;
	/**
	* @brief resizeVerSup Specifies if the resize is in the top of the window.
	*/
	bool resizeVerSup;
	/**
	* @brief resizeHorEsq Specifies if the resize is in the left of the window.
	*/
	bool resizeHorEsq;
	/**
	* @brief resizeDiagSupEsq Specifies if the resize is in the top left of the window.
	*/
	bool resizeDiagSupEsq;
	/**
	* @brief resizeDiagSupDer Specifies if the resize is in the top right of the window.
	*/
	bool resizeDiagSupDer;

	/**
	* @brief mouseMoveEvent Overloaded member that moves of resizes depending of the
	* configuration sotred at mousePressEvent().
	* @param e The mouse event.
	*/
	virtual void mouseMoveEvent(QMouseEvent* e);
	/**
	* @brief mousePressEvent Overloaded member that stores that changes the cursor and
	* configures the side that is changing.
	* @param e The mouse event.
	*/
	virtual void mousePressEvent(QMouseEvent* e);
	/**
	* @brief mouseReleaseEvent Overloaded member that removes the configuration set in mousePressEvent().
	* @param e The mouse event.
	*/
	virtual void mouseReleaseEvent(QMouseEvent* e);
	/**
	* @brief mouseDoubleClickEvent Overloadad member that maximize/restore the window if is
	* doubleclicked and the position of the mouse is not the top left of the window (menu zone).
	* @param e The mouse event.
	*/
	virtual void mouseDoubleClickEvent(QMouseEvent* e);
	/**
	* @brief paintEvent Overloaded method that allows to customize the styles of the window.
	*/
	virtual void paintEvent(QPaintEvent*);
	/**
	* @brief resizeWindow Method that calculates the resize and new position of the window an
	* does this actions.
	* @param e The mouse event to calculate the new position and size.
	*/
	virtual void resizeWindow(QMouseEvent* e);

	virtual void moveWindow(QMouseEvent* e);
	virtual void setupCustomWindow();

	virtual void setupWindowIcons();
	virtual QPushButton* getMinBtn();
	virtual QPushButton* getMaxBtn();
	virtual QPushButton* getCloseBtn();
	virtual QWidget* getTitleBar();
	virtual QWidget* centralWidget();
	virtual QLabel* getTitleLabel();
	virtual QLabel* getTitleIcon();
public slots:
	virtual void maximizeBtnClicked();
	virtual void minimizeBtnClicked();
};

template <class T>
typename BaseWindow<T>::MoveType BaseWindow<T>::detectMoveType(const QPoint pos)
{
	QRect desktopGeometry = getMultyMonitorDesktopRect();
	if (isMaximized())
	{
        QRect windowGeometry = T::geometry();

		if (abs(pos.y() - desktopGeometry.y()) <= PIXELS_TO_ACT)
		{
			return MT_ShowingMaximized;
		}
		else if (abs(pos.x() - desktopGeometry.x()) <= PIXELS_TO_ACT)
		{
			QRect halfDesctopRect = desktopGeometry;
			halfDesctopRect.setWidth(halfDesctopRect.width() / 2);

			//qDebug() << "MT_ShowingLeftHalfDesktopPreview" << windowGeometry << halfDesctopRect;
			if (m_bIsHalfDeskMode)
			{
				return MT_ShowingLeftHalfDesktopPreview;
			}
			else
			{
				return MT_ShowLeftHalfDesktopPreview;
			}
		}
		else if (abs(pos.x() - desktopGeometry.width() - desktopGeometry.x()) <= PIXELS_TO_ACT)
		{
			QRect halfDesctopRect = desktopGeometry;
			halfDesctopRect.setX(halfDesctopRect.width() / 2);

			if (m_bIsHalfDeskMode)
			{
				return MT_ShowingRightHalfDesktopPreview;
			}
			else
			{
				return MT_ShowRightHalfDesktopPreview;
			}
		}
	}
	else
	{
		if (abs(pos.y() - desktopGeometry.y()) <= PIXELS_TO_ACT)
		{
			return MT_ShowMaximized;
		}
		else
		{
            QRect windowGeometry = T::geometry();

			if (abs(pos.x() - desktopGeometry.x()) <= PIXELS_TO_ACT)
			{
				QRect halfDesctopRect = desktopGeometry;
				halfDesctopRect.setWidth(halfDesctopRect.width() / 2);

				//qDebug() << "MT_ShowingLeftHalfDesktopPreview" << windowGeometry << halfDesctopRect;
				if (m_bIsHalfDeskMode)
				{
					return MT_ShowingLeftHalfDesktopPreview;
				}
				else
				{
					return MT_ShowLeftHalfDesktopPreview;
				}
			}
			else if (abs(pos.x() - desktopGeometry.width() - desktopGeometry.x()) <= PIXELS_TO_ACT)
			{
				QRect halfDesctopRect = desktopGeometry;
				halfDesctopRect.setX(halfDesctopRect.width() / 2);

				if (m_bIsHalfDeskMode)
				{
					return MT_ShowingRightHalfDesktopPreview;
				}
				else
				{
					return MT_ShowRightHalfDesktopPreview;
				}
			}
		}
	}

	return MT_NormalMove;
}

template <class T>
QRect BaseWindow<T>::getMultyMonitorDesktopRect()
{
	QRect desktopGeometry;
	QDesktopWidget* desktopWidget = QApplication::desktop();
	int screenCount = desktopWidget->numScreens();
	for (int i = 0; i < screenCount; i++)
	{
		desktopGeometry = desktopGeometry.united(desktopWidget->availableGeometry(i));
	}
	//qDebug() << "Final desktop gemetry is:" << desktopGeometry;
	return desktopGeometry;
}

template <class T>
void BaseWindow<T>::setTitle(QString title)
{
	if (getTitleLabel() != NULL)
	{
		getTitleLabel()->setText(title);
	}
}

template <class T>
void BaseWindow<T>::hide()
{
    preMaximizeGeomentry = T::geometry();
	T::hide();
}

template <class T>
bool BaseWindow<T>::isMaximized()
{
	return m_bIsMaximized;
}

template <class T>
void BaseWindow<T>::setGeometry(QRect geo)
{
	if (!m_bIsMaximized)
	{
		preMaximizeGeomentry = geo;
	}

	T::setGeometry(geo);
}

template <class T>
void BaseWindow<T>::showNormal()
{
	setGeometry(preMaximizeGeomentry);
	T::showNormal();
	m_bIsMaximized = false;
	m_bIsHalfDeskMode = false;
}

template <class T>
void BaseWindow<T>::showMaximized()
{
	QDesktopWidget* desktop = QApplication::desktop();
    preMaximizeGeomentry = T::geometry();
	m_bIsMaximized = true;
	setGeometry(desktop->availableGeometry(this));
	T::showNormal();
}

template <class T>
QLabel* BaseWindow<T>::getTitleIcon()
{
	return NULL;
}

template <class T>
QLabel* BaseWindow<T>::getTitleLabel()
{
	return NULL;
}

template <class T>
QWidget* BaseWindow<T>::centralWidget()
{
	return NULL;
}

template <class T>
QWidget* BaseWindow<T>::getTitleBar()
{
	return NULL;
}

template <class T>
QPushButton* BaseWindow<T>::getCloseBtn()
{
	return NULL;
}

template <class T>
QPushButton* BaseWindow<T>::getMaxBtn()
{
	return NULL;
}

template <class T>
QPushButton* BaseWindow<T>::getMinBtn()
{
	return NULL;
}

template <class T>
BaseWindow<T>::BaseWindow(TitleMode titleMode, ResizeMode resizeMode, QWidget* /*parent*/) : T(), m_bIsMaximized(false)
{
	m_titleMode = titleMode;
	m_resizeMode = resizeMode;
}

template <class T>
void BaseWindow<T>::setupWindowIcons()
{
	StyleEngene* style = StyleEngene::getInstance();

	if((m_titleMode & MinimizeModeOff) == 0 && getMinBtn() != NULL)
	{
		getMinBtn()->setIcon(style->getIcon("app_min"));
	}

	if((m_titleMode & MaximizeModeOff) == 0 && getMaxBtn() != NULL)
	{
		if (m_bIsMaximized)
		{
			getMaxBtn()->setIcon(style->getIcon("app_reset"));
		}
		else
		{
			getMaxBtn()->setIcon(style->getIcon("app_max"));
		}
	}

	getCloseBtn()->setIcon(style->getIcon("app_close"));
}

template <class T>
void BaseWindow<T>::setupCustomWindow()
{
    T::setWindowFlags(Qt::FramelessWindowHint);
    T::setMouseTracking(true);
	getTitleBar()->setMouseTracking(true);
	getTitleLabel()->setMouseTracking(true);
	getTitleIcon()->setMouseTracking(true);
	getCloseBtn()->setMouseTracking(true);

#ifndef Q_WS_WIN
    QWidget* titleBar = getTitleBar();
    QHBoxLayout* layout = qobject_cast<QHBoxLayout*>(titleBar->layout());
    if (layout != NULL)
    {
        QPushButton* closeBtn = getCloseBtn();
        if (closeBtn != NULL)
        {
            layout->removeWidget(closeBtn);
            layout->insertWidget(0, closeBtn);
        }

        QPushButton* minBtn = getMinBtn();
        if (minBtn != NULL)
        {
            layout->removeWidget(minBtn);
            layout->insertWidget(1, minBtn);
        }

        QPushButton* maxBtn = getMaxBtn();
        if (maxBtn != NULL)
        {
            layout->removeWidget(maxBtn);
            layout->insertWidget(2, maxBtn);
        }
    }
#endif


	if(centralWidget() == NULL)
	{
		qCritical() << "Central widget is null";
	}
	else
	{
		centralWidget()->setMouseTracking(true);
	}

	if ((m_titleMode & MinimizeModeOff) == 0 && getMinBtn() != NULL)
	{
		getMinBtn()->setMouseTracking(true);
		T::connect(getMinBtn(), SIGNAL(clicked()), this, SLOT(minimizeBtnClicked()));
	}
	else if(getMinBtn() != NULL)
	{
		getMinBtn()->hide();
	}

	if ((m_titleMode & MaximizeModeOff) == 0 && getMaxBtn() != NULL)
	{
		getMaxBtn()->setMouseTracking(true);
		T::connect(getMaxBtn(), SIGNAL(clicked()), this, SLOT(maximizeBtnClicked()));
	}
	else if(getMaxBtn() != NULL)
	{
		getMaxBtn()->hide();
	}

	T::connect(getCloseBtn(), SIGNAL(clicked()), this, SLOT(close()));
	moveWidget = false;
	inResizeZone = false;
	allowToResize = false;
	resizeVerSup = false;
	resizeHorEsq = false;
	resizeDiagSupEsq = false;
	resizeDiagSupDer = false;
}


template <class T>
void BaseWindow<T>::maximizeBtnClicked()
{
	StyleEngene* styleEngine = StyleEngene::getInstance();

	if (getMaxBtn() != NULL)
	{
        if (T::isFullScreen() || isMaximized())
		{
			getMaxBtn()->setIcon(styleEngine->getIcon("app_max"));
			showNormal();
		}
		else
		{
			getMaxBtn()->setIcon(styleEngine->getIcon("app_reset"));
			showMaximized();
		}
	}
}

template <class T>
void BaseWindow<T>::minimizeBtnClicked()
{
    if(T::isMinimized())
	{
        T::setWindowState(T::windowState() & ~Qt::WindowMinimized);
	}
	else
	{
        T::setWindowState(T::windowState() | Qt::WindowMinimized);
	}
}
template <class T>
void BaseWindow<T>::moveWindow(QMouseEvent* e)
{
	if (e->buttons() & Qt::LeftButton)
	{
		QRect desktopGeometry = QApplication::desktop()->availableGeometry(this);
		const QPoint pos = e->globalPos();

		if (isMaximized())
		{
			maximizeBtnClicked();
            dragPosition = QPoint(T::geometry().width() / 2, 0);
		}

        T::move(pos - dragPosition);
		e->accept();
	}
}

template <class T>
void BaseWindow<T>::resizeWindow(QMouseEvent* e)
{
	if(allowToResize)
	{
        QRect windowGeoemtry = T::geometry();
        int xMouse = e->globalPos().x();
        int yMouse = e->globalPos().y();
        int wWidth = windowGeoemtry.width();
		int wHeight = windowGeoemtry.height();
        qDebug()<< "resizeWindow" << "xMouse" << xMouse << "yMouse" << yMouse << "wWidth" << wWidth << "wHeight" << wHeight << T::cursor().shape() << windowGeoemtry << T::parentWidget();
        if(T::cursor().shape() == Qt::SizeVerCursor)
		{
            qDebug() << "resizeVerSup" << resizeVerSup;
			if(resizeVerSup)
			{
                int newY = yMouse;
                qDebug() << "newY" << newY;
                int newHeight = wHeight - (yMouse - windowGeoemtry.y());
                qDebug() << "newHeight" << newHeight;
                if(newHeight > T::minimumSizeHint().height())
				{
                    T::resize(wWidth, newHeight);
                    T::move(windowGeoemtry.x(), newY);
				}
			}
			else
			{
                T::resize(wWidth, yMouse + 1);
			}
		}
        else if(T::cursor().shape() == Qt::SizeHorCursor)
		{
			if(resizeHorEsq)
			{
                int newX = xMouse;
				int newWidth = wWidth - xMouse;

                if(newWidth > T::minimumSizeHint().width())
				{
                    T::resize(newWidth, wHeight);
                    T::move(newX, windowGeoemtry.y());
				}
			}
			else
			{
                T::resize(xMouse, wHeight);
			}
		}
        else if(T::cursor().shape() == Qt::SizeBDiagCursor)
		{
			int newX = 0;
			int newWidth = 0;
			int newY = 0;
			int newHeight = 0;

			if(resizeDiagSupDer)
			{
				newX = windowGeoemtry.x();
				newWidth = xMouse;
				newY = windowGeoemtry.y() + yMouse;
				newHeight = wHeight - yMouse;
			}
			else
			{
                newX = xMouse;
				newWidth = wWidth - xMouse;
				newY = windowGeoemtry.y();
				newHeight = yMouse;
			}

            if(newWidth >= T::minimumSizeHint().width() && newHeight >= T::minimumSizeHint().height())
			{
                T::resize(newWidth, newHeight);
                T::move(newX, newY);
			}
            else if(newWidth >= T::minimumSizeHint().width())
			{
                T::resize(newWidth, wHeight);
                T::move(newX, windowGeoemtry.y());
			}
            else if(newHeight >= T::minimumSizeHint().height())
			{
                T::resize(wWidth, newHeight);
                T::move(windowGeoemtry.x(), newY);
			}
		}
        else if(T::cursor().shape() == Qt::SizeFDiagCursor)
		{
			if(resizeDiagSupEsq)
			{
				int newX = windowGeoemtry.x() + xMouse;
				int newWidth = wWidth - xMouse;
				int newY = windowGeoemtry.y() + yMouse;
				int newHeight = wHeight - yMouse;

                if(newWidth >= T::minimumSizeHint().width() && newHeight >= T::minimumSizeHint().height())
				{
                    T::resize(newWidth, newHeight);
                    T::move(newX, newY);
				}
                else if(newWidth >= T::minimumSizeHint().width())
				{
                    T::resize(newWidth, wHeight);
                    T::move(newX, windowGeoemtry.y());
				}
                else if(newHeight >= T::minimumSizeHint().height())
				{
                    T::resize(wWidth, newHeight);
                    T::move(windowGeoemtry.x(), newY);
				}
			}
			else
			{
                T::resize(xMouse + 1, yMouse + 1);
			}
		}

		e->accept();
	}
}

template <class T>
void BaseWindow<T>::paintEvent(QPaintEvent*)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
    T::style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

template <class T>
void BaseWindow<T>::mouseDoubleClickEvent(QMouseEvent* e)
{
    QPoint position = e->globalPos();
    QRect titleGeometry = getTitleBar()->geometry();
    titleGeometry.moveTopLeft(getTitleBar()->parentWidget()->mapToGlobal(titleGeometry.topLeft()));


    if(position.x() < titleGeometry.right() && position.y() < titleGeometry.bottom()
            && position.x() >= titleGeometry.x() && position.y() >= titleGeometry.y()
	        && getTitleIcon()->isVisible())
	{
		T::close();
	}
    else if(position.x() < titleGeometry.width()
            && position.y() < titleGeometry.height()
	        && m_titleMode != FullScreenMode)
	{
		maximizeBtnClicked();
	}

	e->accept();
}

template <class T>
void BaseWindow<T>::mouseReleaseEvent(QMouseEvent* e)
{
	moveWidget = false;
	allowToResize = false;
	resizeVerSup = false;
	resizeHorEsq = false;
	resizeDiagSupEsq = false;
	resizeDiagSupDer = false;
	e->accept();
}

template <class T>
void BaseWindow<T>::mousePressEvent(QMouseEvent* e)
{
	if(e->button() == Qt::LeftButton)
	{
        QPoint pos = e->globalPos();
        QRect windowGeoemtry = T::geometry();
        QRect titleGeometry = getTitleBar()->geometry();
        titleGeometry.moveTopLeft(getTitleBar()->parentWidget()->mapToGlobal(titleGeometry.topLeft()));
        qDebug() << "TitleBar geometry" << titleGeometry << "pressPos" << pos;
		if(inResizeZone && m_resizeMode == AllowResize)
		{
			allowToResize = true;

            if(pos.y() <= PIXELS_TO_ACT + windowGeoemtry.y())
			{
                if(pos.x() <= PIXELS_TO_ACT + windowGeoemtry.x())
				{
					resizeDiagSupEsq = true;
				}
                else if(pos.x() >= windowGeoemtry.x() + windowGeoemtry.width() - PIXELS_TO_ACT)
				{
					resizeDiagSupDer = true;
				}
				else
				{
					resizeVerSup = true;
				}
			}
            else if(pos.x() <= PIXELS_TO_ACT + windowGeoemtry.x())
			{
				resizeHorEsq = true;
			}
		}
        else if(titleGeometry.contains(pos))
		{
			moveWidget = true;
            dragPosition = e->globalPos() - T::frameGeometry().topLeft();
		}
	}

	e->accept();
}

template <class T>
void BaseWindow<T>::mouseMoveEvent(QMouseEvent* e)
{
	QPoint pos = e->globalPos();
	int xMouse = pos.x();
	int yMouse = pos.y();
    QRect windowGemetry = T::geometry();
	int wLeft = windowGemetry.left();
	int wTop = windowGemetry.top();
	int wWidth = windowGemetry.width();
	int wHeight = windowGemetry.height();
	bool isResizeEnabled = m_resizeMode == AllowResize;
    qDebug() << "mouseMoveEvent" << moveWidget << xMouse << yMouse << wWidth << wHeight << allowToResize << isResizeEnabled;

	if(moveWidget)
	{
		inResizeZone = false;
		QRect desktopGeometry = getMultyMonitorDesktopRect();

		if (!desktopGeometry.contains(pos))
		{
			if (xMouse > desktopGeometry.x() + desktopGeometry.width())
			{
				pos.setX(desktopGeometry.x() + desktopGeometry.width());
				QCursor::setPos(desktopGeometry.x() + desktopGeometry.width(), yMouse);
			}

			if (xMouse < desktopGeometry.x())
			{
				pos.setX(desktopGeometry.x());
				QCursor::setPos(desktopGeometry.x(), yMouse);
			}

			if (yMouse > desktopGeometry.y() + desktopGeometry.height())
			{
				pos.setX(desktopGeometry.y() + desktopGeometry.height());
				QCursor::setPos(xMouse, desktopGeometry.y() + desktopGeometry.height());
			}

			if (xMouse < desktopGeometry.y())
			{
				pos.setX(desktopGeometry.y());
				QCursor::setPos(xMouse, desktopGeometry.y());
			}
		}

		MoveType prevMoveType = m_moveType;
		m_moveType = detectMoveType(pos);
		//qDebug() << "MoveType" << m_moveType;

		switch (m_moveType)
		{
			case MT_None:
			{
				break;
			}

			case MT_NormalMove:
			{
				moveWindow(e);
				break;
			}

			case MT_ShowRightHalfDesktopPreview:
			{
				QRect halfDesctopRect = QApplication::desktop()->availableGeometry(this);
				halfDesctopRect.setX(halfDesctopRect.x() + halfDesctopRect.width() / 2);
				m_bIsMaximized = true;
				m_bIsHalfDeskMode = true;
				preMaximizeGeomentry = windowGemetry;
				setGeometry(halfDesctopRect);
				break;
			}

			case MT_ShowingRightHalfDesktopPreview:
			{
				e->ignore();
				break;
			}

			case MT_ShowLeftHalfDesktopPreview:
			{
				QRect halfDesctopRect = QApplication::desktop()->availableGeometry(this);;
				halfDesctopRect.setWidth(halfDesctopRect.width() / 2);
				m_bIsMaximized = true;
				m_bIsHalfDeskMode = true;
				preMaximizeGeomentry = windowGemetry;
				setGeometry(halfDesctopRect);
				break;
			}

			case MT_ShowingLeftHalfDesktopPreview:
			{
				e->ignore();
				break;
			}

			case MT_ShowMaximized:
			{
				maximizeBtnClicked();
				break;
			}

			case MT_ShowingMaximized:
			{
				break;
			}

			default:
				break;
		}
	}
	else if(allowToResize)
	{
		resizeWindow(e);
	}
	else if(isResizeEnabled && !m_bIsMaximized)
	{
#ifdef Q_WS_WIN
        if ((xMouse >= wLeft + wWidth - PIXELS_TO_ACT && xMouse <= wLeft + wWidth + PIXELS_TO_ACT) || allowToResize)
		{
			inResizeZone = true;

			if (yMouse >= wTop + wHeight - PIXELS_TO_ACT && yMouse <= wTop + wHeight + PIXELS_TO_ACT)
			{
                T::setCursor(Qt::SizeFDiagCursor);
			}
			else if (yMouse >= wTop - PIXELS_TO_ACT && yMouse <= wTop + PIXELS_TO_ACT)
			{
                T::setCursor(Qt::SizeBDiagCursor);
			}
			else
			{
                T::setCursor(Qt::SizeHorCursor);
			}

			resizeWindow(e);
		}
		//Cursor part esquerra
		else if ((xMouse >= wLeft - PIXELS_TO_ACT && xMouse <= wLeft + PIXELS_TO_ACT) || allowToResize)
		{
			inResizeZone = true;

			if (yMouse >= wTop + wHeight - PIXELS_TO_ACT && yMouse <= wTop + wHeight + PIXELS_TO_ACT)
			{
                T::setCursor(Qt::SizeBDiagCursor);
			}
			else if (yMouse >= wTop - PIXELS_TO_ACT && yMouse <= wTop + PIXELS_TO_ACT)
			{
                T::setCursor(Qt::SizeFDiagCursor);
			}
			else
			{
                T::setCursor(Qt::SizeHorCursor);
			}

            resizeWindow(e);
		}
		//Cursor part inferior
		else if ((yMouse >= wTop + wHeight - PIXELS_TO_ACT && yMouse <= wTop + wHeight + PIXELS_TO_ACT) || allowToResize)
		{
			inResizeZone = true;
            T::setCursor(Qt::SizeVerCursor);
			resizeWindow(e);
		}
		//Cursor part superior
		else if ((yMouse >= wTop - PIXELS_TO_ACT && yMouse <= wTop + PIXELS_TO_ACT) || allowToResize)
		{
			inResizeZone = true;
            T::setCursor(Qt::SizeVerCursor);
			resizeWindow(e);
		}
		else
		{
			inResizeZone = false;
            T::setCursor(Qt::ArrowCursor);
        }
#endif
	}

	e->accept();
}

