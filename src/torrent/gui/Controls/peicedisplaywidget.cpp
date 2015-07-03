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
	img = img2;
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
	if(!img.isNull())
	{
		painter.drawImage(imageRect, img);
	}

	painter.setPen(m_cBorder);
	painter.drawRect(0, 0, width() - 1, height() - 1);
}

void PeiceDisplayWidget::UpdateImage()
{
	QImage img2(m_iPiceCount, 1, QImage::Format_RGB32);
	
	img2.fill(m_cBackground);

	if(m_iPiceCount != 0)
	{
		double pieceItemWidth = width() * 1.0 / m_iPiceCount;
		double pieceItemHeight = height();
		int i = 0;

		for(i = 0; i < m_dowloadedParts.size(); i++)
		{
			if (m_dowloadedParts.at(i))
			{
				img2.setPixel(i, 0, m_cDwonloaded.rgb());
			}
			
		}

		for(i = 0; i < m_dowloadingParts.size(); i++)
		{
			if (m_dowloadingParts.at(i))
			{
				img2.setPixel(i, 0, m_cDownloading.rgb());
			}
			
		}
	}

	img = img2;
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
