#include "peicedisplaywidget.h"
#include <QPainter>
#include <QDebug>
PeiceDisplayWidget::PeiceDisplayWidget(QWidget* parent)
	: QWidget(parent)
{
	this->setMinimumHeight(20);
	m_iPiceCount = 0;
	m_cDwonloaded = QColor(0x32, 0x91, 0xD4);
	m_cDownloading = QColor(0x52, 0xD3, 0x31);
	m_cBorder = QColor(0x90, 0x90, 0x90);
	m_cBackground = QColor(0xFF, 0xFF, 0xFF);
}

PeiceDisplayWidget::~PeiceDisplayWidget()
{
}

void PeiceDisplayWidget::clear()
{
	QImage img2(m_iPiceCount, 1, QImage::Format_RGB32);
	img2.fill(m_cBackground);
	m_dowloadingParts.clear();
	m_dowloadedParts.clear();
	m_image = img2;
}

void PeiceDisplayWidget::setProgress(const QBitArray& availibalePieces, const QBitArray& downloadingPieces)
{
	m_dowloadingParts = downloadingPieces;
	m_dowloadedParts = availibalePieces;
	m_iPiceCount = m_dowloadedParts.size();
	UpdateImage();
}

void PeiceDisplayWidget::paintEvent(QPaintEvent* paintEvent)
{
	QPainter painter(this);
	painter.fillRect(0, 0, width() - 1, height() - 1, m_cBackground);
	QRect imageRect(1, 1, width() - 2, height() - 2);

	if(!m_image.isNull())
	{
		if (m_image.width() != width() - 1)
		{
			UpdateImage();
		}

		painter.drawImage(imageRect, m_image);
	}

	painter.setPen(m_cBorder);
	painter.drawRect(0, 0, width() - 1, height() - 1);
}

int PeiceDisplayWidget::mixTwoColors(QRgb rgb1, QRgb rgb2, float ratio)
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

QVector<float> bitfieldToFloatVector(const QBitArray& vecin, int reqSize)
{
	QVector<float> result(reqSize, 0.0);

	if (vecin.isEmpty())
	{
		return result;
	}

	const float ratio = vecin.size() / (float)reqSize;

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
		const float fromR = float(x * vecin.size()) / reqSize;
		const float toR = float((x + 1) * vecin.size()) / reqSize;
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
				value += toR - fromR;
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
					value += 1.0 - (fromR - fromC);
				}

				++x2;
			}

			// subcase (16 >= x < 17)
			for (; x2 < toCMinusOne; ++x2)
			{
				if (vecin[x2])
				{
					value += 1.0;
				}
			}

			// subcase (17 >= x < 17.8)
			if (x2 == toCMinusOne)
			{
				if (vecin[x2])
				{
					value += 1.0 - (toC - toR);
				}

				++x2;
			}
		}

		// normalization <0, 1>
		value /= ratio;
		// float precision sometimes gives > 1, because in not possible to store irrational numbers
		value = qMin(value, (float)1.0);
		result[x] = value;
	}

	return result;
}

void PeiceDisplayWidget::UpdateImage()
{
	QImage image2(width() - 2, 1, QImage::Format_RGB888);

	if (m_dowloadedParts.isEmpty())
	{
		image2.fill(0xffffff);
		m_image = image2;
		update();
		return;
	}

	QVector<float> scaled_pieces = bitfieldToFloatVector(m_dowloadedParts, image2.width());
	QVector<float> scaled_pieces_dl = bitfieldToFloatVector(m_dowloadingParts, image2.width());

	// filling image
	for (int x = 0; x < scaled_pieces.size(); ++x)
	{
		float pieces2_val = scaled_pieces.at(x);
		float pieces2_val_dl = scaled_pieces_dl.at(x);

		if (pieces2_val_dl != 0)
		{
			float fill_ratio = pieces2_val + pieces2_val_dl;
			float ratio = pieces2_val_dl / fill_ratio;
			int mixedColor = mixTwoColors(m_cDownloading.rgb(), m_cDwonloaded.rgb(), ratio);
			mixedColor = mixTwoColors(m_cBackground.rgb(), mixedColor, fill_ratio);
			image2.setPixel(x, 0, mixedColor);
		}
		else
		{
			image2.setPixel(x, 0, mixTwoColors(m_cBackground.rgb(), m_cDwonloaded.rgb(), pieces2_val));
		}
	}

	m_image = image2;
}

QColor PeiceDisplayWidget::downloadedColor()
{
	return m_cDwonloaded;
}

QColor PeiceDisplayWidget::downloadingColor()
{
	return m_cDownloading;
}

void PeiceDisplayWidget::setDownloadedColor(QColor value)
{
	m_cDwonloaded = value;
}

void PeiceDisplayWidget::setDownloadingColor(QColor value)
{
	m_cDownloading = value;
}

QColor PeiceDisplayWidget::borderColor()
{
	return m_cBorder;
}

void PeiceDisplayWidget::setBorderColor(QColor value)
{
	m_cBorder = value;
}

QColor PeiceDisplayWidget::backgroudColor()
{
	return m_cBackground;
}

void PeiceDisplayWidget::setBackgroudColor(QColor value)
{
	m_cBackground = value;
}
