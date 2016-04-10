/*
Copyright 2010, David Sansome <me@davidsansome.com>
Copyright (C) 2005-2014 Mario Stephan <mstephan@shared-files.de>

This library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QMouseEvent>
#include <QStylePainter>
#include "RaitingWidget.h"
#include <QDebug>
#include <qmath.h>
const int RatingPainter::kStarCount;
int RatingPainter::kStarSize = 15;

RatingPainter::RatingPainter()
{
	//starPolygon << QPointF(1.0, 0.5);
	double a = 0;
	for (int i = 1; i < 12; ++i)
	{
		if (i % 2)
		{
			starPolygon << QPointF(0.5 + 0.5 * cos(a * M_PI / 180.0),
			                       0.5 + 0.5 * sin(a * M_PI / 180.0));
		}
		else
		{
			starPolygon << QPointF(0.5 + 0.25 * cos(a * M_PI / 180.0),
			                       0.5 + 0.25 * sin(a * M_PI / 180.0));
		}
		a = a + 180.0 / 5.0;
	}
}

QRect RatingPainter::Contents(const QRect& rect)
{
	const int width = rect.height() * kStarCount;
	return QRect(rect.x(), rect.y(), width, rect.height());
}

double RatingPainter::RatingForPos(const QPoint& pos, const QRect& rect)
{
	const QRect contents = Contents(rect);
	const double raw = double(pos.x() - contents.left()) / contents.width();

	// Round to the nearest 0.1
	double raiting = raw;
	if (raiting == 0.0)
	{
		raiting = -1.0;
	}
	if (raiting > 1.0)
	{
		raiting = 1.0;
	}
	return raiting;
}

void RatingPainter::Paint(QPainter* painter, const QRect& rect, float rating, QColor activeColor, QColor inactiveColor) const
{
	// save some time here

	if (rating == 0)
		return;

	rating *= kStarCount;
	QPixmap on(":/images/star-on.png");
	QPixmap off(":/images/star-off.png");
	kStarSize = rect.height();
	on = on.scaled(kStarSize, kStarSize);
	off = off.scaled(kStarSize, kStarSize);

	// Draw the stars
	//qDebug() << "Draw the stars";
	int x = 0;
	painter->save();
	painter->scale(kStarSize, kStarSize);

	for (int i = 0; i < kStarCount; i++ , x += kStarSize)
	{
		/*const QRect t_rect(x, 0, kStarSize, kStarSize);

		if (rating - 0.25 <= i)
		{
			// Totally empty
			//qDebug() << "Totally empty";
			painter->drawPixmap(t_rect, off);
		}
		else if (rating - 0.75 <= i)
		{
			// Half full
			//qDebug() << "Half full";
			const QRect target_left(t_rect.x(), t_rect.y(), kStarSize / 2, kStarSize);
			const QRect target_right(t_rect.x() + kStarSize / 2, t_rect.y(), kStarSize / 2, kStarSize);
			const QRect source_left(0, 0, kStarSize / 2, kStarSize);
			const QRect source_right(kStarSize / 2, 0, kStarSize / 2, kStarSize);
			painter->drawPixmap(target_left, on, source_left);
			painter->drawPixmap(target_right, off, source_right);
		}
		else
		{
			// Totally full
			//qDebug() << "Totally full";
			painter->drawPixmap(t_rect, on);
		}*/

		if (rating - i <= 1e-2)
		{
			// Totally empty
			//qDebug() << "Totally empty";
			painter->setBrush(QBrush(inactiveColor));
			painter->setPen(inactiveColor);
			painter->drawPolygon(starPolygon, Qt::WindingFill);
		}
		else if (rating - i <= 1.0)
		{
			// Half full
			float leftProportion = rating - i;
			float rightProportion = 1 - leftProportion;
			QRectF boundingRect = starPolygon.boundingRect();
			QRectF leftHalfRect = boundingRect;
			//qDebug() << "boundingRect " << leftHalfRect;
			leftHalfRect.setWidth(boundingRect.width() * leftProportion);
			QPolygonF leftRectPolygon;
			leftRectPolygon << leftHalfRect.bottomLeft() << leftHalfRect.topLeft() << leftHalfRect.topRight() << leftHalfRect.bottomRight() << leftHalfRect.bottomLeft();
			QRectF rightHalfRect = leftHalfRect;
			rightHalfRect.setX(rightHalfRect.x() + leftHalfRect.width());
			rightHalfRect.setWidth(boundingRect.width() * rightProportion);
			//qDebug() << "rightHalfRect " << rightHalfRect;
			QPolygonF rightRectPolygon;
			rightRectPolygon << rightHalfRect.bottomLeft() << rightHalfRect.topLeft() << rightHalfRect.topRight() << rightHalfRect.bottomRight() << rightHalfRect.bottomLeft();
			QPolygonF leftHalfStarPoligon = starPolygon.intersected(leftRectPolygon);
			QPolygonF rightHalfStarPoligon = starPolygon.intersected(rightRectPolygon);
			//qDebug() << "Half full";
			painter->setBrush(QBrush(activeColor));
			painter->setPen(activeColor);
			painter->drawPolygon(leftHalfStarPoligon, Qt::WindingFill);

			painter->setBrush(QBrush(inactiveColor));
			painter->setPen(inactiveColor);
			painter->drawPolygon(rightHalfStarPoligon, Qt::WindingFill);
		}
		else
		{
			// Totally full
			//qDebug() << "Totally full";
			painter->setBrush(QBrush(activeColor));
			painter->setPen(activeColor);
			painter->drawPolygon(starPolygon, Qt::WindingFill);
		}
		painter->translate(1.0, 0.0);
	}
	painter->restore();
}


