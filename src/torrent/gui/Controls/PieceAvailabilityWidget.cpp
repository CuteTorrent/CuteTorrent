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
		painter.drawImage(imageRect, m_img);
	}
	painter.setPen(m_borderColor);
	painter.drawRect(0, 0, width() - 1, height() - 1);
	painter.restore();
	
}
/*
QSize PieceAvailabilityWidget::minimumSizeHint()
{
	QStyleOption opt;
	opt.initFrom(this);
	
	QString avalStr = "8.88";

	QFontMetrics fontMetrics(opt.fontMetrics);

	int strWidht = fontMetrics.width(avalStr);
	int height = fontMetrics.height();
	return QSize(GUI_PAD * 2 + strWidht, height*2);
}
*/

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
	updateImage();
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
	QImage img(width() - 1, height() - 1, QImage::Format_RGB32);
	img.fill(m_backgroundColor);
	m_img = img;
}

void PieceAvailabilityWidget::updateImage()
{
	QImage img(m_pieceAvailibility.size(), 1, QImage::Format_RGB32);
	img.fill(m_backgroundColor);
	

	size_t size = m_pieceAvailibility.size();
	if (size > 0 && m_maxAvailibility > 0 && std::count(m_pieceAvailibility.begin(), m_pieceAvailibility.end(), 0) != size)
	{
		for (int i = 0; i < size; i++)
		{
			int availibility = m_pieceAvailibility[i];
			float ratio = float(availibility) / m_maxAvailibility;
			
			QColor currentItemColor = m_pieceColor;
			currentItemColor.setRed(ratio * currentItemColor.red() + m_backgroundColor.red() * (1.0 - ratio));
			currentItemColor.setBlue(ratio * currentItemColor.blue() + m_backgroundColor.blue() * (1.0 - ratio));
			currentItemColor.setGreen(ratio * currentItemColor.green() + m_backgroundColor.green() * (1.0 - ratio));
			
			img.setPixel(i,0, currentItemColor.rgb());
		}
	}
	m_img = img;
}