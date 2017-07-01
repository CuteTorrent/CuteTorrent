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
#include <QSize>
#include <QStyle>
#include <QString>
#include <QStyleOptionProgressBarV2>
#include "QTorrentDisplayModel.h"
#include <QDebug>
#include "StyleEngene.h"

enum
{
	GUI_PAD = 6,
	BAR_HEIGHT = 12
};


QTorrentItemDelegat::QTorrentItemDelegat(const QTorrentItemDelegat& dlg) : QStyledItemDelegate(0), myProgressBarStyle(new StyledProgressBar()), button(new QPushButton())
{
	myProgressBarStyle->setMinimum(0);
	myProgressBarStyle->setMaximum(100);
	myProgressBarStyle->setTextVisible(false);
	updateIcons();
	m_pressedIndex.append(QModelIndex());
	m_pressedIndex.append(QModelIndex());
	m_buttonState.append(QStyle::State_Enabled);
	m_buttonState.append(QStyle::State_Enabled);
}

QTorrentItemDelegat::QTorrentItemDelegat() : QStyledItemDelegate(0), myProgressBarStyle(new StyledProgressBar()), button(new QPushButton())
{
	myProgressBarStyle->setMinimum(0);
	myProgressBarStyle->setMaximum(100);
	myProgressBarStyle->setTextVisible(false);

	updateIcons();
	m_pressedIndex.append(QModelIndex());
	m_pressedIndex.append(QModelIndex());
	m_buttonState.append(QStyle::State_Enabled);
	m_buttonState.append(QStyle::State_Enabled);
}


void QTorrentItemDelegat::updateIcons()
{
	StyleEngene* styleEngene = StyleEngene::getInstance();
	m_removeIcon = styleEngene->getIcon("toolbar_remove");
	m_startIcon = styleEngene->getIcon("toolbar_start");
	m_pauseIcon = styleEngene->getIcon("toolbar_pause");
}

QTorrentItemDelegat::QTorrentItemDelegat(QObject* parent) :
	QStyledItemDelegate(parent),
	myProgressBarStyle(new StyledProgressBar()),
	button(new QPushButton())
{
	myProgressBarStyle->setMinimum(0);
	myProgressBarStyle->setMaximum(100);
	myProgressBarStyle->setTextVisible(false);
	StyleEngene* styleEngene = StyleEngene::getInstance();
	connect(styleEngene, SIGNAL(styleChanged()), SLOT(updateIcons()));
	updateIcons();
	m_pressedIndex.append(QModelIndex());
	m_pressedIndex.append(QModelIndex());
	m_buttonState.append(QStyle::State_Enabled);
	m_buttonState.append(QStyle::State_Enabled);
}

QTorrentItemDelegat::~QTorrentItemDelegat()
{
	
}


