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
#ifndef _ITEM_DELEGATE_H
#define _ITEM_DELEGATE_H

#include <QStyledItemDelegate>
#include <QSize>
#include <QStyle>
#include <QString>
#include <QStyleOptionViewItem>
#include <QPainter>
#include <QModelIndex>
#include "Torrent.h"
#include <QProgressBar>
class Torrent;

class QTorrentItemDelegat: public QStyledItemDelegate
{
	Q_OBJECT
protected:
	QProgressBar* myProgressBarStyle;



protected:

	QSize margin(const QStyle& style) const;
	QSize sizeHint(const QStyleOptionViewItem&, const Torrent&) const;

	void drawTorrent(QPainter* painter, const QStyleOptionViewItem& option, const Torrent&, const QModelIndex& index) const;
private:
	QString GetStatusString(const Torrent& tor) const;
	QString GetProgressString(const Torrent& tor) const;
public:
	QTorrentItemDelegat();
	QTorrentItemDelegat(const QTorrentItemDelegat&);
	static  QColor blueBrush, greenBrush;
	static QColor blueBack,  greenBack;
	explicit QTorrentItemDelegat(QObject* parent);
	virtual ~QTorrentItemDelegat();

	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
};

Q_DECLARE_METATYPE(QTorrentItemDelegat)

#endif
