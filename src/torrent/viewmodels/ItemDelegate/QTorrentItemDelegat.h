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
#include <StyledProgressBar.h>
#include <QPushButton>
#include <QEvent>
class Torrent;

class QTorrentItemDelegat: public QStyledItemDelegate
{
	Q_OBJECT
protected:
	QScopedPointer<StyledProgressBar> myProgressBarStyle;


protected:
	
	bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) override;
	QSize margin(const QStyle& style) const;
	QSize sizeHint(const QStyleOptionViewItem&, const Torrent&) const;

	void drawTorrent(QPainter* painter, const QStyleOptionViewItem& option, const Torrent&, const QModelIndex& index) const;

public:
	enum Buttons
	{
		Pause,
		Remove
	};
	QTorrentItemDelegat();
	
	QTorrentItemDelegat(const QTorrentItemDelegat&);
	explicit QTorrentItemDelegat(QObject* parent);
	virtual ~QTorrentItemDelegat();

	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
signals:
	void buttonClicked(QModelIndex index, QTorrentItemDelegat::Buttons button);
private:

	void HandleEvent(QEvent::Type type, Buttons pause, const QModelIndex& index);
	QString GetStatusString(const Torrent& tor) const;
	QString GetProgressString(const Torrent& tor) const;
	QList<QModelIndex> m_pressedIndex;
	QList<QStyle::State> m_buttonState;
	QIcon m_pauseIcon, m_startIcon, m_removeIcon;
	QScopedPointer<QPushButton> button;
private slots:
	void updateIcons();
};

Q_DECLARE_METATYPE(QTorrentItemDelegat)

#endif

