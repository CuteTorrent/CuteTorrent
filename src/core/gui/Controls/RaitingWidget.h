#ifndef RATINGWIDGET_H
#define RATINGWIDGET_H

#include <QFrame>
#include <QPixmap>

class RatingPainter {
public:
	RatingPainter();

	static const int kStarCount = 5;
	static int kStarSize;
	static QRect Contents(const QRect& rect);
	static double RatingForPos(const QPoint& pos, const QRect& rect);

	void Paint(QPainter* painter, const QRect& rect, float rating) const;

private:
	QPixmap stars_[kStarCount * 2 + 1];
};

class RatingWidget : public QWidget {
	Q_OBJECT
	Q_PROPERTY(float rating READ rating WRITE setRating);

public:
	RatingWidget(QWidget* parent = 0);

	QSize sizeHint() const;

	float rating() const { return rating_; }
	void setRating(float rating);

signals:
	void RatingChanged(float rating);

protected:
	void paintEvent(QPaintEvent*);
	void mousePressEvent(QMouseEvent* e);
	void mouseMoveEvent(QMouseEvent* e);
	void leaveEvent(QEvent*);

private:
	RatingPainter painter_;
	float rating_;
	float hover_rating_;
};

#endif // RATINGWIDGET_H