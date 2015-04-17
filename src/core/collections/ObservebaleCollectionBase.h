#ifndef _ObservebaleCollectionBase_INCLUDED_
#define _ObservebaleCollectionBase_INCLUDED_
#include <QObject>

class ObservebaleCollectionBase : public QObject
{
	Q_OBJECT
signals:
	void itemAdeed(int index);
	void itemsAdded(int startIndex, int count);
	void itemRemoved(int index);
	void itemsRemoved(int startIndex, int count);
	void itemMoved(int from, int to);
	void itemChanged(int index);
};
#endif