bool QTorrentItemDelegat::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
	qDebug() << "QTorrentItemDelegat::editorEvent" << event->type();
	if (event->type() == QEvent::MouseButtonPress ||
		event->type() == QEvent::MouseButtonRelease)
	{
		
	}
	else
	{
		//ignoring other mouse event and reseting button's state
		m_buttonState[Remove] = QStyle::State_Raised;
		m_buttonState[Pause] = QStyle::State_Raised;
		m_pressedIndex[Remove] = QModelIndex();
		m_pressedIndex[Pause] = QModelIndex();
		return false;
	}

	QStyle* style;
	QStyleOptionViewItemV4 opt = option;
	initStyleOption(&opt, index);
	if (opt.widget != NULL)
	{
		style = opt.widget->style();
	}
	else
	{
		style = QApplication::style();
	}

	
	QSize m(4, 4);
	
	QRect fillArea(option.rect);
	fillArea.adjust(m.width(), m.height(), -m.width(), -m.height());
	const int iconSize(style->pixelMetric(QStyle::PM_MessageBoxIconSize));
	QRect iconArea(fillArea.x(), fillArea.y() + (fillArea.height() - iconSize) / 2, iconSize, iconSize);
	
	QFont statusFont(option.font);
	statusFont.setPointSize(int(option.font.pointSize() * 0.9));
	const QFontMetrics statusFM(statusFont);
	QRect nameArea(iconArea.x() + iconArea.width() + GUI_PAD, fillArea.y(),
		fillArea.width() - GUI_PAD - iconArea.width(), statusFM.height());

	if (nameArea.width() + nameArea.x() > opt.rect.width())
	{
		nameArea.setWidth(opt.rect.width() - nameArea.x());
	}
	const int buttonSize(style->pixelMetric(QStyle::PM_ListViewIconSize));
	QRect barArea(nameArea.x(), nameArea.y() + statusFM.lineSpacing() + GUI_PAD / 2, nameArea.width() - 2 * (buttonSize + m.width()), BAR_HEIGHT);
	QRect pauseButtonRect(barArea.x() + barArea.width() + m.width(), barArea.y(), buttonSize, buttonSize);
	QRect removeButtonRect(pauseButtonRect.x() + pauseButtonRect.width() + m.width(), pauseButtonRect.y(), buttonSize, buttonSize);
	qDebug() << index << "editorEvent" << pauseButtonRect << removeButtonRect;
	QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
	if (pauseButtonRect.contains(mouseEvent->pos()))
	{
		HandleEvent(mouseEvent->type(), Pause, index);
	}

	if (removeButtonRect.contains(mouseEvent->pos()))
	{
		HandleEvent(mouseEvent->type(), Remove, index);
	}
	return QStyledItemDelegate::editorEvent(event, model, option, index);
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
	const QStyle* style(QApplication::style());
	const int mainIconSize(style->pixelMetric(QStyle::PM_MessageBoxIconSize));
	const int smallIconSize(style->pixelMetric(QStyle::PM_SmallIconSize));
	QFont nameFont(option.font);
	nameFont.setWeight(QFont::Bold);
	const QFontMetrics nameFM(nameFont);
	QString nameStr;
	int queuePosition = tor.GetQueuePosition();

	if (queuePosition > -1)
	{
		nameStr = QString("#%1 %2").arg(QString::number(queuePosition + 1), tor.GetName());
	}
	else
	{
		nameStr = tor.GetName();
	}
	QString timeStr(tor.GetRemainingTimeStr());
	int nameWidth = nameFM.width(nameStr);
	QFont statusFont(option.font);
	statusFont.setPointSize(int(option.font.pointSize() * 0.9));
	const QFontMetrics statusFM(statusFont);
	QSize timeSize(statusFM.size(0, timeStr));
	const QString statusStr(GetStatusString(tor));
	int statusWidth = statusFM.width(statusStr);
	QFont progressFont(statusFont);
	const QFontMetrics progressFM(progressFont);
	const QString progressStr(GetProgressString(tor));
	const int progressWidth = progressFM.width(progressStr);
	const QSize m(margin(*style));
	int width = m.width() + mainIconSize + std::max(nameWidth + timeSize.width(), statusWidth + progressWidth);
	int height =m.height() * 3 + nameFM.lineSpacing() + 2 * GUI_PAD + progressFM.lineSpacing();
	return QSize(width, height);
}

QSize
QTorrentItemDelegat::sizeHint(const QStyleOptionViewItem& option,
                              const QModelIndex& index) const
{
	try
	{
		Torrent* tor(index.data(QTorrentDisplayModel::TorrentRole).value<Torrent*>());
		initStyleOption(const_cast<QStyleOptionViewItem*>(&option), index);

		if (tor != NULL)
		{
			return sizeHint(option, *tor);
		}
		else
		{
			return QSize(0, 0);
		}
	}
	catch (...)
	{
		qDebug() << "Exception in QTorrentItemDelegat::sizeHint";
	}

	return QSize(0, 0);
}

void
QTorrentItemDelegat::paint(QPainter* painter,
                           const QStyleOptionViewItem& option,
                           const QModelIndex& index) const
{
	try
	{
		Torrent* tor(index.data(QTorrentDisplayModel::TorrentRole).value<Torrent*>());
		painter->save();
		painter->setClipRect(option.rect);
		drawTorrent(painter, option, *tor, index);
	}
	catch (...)
	{
		qDebug() << "Excepion in QTorrentItemDelegat::paint";
	}

	painter->restore();
}

QString QTorrentItemDelegat::GetProgressString(const Torrent& tor) const
{
	if (tor.isDownloading())
	{
		return tr("%1 STR_DOWNLOADED %2 STR_FROM %3").arg(tor.GetProgresString()).arg(tor.GetTotalDownloadedStr()).arg(tor.GetTotalSizeStr());
	}

	if (tor.isSeeding())
	{
		return tr("%1 - %3 STR_UPLOADED %2").arg(tor.GetProgresString()).arg(tor.GetTotalUploadedStr()).arg(tor.GetTotalSizeStr());
	}

	return tor.GetProgresString();
}

