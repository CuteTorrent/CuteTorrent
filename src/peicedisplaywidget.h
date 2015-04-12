#ifndef PEICEDISPLAYWIDGET_H
#define PEICEDISPLAYWIDGET_H

#include <QtGui>
class PeiceDisplayWidget : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(QColor downloadedColor READ downloadedColor WRITE setDownloadedColor)
	Q_PROPERTY(QColor downloadingColor READ downloadingColor WRITE setDownloadingColor)
	Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
	Q_PROPERTY(QColor backgroudColor READ backgroudColor WRITE setBackgroudColor)
public:
	PeiceDisplayWidget(QWidget* parent = 0);
	~PeiceDisplayWidget();
	void setDowloadedParts(QVector<int> parts);
	void setDowloadingParts(QVector<int> parts);
	void setPiceCount(int);
	QColor downloadedColor();
	QColor downloadingColor();
	QColor borderColor();
	QColor backgroudColor();
	void setDownloadedColor(QColor value);
	void setDownloadingColor(QColor value);
	void setBorderColor(QColor value);
	void setBackgroudColor(QColor value);
protected:
	void paintEvent(QPaintEvent*);
private:
	void UpdateImage();
	QImage img;
	QColor m_cDwonloaded, m_cDownloading, m_cBorder, m_cBackground;
	int m_iPiceCount;
	QVector<QPair<int, int> > dowloadingParts;
	QVector<QPair<int, int> > dowloadedParts;

};

#endif // PEICEDISPLAYWIDGET_H
