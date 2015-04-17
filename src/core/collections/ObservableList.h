#ifndef _ObservableList_INCLUDED_
#define _ ObservableList_INCLUDED_
#include "ObservebaleCollectionBase.h"
#include <QList>
template<typename T>
class ObservableList : public ObservebaleCollectionBase, public QList<T>
{
public:
	void append(const T &t);
	void append(const QList<T> &t);
	void prepend(const T &t);
	void insert(int i, const T &t);
	void replace(int i, const T &t);
	void removeAt(int i);
	int removeAll(const T &t);
	bool removeOne(const T &t);
	void move(int from, int to);
	QList<T>::iterator insert(QList<T>::iterator before, const T &t);
	QList<T>::iterator erase(QList<T>::iterator pos);
	QList<T>::iterator erase(QList<T>::iterator first, QList<T>::iterator last);
};

template <typename T>
void ObservableList<T>::append(const T& t)
{
	QList<T>::append(t);
	emit itemAdeed(QList<T>::size() - 1);
}

template <typename T>
void ObservableList<T>::append(const QList<T>& t)
{
	QList<T>::append(t);
	emit itemAdeed(QList<T>::size() - t.size());
}

template <typename T>
void ObservableList<T>::prepend(const T& t)
{
	QList<T>::prepend(t);
	emit itemAdeed(0);
}

template <typename T>
void ObservableList<T>::insert(int i, const T& t)
{
	QList<T>::insert(i, t);
	emit itemAdeed(i);
}

template <typename T>
void ObservableList<T>::replace(int i, const T& t)
{
	QList<T>::replace(i, t);
	emit itemChanged(i);
}

template <typename T>
void ObservableList<T>::removeAt(int i)
{
	QList<T>::removeAt(i);
	emit itemRemoved(i);
}

template <typename T>
int ObservableList<T>::removeAll(const T& t)
{
	int foundIndex = indexOf(t);
	while (foundIndex >= 0)
	{
		removeAt(foundIndex);
		foundIndex = indexOf(t);
	}
}

template <typename T>
bool ObservableList<T>::removeOne(const T& t)
{
	QList<T>::removeOne(t);
}

template <typename T>
typename QList<T>::iterator ObservableList<T>::insert(typename QList<T>::iterator before, const T& t)
{
	QList<T>::insert(before, t);
	emit itemAdeed(before - QList<T>::begin());
}

template <typename T>
typename QList<T>::iterator ObservableList<T>::erase(typename QList<T>::iterator pos)
{
	QList<T>::erase(pos);
	emit itemRemoved(pos - QList<T>::begin());
}

template <typename T>
typename QList<T>::iterator ObservableList<T>::erase(typename QList<T>::iterator first, typename QList<T>::iterator last)
{
	QList<T>::erase(first, last);
	emit itemsRemoved(first - QList<T>::begin(), last - first);
}

template <typename T>
void ObservableList<T>::move(int from, int to)
{
	QList<T>::move(i);
	emit itemMoved(from, to);
}
#endif
