#include "PieceAvailabilityWidget.h"
#include <QPainter>
#include <QStyleOption>
#include <QApplication>
#include "StaticHelpers.h"

PieceAvailabilityWidget::PieceAvailabilityWidget(QObject* parent) : m_pieceColor(0x32, 0x91, 0xD4), m_borderColor(0x90, 0x90, 0x90), m_backgroundColor(Qt::white)
{
	m_pieceAvailibility.clear();
	m_maxAvailibility = 0;
}


void PieceAvailabilityWidget::paintEvent(QPaintEvent* paintEvent)
{
	QPainter painter(this);
	QStyleOption opt;
	opt.initFrom(this);
	QStyle* style = QApplication::style();
	style->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
	painter.save();
	QRect imageRect(1, 1, width() - 2, height() - 2);
	painter.fillRect(0, 0, width() - 1, height() - 1, m_backgroundColor);

	if (!m_img.isNull())
	{
		if (m_img.width() != width() - 2)
		{
			UpdateImage();
		}

		painter.drawImage(imageRect, m_img);
	}

	painter.setPen(m_borderColor);
	painter.drawRect(0, 0, width() - 1, height() - 1);
	painter.restore();
}

QColor PieceAvailabilityWidget::pieceColor()
{
	return m_pieceColor;
}

QColor PieceAvailabilityWidget::borderColor()
{
	return m_borderColor;
}

QColor PieceAvailabilityWidget::backgroudColor()
{
	return m_backgroundColor;
}

void PieceAvailabilityWidget::setPiceAvailability(std::vector<int> availiblePieces)
{
	m_pieceAvailibility.swap(availiblePieces);

	if (!m_pieceAvailibility.empty())
	{
		m_maxAvailibility = *max_element(m_pieceAvailibility.begin(), m_pieceAvailibility.end());
	}

	UpdateImage();
}

void PieceAvailabilityWidget::setBorderColor(QColor value)
{
	if (m_borderColor != value)
	{
		m_borderColor = value;
	}
}

void PieceAvailabilityWidget::setPieceColor(QColor value)
{
	if (m_pieceColor != value)
	{
		m_pieceColor = value;
	}
}

void PieceAvailabilityWidget::setBackgroudColor(QColor value)
{
	if (m_backgroundColor != value)
	{
		m_backgroundColor = value;
	}
}

void PieceAvailabilityWidget::clear()
{
	QImage img(width() - 2, 1, QImage::Format_RGB888);
	img.fill(m_backgroundColor);
	m_pieceAvailibility.clear();
	m_img = img;
}

QVector<float> PieceAvailabilityWidget::intToFloatVector(const std::vector<int>& vecin, int reqSize)
{
	QVector<float> result(reqSize, 0.0);

	if (vecin.empty())
	{
		return result;
	}

	const float ratio = vecin.size() / float(reqSize);
	const int maxElement = *std::max_element(vecin.begin(), vecin.end());

	// qMax because in normalization we don't want divide by 0
	// if maxElement == 0 check will be disabled please enable this line:
	// const int maxElement = qMax(*std::max_element(avail.begin(), avail.end()), 1);

	if (maxElement == 0)
	{
		return result;
	}

	// simple linear transformation algorithm
	// for example:
	// image.x(0) = pieces.x(0.0 >= x < 1.7)
	// image.x(1) = pieces.x(1.7 >= x < 3.4)

	for (int x = 0; x < reqSize; ++x)
	{
		// don't use previously calculated value "ratio" here!!!
		// float cannot save irrational number like 7/9, if this number will be rounded up by std::ceil
		// give you x2 == pieces.size(), and index out of range: pieces[x2]
		// this code is safe, so keep that in mind when you try optimize more.
		// tested with size = 3000000ul
		// R - real
		const float fromR = float(x * vecin.size()) / float(reqSize);
		const float toR = float((x + 1) * vecin.size()) / float(reqSize);
		// C - integer
		int fromC = fromR;// std::floor not needed
		int toC = std::ceil(toR);
		// position in pieces table
		// libtorrent::bitfield::m_size is unsigned int(31 bits), so qlonglong is not needed
		// tested with size = 3000000ul
		int x2 = fromC;
		// little speed up for really big pieces table, 10K+ size
		const int toCMinusOne = toC - 1;
		// value in returned vector
		float value = 0;

		// case when calculated range is (15.2 >= x < 15.7)
		if (x2 == toCMinusOne)
		{
			if (vecin[x2])
			{
				value += (toR - fromR) * vecin[x2];
			}

			++x2;
		}
		// case when (15.2 >= x < 17.8)
		else
		{
			// subcase (15.2 >= x < 16)
			if (x2 != fromR)
			{
				if (vecin[x2])
				{
					value += (1.0 - (fromR - fromC)) * vecin[x2];
				}

				++x2;
			}

			// subcase (16 >= x < 17)
			for (; x2 < toCMinusOne; ++x2)
			{
				if (vecin[x2])
				{
					value += vecin[x2];
				}
			}

			// subcase (17 >= x < 17.8)
			if (x2 == toCMinusOne)
			{
				if (vecin[x2])
				{
					value += (1.0 - (toC - toR)) * vecin[x2];
				}

				++x2;
			}
		}

		// normalization <0, 1>
		value /= ratio * maxElement;
		// float precision sometimes gives > 1, because in not possible to store irrational numbers
		value = qMin(value, float(1.0));
		result[x] = value;
	}

	return result;
}

int PieceAvailabilityWidget::mixTwoColors(const int& rgb1, const int& rgb2, float ratio)
{
	int r1 = qRed(rgb1);
	int g1 = qGreen(rgb1);
	int b1 = qBlue(rgb1);
	int r2 = qRed(rgb2);
	int g2 = qGreen(rgb2);
	int b2 = qBlue(rgb2);
	float ratio_n = 1.0 - ratio;
	int r = (r1 * ratio_n) + (r2 * ratio);
	int g = (g1 * ratio_n) + (g2 * ratio);
	int b = (b1 * ratio_n) + (b2 * ratio);
	return qRgb(r, g, b);
}

void PieceAvailabilityWidget::UpdateImage()
{
	//  qDebug() << "updateImageAv";
	QImage image2(width() - 2, 1, QImage::Format_RGB888);

	if (m_pieceAvailibility.empty())
	{
		image2.fill(m_backgroundColor);
		m_img = image2;
		update();
		return;
	}

	QVector<float> scaled_pieces = intToFloatVector(m_pieceAvailibility, image2.width());

	// filling image
	for (int x = 0; x < scaled_pieces.size(); ++x)
	{
		float pieces2_val = scaled_pieces.at(x);
		int rgb2 = m_pieceColor.rgb();
		int rgb1 = m_backgroundColor.rgb();
		image2.setPixel(x, 0, mixTwoColors(rgb1, rgb2, pieces2_val));
	}

	m_img = image2;
}