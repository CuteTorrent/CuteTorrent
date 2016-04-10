#ifndef _ObservebaleCollectionBase_INCLUDED_
#define _ObservebaleCollectionBase_INCLUDED_
#include <QObject>

class ObservebaleCollectionBase : public QObject
{
	Q_OBJECT
public:
	enum ChangeAction
	{
		Add,
		Remove,
		Replace,
		Move,
		Reset
	};

	struct CollectionChangedInfo
	{
		ChangeAction Action;
		int newStartIndex, newCount;
		int oldStartIndex, oldCount;
	};

	signals:
	void CollectionChanged(CollectionChangedInfo info);
};
#endif

