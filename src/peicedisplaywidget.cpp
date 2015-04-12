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

void PeiceDisplayWidget::paintEvent(QPaintEvent* paintEvent)
{
	QPainter painter(this);

	if(img.isNull())
	{
		painter.fillRect(0, 0, width() - 1, height() - 1, m_cBackground);
	}
	else
	{
		painter.drawImage(0, 0, img);
	}

	painter.setPen(m_cBorder);
	painter.drawRect(0, 0, width() - 1, height() - 1);
}

void PeiceDisplayWidget::UpdateImage()
{
	QImage img2(width() - 1, height() - 1, QImage::Format_RGB32);
	QPainter painter(&img2);
	painter.fillRect(0, 0, width() - 1, height() - 1, m_cBackground);

	if(m_iPiceCount != 0)
	{
		double pieceItemWidth = width() * 1.0 / m_iPiceCount;
		double pieceItemHeight = height();
		int i = 0;

		for(i = 0; i < dowloadedParts.size(); i++)
		{
			QRectF currentRect(dowloadedParts[i].first * pieceItemWidth, 0, (dowloadedParts[i].second - dowloadedParts[i].first + 1) *pieceItemWidth, pieceItemHeight);
			painter.fillRect(currentRect, m_cDwonloaded);
		}

		for(i = 0; i < dowloadingParts.size(); i++)
		{
			QRectF currentRect(dowloadingParts[i].first * pieceItemWidth, 0, (dowloadingParts[i].second - dowloadingParts[i].first + 1) *pieceItemWidth, pieceItemHeight);
			painter.fillRect(currentRect, m_cDownloading);
		}
	}

	img = img2;
}

void PeiceDisplayWidget::setPiceCount(int _pieceCount)
{
	m_iPiceCount = _pieceCount;
	UpdateImage();
}

void PeiceDisplayWidget::setDowloadingParts(QVector<int> parts)
{
	dowloadingParts.clear();
	int previusPicNumber = -1;

	foreach(int piceNumber, parts)
	{
		if(piceNumber - previusPicNumber == 1)
		{
			dowloadingParts.last().second = piceNumber;
		}
		else
		{
			dowloadingParts.append(qMakePair(piceNumber, piceNumber + 1));
		}

		previusPicNumber = piceNumber;
	}
}

void PeiceDisplayWidget::setDowloadedParts(QVector<int> parts)
{
	dowloadedParts.clear();
	int previusPicNumber = 0;

	foreach(int piceNumber, parts)
	{
		if(piceNumber - previusPicNumber == 1)
		{
			dowloadedParts.last().second = piceNumber;
		}
		else
		{
			dowloadedParts.append(qMakePair(piceNumber, piceNumber + 1));
		}

		previusPicNumber = piceNumber;
	}
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
