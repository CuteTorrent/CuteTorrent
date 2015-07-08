#ifndef _PIECE_AVAL_WIDGET_
#define _PIECE_AVAL_WIDGET_
#include <QWidget>

class PieceAvailabilityWidget : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(QColor PieceColor READ pieceColor WRITE setPieceColor)
	Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
	Q_PROPERTY(QColor backgroudColor READ backgroudColor WRITE setBackgroudColor)
	static const int GUI_PAD = 4;

protected:
	void paintEvent(QPaintEvent*) override;
//	QSize minimumSizeHint();
public:
	explicit PieceAvailabilityWidget(QObject* parent = NULL);
	QColor pieceColor();
	QColor borderColor();
	QColor backgroudColor();
	void setPiceAvailability(std::vector<int> availiblePieces);

public slots:
	void setBorderColor(QColor value);
	void setPieceColor(QColor newValue);
	void setBackgroudColor(QColor value);
	void clear();
	QVector<float> intToFloatVector(const std::vector<int>& vecin, int reqSize);
	int mixTwoColors(int& rgb1, int& rgb2, float ratio);
private:
	QImage m_img;
	QColor m_pieceColor, m_borderColor;
	std::vector<int> m_pieceAvailibility;
	int m_maxAvailibility;
	float m_totalAvailibility;
	QColor m_backgroundColor;
	void updateImage();
};

#endif