RatingWidget::RatingWidget(QWidget* parent)
	: QWidget(parent),
	  rating_(-1.0),
	  hover_rating_(-1.0),
	  m_isReadonly(false),
	  m_activeColor(Qt::green),
	  m_inactiveColor(Qt::gray)
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setAttribute(Qt::WA_NoSystemBackground);
	setAttribute(Qt::WA_TranslucentBackground);
	setMouseTracking(true);
}

QSize RatingWidget::sizeHint() const
{
	QRect rect = this->rect();

	return QSize(rect.height() * (RatingPainter::kStarCount),
	             RatingPainter::kStarSize);
}

void RatingWidget::setRating(float rating)
{
	rating_ = rating;
	update();
}

void RatingWidget::setIsReadonly(bool isReadonly)
{
	m_isReadonly = isReadonly;
}

void RatingWidget::paintEvent(QPaintEvent* e)
{
	Q_UNUSED(e);
	QStylePainter p(this);
	// Draw the stars
	painter_.Paint(&p, rect(), hover_rating_ == -1.0 ? rating_ : hover_rating_, m_activeColor, m_inactiveColor);
}

void RatingWidget::mousePressEvent(QMouseEvent* e)
{
	if (m_isReadonly)
		return;
	rating_ = RatingPainter::RatingForPos(e->pos(), rect());
	emit RatingChanged(rating_);
	e->accept();
}

void RatingWidget::mouseMoveEvent(QMouseEvent* e)
{
	if (m_isReadonly)
		return;
	hover_rating_ = RatingPainter::RatingForPos(e->pos(), rect());
	update();
}

void RatingWidget::leaveEvent(QEvent*)
{
	if (m_isReadonly)
		return;
	hover_rating_ = -1.0;
	update();
}

QColor RatingWidget::activeColor() const
{
	return m_activeColor;
}

void RatingWidget::setActiveColor(const QColor& activeColor)
{
	m_activeColor = activeColor;
}

QColor RatingWidget::inactiveColor() const
{
	return m_inactiveColor;
}

void RatingWidget::setInactiveColor(const QColor& inactiveColor)
{
	m_inactiveColor = inactiveColor;
}

