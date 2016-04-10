#ifndef RATINGWIDGET_H
#define RATINGWIDGET_H

#include <QFrame>
#include <QPixmap>

class RatingWidget;

class RatingPainter
{
public:
	RatingPainter();

	static const int kStarCount = 5;
	static int kStarSize;
	static QRect Contents(const QRect& rect);
	static double RatingForPos(const QPoint& pos, const QRect& rect);

	void Paint(QPainter* painter, const QRect& rect, float rating, QColor activeColor, QColor inactiveColor) const;

private:
	QPolygonF starPolygon;
	QPixmap stars_[kStarCount * 2 + 1];
};

class RatingWidget : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(float rating READ rating WRITE setRating NOTIFY RatingChanged);
	Q_PROPERTY(bool isReadonly READ isReadonly WRITE setIsReadonly);
	Q_PROPERTY(QColor activeColor READ activeColor WRITE setActiveColor);
	Q_PROPERTY(QColor inactiveColor READ inactiveColor WRITE setInactiveColor);

public:
	RatingWidget(QWidget* parent = 0);

	QSize sizeHint() const override;

	float rating() const
	{
		return rating_;
	}

	void setRating(float rating);
	void setIsReadonly(bool isReadonly);

	bool isReadonly() const
	{
		return m_isReadonly;
	}

	QColor activeColor() const;
	void setActiveColor(const QColor& activeColor);
	QColor inactiveColor() const;
	void setInactiveColor(const QColor& inactiveColor);
	signals:
	void RatingChanged(float rating);


protected:
	void paintEvent(QPaintEvent*) override;
	void mousePressEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* e) override;
	void leaveEvent(QEvent*) override;

private:
	QColor m_activeColor, m_inactiveColor;

public:

private:
	RatingPainter painter_;
	float rating_;
	float hover_rating_;
	bool m_isReadonly;
};

#endif // RATINGWIDGET_H