QString QTorrentItemDelegat::GetStatusString(const Torrent& tor) const
{
	QString upSpeed(tor.GetUploadSpeedStr());
	QString downSpeed(tor.GetDwonloadSpeedStr());
	QString status(tor.GetStatusString());
	bool hasError(tor.hasError());

	if (hasError)
	{
		return tor.GetErrorMessage();
	}

	if (tor.isPaused())
	{
		return tr("STR_PAUSED");
	}

	if (tor.isDownloading())
	{
		return QString("%1: %2 %3 - %4 %5").arg(status).arg(QChar(0x2193)).arg(downSpeed).arg(QChar(0x2191)).arg(upSpeed);
	}

	if (tor.isSeeding())
	{
		if (!upSpeed.isEmpty())
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

	if (opt.widget != NULL)
	{
		style = opt.widget->style();
	}
	else
	{
		style = QApplication::style();
	}

	const int mainIconSize(style->pixelMetric(QStyle::PM_MessageBoxIconSize));
	const int smallIconSize(style->pixelMetric(QStyle::PM_SmallIconSize));
	QFont nameFont(option.font);
	nameFont.setWeight(QFont::Bold);
	const QFontMetrics nameFM(nameFont);
	const QIcon mimeIcon(tor.GetMimeTypeIcon());
	QString timeStr(tor.GetRemainingTimeStr());
	QString nameStr;
	int queuePosition = tor.GetQueuePosition();

	if (queuePosition > -1)
	{
		nameStr = QString("#%1 %2").arg(QString::number(queuePosition + 1), tor.GetName());
	}
	else
	{
		nameStr = tor.GetName();
	}

	QSize nameSize(nameFM.size(0, nameStr));
	QFont statusFont(option.font);
	statusFont.setPointSize(int(option.font.pointSize() * 0.9));

	const QFontMetrics statusFM(statusFont);
	QSize timeSize(statusFM.size(0, timeStr));
	QString statusStr(GetStatusString(tor));
	int statusWidth = statusFM.width(statusStr);
	QFont progressFont(statusFont);
	const QFontMetrics progressFM(progressFont);
	QString progressStr(GetProgressString(tor));
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

	if (isPaused || !(option.state & QStyle::State_Enabled))
	{
		im = QIcon::Disabled;
	}
	else if (option.state & QStyle::State_Selected)
	{
		im = QIcon::Selected;
	}
	else
	{
		im = QIcon::Normal;
	}

	QIcon::State qs;

	if (isPaused)
	{
		qs = QIcon::Off;
	}
	else
	{
		qs = QIcon::On;
	}

	QPalette::ColorGroup cg = QPalette::Normal;

	if (isPaused || !(option.state & QStyle::State_Enabled))
	{
		cg = QPalette::Disabled;
	}

	if (cg == QPalette::Normal && !(option.state & QStyle::State_Active))
	{
		cg = QPalette::Inactive;
	}

	QPalette::ColorRole cr;

	if (option.state & QStyle::State_Selected)
	{
		cr = QPalette::HighlightedText;
	}

	cr = QPalette::Text;
	// layout
	const QSize m(margin(*style));
	QRect fillArea(option.rect);
	fillArea.adjust(m.width(), m.height(), -m.width(), -m.height());
	QRect iconArea(fillArea.x(), fillArea.y() + (fillArea.height() - mainIconSize) / 2, mainIconSize, mainIconSize);
	QRect timeArea(fillArea.x() + (fillArea.width() - timeSize.width()), fillArea.y(), timeSize.width(), timeSize.height());
	int progressPercentage = tor.GetProgress();
	bool isDownloaded = (progressPercentage == 100);
	if (isDownloaded)
	{
		timeArea.setWidth(0);
	}
	QRect nameArea(iconArea.x() + iconArea.width() + GUI_PAD, fillArea.y(),
	               fillArea.width() - GUI_PAD - iconArea.width() - timeArea.width(), nameSize.height());

	if (nameArea.width() + nameArea.x() > opt.rect.width())
	{
		nameArea.setWidth(opt.rect.width() - nameArea.x());
	}
	const int buttonSize(BAR_HEIGHT);
	QRect barArea(nameArea.x(), nameArea.y() + statusFM.lineSpacing() + GUI_PAD / 2, nameArea.width() + timeArea.width() - 2 * (buttonSize + 2*m.width()), BAR_HEIGHT);
	QRect pauseButtonRect(barArea.x() + barArea.width() + 2 * m.width(), barArea.y(), buttonSize, buttonSize);
	QRect removeButtonRect(pauseButtonRect.x() + pauseButtonRect.width() + 2 * m.width(), pauseButtonRect.y(), buttonSize, buttonSize);
	QRect progArea(nameArea.x(), barArea.y() + barArea.height() + GUI_PAD / 2, barArea.width() + 2 * (buttonSize + 2 * m.width()) - statusWidth, nameArea.height());
	QRect statusArea(progArea.x() + progArea.width(), progArea.y(), statusWidth, progArea.height());
	if (tor.hasError())
	{
		painter->setPen(QColor("red"));
	}
	else
	{
		painter->setPen(opt.palette.color(cg, cr));
	}

	mimeIcon.paint(painter, iconArea, Qt::AlignCenter, im, qs);
	painter->setFont(nameFont);
	nameStr = nameFM.elidedText(nameStr, Qt::ElideRight, nameArea.width());
	style->drawItemText(painter, nameArea, Qt::AlignLeft, opt.palette, option.state & QStyle::State_Enabled, nameStr);
	painter->setFont(statusFont);
	if (!isDownloaded)
		style->drawItemText(painter, timeArea, Qt::AlignRight, opt.palette, option.state & QStyle::State_Enabled, timeStr);
	style->drawItemText(painter, statusArea, Qt::AlignRight, opt.palette, option.state & QStyle::State_Enabled, statusStr);
	painter->setFont(progressFont);
	progressStr = statusFM.elidedText(progressStr, Qt::ElideRight, progArea.width());
	style->drawItemText(painter, progArea, Qt::AlignLeft, opt.palette, option.state & QStyle::State_Enabled, progressStr);
	QStyleOptionButton styleOptionButton;

	styleOptionButton.features = QStyleOptionButton::Flat;
	styleOptionButton.rect = pauseButtonRect;
	styleOptionButton.icon = isPaused ? m_startIcon : m_pauseIcon;
	styleOptionButton.iconSize = QSize(buttonSize, buttonSize);
	styleOptionButton.state = QStyle::State_Enabled;
	if (m_buttonState[Pause] == QStyle::State_Sunken && m_pressedIndex[Pause].isValid() && m_pressedIndex[Pause] == index)

	{
		styleOptionButton.state |= m_buttonState[Pause];
	}
	style->drawControl(QStyle::CE_PushButton, &styleOptionButton, painter,
		button.data());
	
	styleOptionButton.rect = removeButtonRect;
	styleOptionButton.icon = m_removeIcon;
	styleOptionButton.state = QStyle::State_Enabled;
	if (m_buttonState[Remove] == QStyle::State_Sunken && m_pressedIndex[Remove].isValid() && m_pressedIndex[Remove] == index)

	{
		styleOptionButton.state |= m_buttonState[Remove];
	}
	style->drawControl(QStyle::CE_PushButton, &styleOptionButton, painter,
		button.data());

	myProgressBarStyle->resize(barArea.size());
	myProgressBarStyle->setValue(progressPercentage);
	QStyleOptionProgressBarV2 pbStyleOpt;
	myProgressBarStyle->initilizeStyleOption(&pbStyleOpt);
	pbStyleOpt.rect = barArea;
	style->drawControl(QStyle::CE_ProgressBar, &pbStyleOpt, painter, myProgressBarStyle.data());
	painter->restore();
}

void QTorrentItemDelegat::HandleEvent(QEvent::Type type, Buttons button, const QModelIndex& index)
{
	if (type == QEvent::MouseButtonPress) {
		m_pressedIndex[button] = index;
		m_buttonState[button] = QStyle::State_Sunken;
	}
	else if (type == QEvent::MouseButtonRelease) {
		m_buttonState[button] = QStyle::State_Raised;
		m_pressedIndex[button] = QModelIndex();
		emit buttonClicked(index, button);
	}
}
