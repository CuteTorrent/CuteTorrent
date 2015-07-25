#include "ObservebaleCollectionBase.h"

template<typename T>
class ObservableList : public ObservebaleCollectionBase, public QList<T>
{
	
public:
	void append(const T &t);
	void append(const QList<T> &t);
	void prepend(const T &t);
	void insert(int i, const T &t);
	void replace(int i, const T &t);
	int removeAll(const T &t);
	void removeAt(int i);
	bool removeOne(const T &t);
	void move(int from, int to);
	void swap(int i, int j);
};

template <typename T>
int ObservableList<T>::removeAll(const T& t)
{
	int foundIndex = indexOf(t);
	int removed = 0;
	while (foundIndex > -1)
	{
		removeAt(foundIndex);
		removed++;
		foundIndex = indexOf(t);
	}

	return removed;
}

template <typename T>
void ObservableList<T>::append(const T& t)
{
	int index = QList<T>::size();
	QList<T>::append(t);
	emit CollectionChanged({ Add , index, 1, -1,-1});
}

template <typename T>
void ObservableList<T>::append(const QList<T>& t)
{
	int index = QList<T>::size();
	QList<T>::append(t);
	emit CollectionChanged({ Add, index, t.size(), -1, -1 });
}

template <typename T>
void ObservableList<T>::prepend(const T& t)
{
	
	QList<T>::prepend(t);
	emit CollectionChanged({ Add, 0, 1, -1, -1 });
}

template <typename T>
void ObservableList<T>::insert(int i, const T& t)
{
	QList<T>::insert(i, t);
	emit CollectionChanged({ Add, i, 1, -1, -1 });
}

template <typename T>
void ObservableList<T>::replace(int i, const T& t)
{
	QList<T>::replace(i, t);
	emit CollectionChanged({ Replace, i, 1, -1, -1 });
}

template <typename T>
void ObservableList<T>::removeAt(int i)
{
	QList<T>::removeAt(i);
	emit CollectionChanged({ Remove, -1, -1, i, 1 });
}


template <typename T>
bool ObservableList<T>::removeOne(const T& t)
{
	return QList<T>::removeOne(t);
}

template <typename T>
void ObservableList<T>::move(int from, int to)
{
	QList<T>::move(from, to);
	emit CollectionChanged({ Move, to, 1, from, 1 });
}

template <typename T>
void ObservableList<T>::swap(int i, int j)
{
	QList<T>::swap(i,j);
	emit CollectionChanged({ Move, i, 1, j, 1 });
}