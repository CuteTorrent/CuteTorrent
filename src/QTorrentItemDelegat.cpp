/*
CuteTorrent BitTorrent Client with dht support, userfriendly interface
and some additional features which make it more convenient.
Copyright (C) 2012 Ruslan Fedoseyenko

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "QTorrentItemDelegat.h"
#include <iostream>

#include <QApplication>
#include <QFont>
#include <QFontMetrics>
#include <QIcon>
#include <QModelIndex>
#include <QPainter>
#include <QPixmap>
#include <QPixmapCache>
#include <QStyleOptionProgressBarV2>
#include "QTorrentDisplayModel.h"
#include "Torrent.h"
#include <QDebug>


enum
{
	GUI_PAD = 6,
	BAR_HEIGHT = 12
};


QTorrentItemDelegat::QTorrentItemDelegat(const QTorrentItemDelegat& dlg) : QStyledItemDelegate(0) , myProgressBarStyle(new QProgressBar())
{
	myProgressBarStyle->setMinimum(0);
	myProgressBarStyle->setMaximum(100);
	myProgressBarStyle->setTextVisible(false);
	greenBrush = dlg.greenBrush;
	greenBack = dlg.greenBack;
	blueBrush = dlg.blueBrush;
	blueBack = dlg.blueBack;
	max_width = dlg.max_width;
}

QTorrentItemDelegat::QTorrentItemDelegat() : QStyledItemDelegate(0) , myProgressBarStyle(new QProgressBar()) {}

QColor QTorrentItemDelegat::greenBrush = QColor(81, 211, 49);
QColor QTorrentItemDelegat ::greenBack = QColor("darkseagreen");

QColor QTorrentItemDelegat ::blueBrush = QColor("steelblue");
QColor QTorrentItemDelegat ::blueBack = QColor("lightgrey");
int QTorrentItemDelegat::max_width = 0;
QTorrentItemDelegat::QTorrentItemDelegat(QObject* parent) :
	QStyledItemDelegate(parent),
	myProgressBarStyle(new QProgressBar())
{
	myProgressBarStyle->setMinimum(0);
	myProgressBarStyle->setMaximum(100);
	myProgressBarStyle->setTextVisible(false);
}

QTorrentItemDelegat::~QTorrentItemDelegat()
{
	delete myProgressBarStyle;
}



QSize QTorrentItemDelegat::margin(const QStyle& style) const
{
	Q_UNUSED(style);
	return QSize(4, 4);
}




namespace
{
int MAX3(int a, int b, int c)
{
	const int ab(a > b ? a : b);
	return ab > c ? ab : c;
}
}

QSize
QTorrentItemDelegat::sizeHint(const QStyleOptionViewItem& option, const Torrent& tor) const
{
	try
	{
		const QStyle* style(QApplication::style());
		const int iconSize(style->pixelMetric(QStyle::PM_MessageBoxIconSize));
		QFont nameFont(option.font);
		nameFont.setWeight(QFont::Bold);
		const QFontMetrics nameFM(nameFont);
		const QString nameStr(tor.GetName());
		int nameWidth = nameFM.width(nameStr);

		if(nameWidth > max_width)
		{
			nameWidth = max_width;
		}

		QFont statusFont(option.font);
		statusFont.setPointSize(int (option.font.pointSize() * 0.9));
		const QFontMetrics statusFM(statusFont);
		const QString statusStr(GetStatusString(tor));
		int statusWidth = statusFM.width(statusStr);

		if(statusWidth > max_width)
		{
			statusWidth = max_width;
		}

		QFont progressFont(statusFont);
		const QFontMetrics progressFM(progressFont);
		const QString progressStr(tor.GetProgresString());
		const int progressWidth = progressFM.width(progressStr);
		const QSize m(margin(*style));
		return QSize(m.width() + iconSize  + MAX3(nameWidth, statusWidth, progressWidth),
		             m.height() * 3 + nameFM.lineSpacing() /*+ statusFM.lineSpacing()*/ + 2 * GUI_PAD + progressFM.lineSpacing());
	}
	catch(...)
	{
		qDebug() << "exception in size hint";
	}

	return QSize(0, 0);
}

QSize
QTorrentItemDelegat::sizeHint(const QStyleOptionViewItem&   option,
                              const QModelIndex&            index) const
{
	try
	{
		Torrent*  tor(index.data(QTorrentDisplayModel::TorrentRole).value<Torrent*>());

		if(tor != NULL)
		{
			return sizeHint(option, *tor);
		}
		else
		{
			return QSize(0, 0);
		}
	}
	catch(...)
	{
		qDebug() << "Exception in QTorrentItemDelegat::sizeHint";
	}

	return QSize(0, 0);
}

