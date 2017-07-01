#include <QtTest/QtTest>
#include <QtWidgets/QtWidgets>

#include "modeltest.h"
#include "dynamictreemodel.h"


class tst_ModelTest : public QObject
{
	Q_OBJECT

public:
	tst_ModelTest() {}
	virtual ~tst_ModelTest() {}

public slots :
	void initTestCase();
	void cleanupTestCase();
	void init();
	void cleanup();

private slots:
	void stringListModel();
	void treeWidgetModel();
	void standardItemModel();
	void testInsertThroughProxy();
	void moveSourceItems();
	void testResetThroughProxy();
};