void
QTorrentItemDelegat::paint(QPainter*                     painter,
                           const QStyleOptionViewItem&   option,
                           const QModelIndex&            index) const
{
	try
	{
		Torrent*  tor(index.data(QTorrentDisplayModel::TorrentRole).value<Torrent*>());
		painter->save();
		painter->setClipRect(option.rect);
		drawTorrent(painter, option, *tor, index);
	}
	catch(...)
	{
		qDebug() << "Excepion in QTorrentItemDelegat::paint";
	}

	painter->restore();
}

QString QTorrentItemDelegat::GetProgressString(const Torrent& tor) const
{
	if(tor.isDownloading())
	{
		return tr("%1 STR_DOWNLOADED %2 STR_FROM %3").arg(tor.GetProgresString()).arg(tor.GetTotalDownloaded()).arg(tor.GetTotalSize());
	}

	if(tor.isSeeding())
	{
		return tr("%1 - %3 STR_UPLOADED %2").arg(tor.GetProgresString()).arg(tor.GetTotalUploaded()).arg(tor.GetTotalSize());
	}

	return tor.GetProgresString();
}
QString QTorrentItemDelegat::GetStatusString(const Torrent& tor) const
{
	QString upSpeed(tor.GetUploadSpeed());
	QString downSpeed(tor.GetDwonloadSpeed());
	QString status(tor.GetStatusString());
	bool hasError(tor.hasError());

	if(hasError)
	{
		return tor.GetErrorMessage();
	}

	if(tor.isPaused())
	{
		return tr("STR_PAUSED");
	}

	if(tor.isDownloading())
	{
		return QString("%1: %2 %3 - %4 %5").arg(status).arg(QChar(0x2193)).arg(downSpeed).arg(QChar(0x2191)).arg(upSpeed);
	}

	if(tor.isSeeding())
	{
		if(!upSpeed.isEmpty())
		{
			return QString("%2 %3 - %1").arg(status).arg(QChar(0x2191)).arg(upSpeed);
		}
	}

	return status;
}
void QTorrentItemDelegat::drawTorrent(QPainter* painter, const QStyleOptionViewItem& option, const Torrent& tor, const QModelIndex& index) const
{
	QStyleOptionViewItemV4 opt = option;
	initStyleOption(&opt, index);
	QStyle* style;

	if(opt.widget != NULL)
	{
		style = opt.widget->style();
	}
	else
	{
		style = QApplication::style();
	}

	const int iconSize(style->pixelMetric(QStyle::PM_LargeIconSize));
	QFont nameFont(option.font);
	nameFont.setWeight(QFont::Bold);
	const QFontMetrics nameFM(nameFont);
	const QIcon mimeIcon(tor.GetMimeTypeIcon());
	QString nameStr(tor.GetName());
	QSize nameSize(nameFM.size(0, nameStr));

	if(nameSize.width() > max_width)
	{
		nameSize.setWidth(max_width);
	}

	QFont statusFont(option.font);
	statusFont.setPointSize(int (option.font.pointSize() * 0.9));
	const QFontMetrics statusFM(statusFont);
	const QString statusStr(GetStatusString(tor));
	QFont progressFont(statusFont);
	const QFontMetrics progressFM(progressFont);
	const QString progressStr(GetProgressString(tor));
	bool isPaused(tor.isPaused());
	painter->save();
	painter->setRenderHint(QPainter::Antialiasing);
	style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, opt.widget);
	/* if (option.state & QStyle::State_Selected) {
	     QPalette::ColorGroup cg = option.state & QStyle::State_Enabled
	             ? QPalette::Active : QPalette::Disabled;
	     if (cg == QPalette::Normal && !(option.state & QStyle::State_Active))
	         cg = QPalette::Inactive;


	    painter->fillRect(option.rect, opt.backgroundBrush);
	 }*/
	//style->drawControl(QStyle::CE_ItemViewItem,&option,painter);
	QIcon::Mode im;

	if(isPaused || !(option.state & QStyle::State_Enabled))
	{
		im = QIcon::Disabled;
	}
	else if(option.state & QStyle::State_Selected)
	{
		im = QIcon::Selected;
	}
	else
	{
		im = QIcon::Normal;
	}

	QIcon::State qs;

	if(isPaused)
	{
		qs = QIcon::Off;
	}
	else
	{
		qs = QIcon::On;
	}

	QPalette::ColorGroup cg = QPalette::Normal;

	if(isPaused || !(option.state & QStyle::State_Enabled))
	{
		cg = QPalette::Disabled;
	}

	if(cg == QPalette::Normal && !(option.state & QStyle::State_Active))
	{
		cg = QPalette::Inactive;
	}

	QPalette::ColorRole cr;

	if(option.state & QStyle::State_Selected)
	{
		cr = QPalette::HighlightedText;
	}

	cr = QPalette::Text;
	// layout
	const QSize m(margin(*style));
	QRect fillArea(option.rect);
	fillArea.adjust(m.width(), m.height(), -m.width(), -m.height());
	QRect iconArea(fillArea.x(), fillArea.y() + (fillArea.height() - iconSize) / 2, iconSize, iconSize);
	QRect nameArea(iconArea.x() + iconArea.width() + GUI_PAD, fillArea.y(),
	               fillArea.width() - GUI_PAD - iconArea.width(), nameSize.height());
	//nameArea.setWidth(max_width);
	QRect barArea(nameArea);
	barArea.setHeight(BAR_HEIGHT);
	barArea.moveTop(nameArea.y() + statusFM.lineSpacing() + GUI_PAD / 2);
	QRect progArea(nameArea);
	progArea.moveTop(barArea.y() + barArea.height() + GUI_PAD / 2);
	progArea.setWidth(barArea.width() / 2);
	QRect statusArea(barArea);
	statusArea.moveTop(barArea.y() + BAR_HEIGHT + GUI_PAD / 2);
	statusArea.moveLeft(barArea.width() / 2 + iconArea.width() + GUI_PAD);
	statusArea.setWidth(barArea.width() / 2);
	statusArea.setHeight(nameSize.height());

	//statusArea.setWidth(max_width);
	// render
	if(tor.hasError())
	{
		painter->setPen(QColor("red"));
	}
	else
	{
		painter->setPen(opt.palette.color(cg, cr));
	}

	mimeIcon.paint(painter, iconArea, Qt::AlignCenter, im, qs);
	painter->setFont(nameFont);
	//nameStr = nameFM.elidedText(nameStr, Qt::ElideRight, nameArea.width());
	style->drawItemText(painter, nameArea, Qt::AlignLeft, opt.palette, option.state & QStyle::State_Enabled, nameStr);
	painter->setFont(statusFont);
	style->drawItemText(painter,  statusArea, Qt::AlignRight, opt.palette, option.state & QStyle::State_Enabled, statusStr);
	painter->setFont(progressFont);
	style->drawItemText(painter, progArea, Qt::AlignLeft, opt.palette, option.state & QStyle::State_Enabled, progressStr);
	int progressPercentage = tor.GetProgress();
	// Customize style using style-sheet..
	QString stylestr =
	    QString("QProgressBar::chunk { background-color: qlineargradient(spread:reflect, x1:0.994318, y1:1, x2:1, y2:0.488636, stop:0.0397727 rgba(255, 255, 255, 255), stop:0.431818 rgba(") +
	    (isPaused ? "172,172,172" : (progressPercentage < 100 ? "81,211,49" : "50,145,212")) + ",255));}";
	myProgressBarStyle->resize(barArea.size());
	myProgressBarStyle->setValue(progressPercentage);
	myProgressBarStyle->setStyleSheet(stylestr);
	painter->translate(barArea.topLeft());
	myProgressBarStyle->render(painter);
	/*myProgressBarStyle->rect = barArea;
	if ( tor.isDownloading() ) {
	    myProgressBarStyle->palette.setBrush( QPalette::Highlight, blueBrush );
	    myProgressBarStyle->palette.setColor( QPalette::Base, blueBack );
	    myProgressBarStyle->palette.setColor( QPalette::Background, blueBack );
	}
	else if ( tor.isSeeding() ) {
	    myProgressBarStyle->palette.setBrush( QPalette::Highlight, greenBrush );
	    myProgressBarStyle->palette.setColor( QPalette::Base, greenBack );
	    myProgressBarStyle->palette.setColor( QPalette::Background, greenBack );
	}

	myProgressBarStyle->state = progressBarState;
	setProgressBarPercentDone( option, tor );

	style->drawControl( QStyle::CE_ProgressBar, myProgressBarStyle, painter );
	*/
	painter->restore();
}
