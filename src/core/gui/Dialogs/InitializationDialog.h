#ifndef InitializationDialog_INCLUDED
#define InitializationDialog_INCLUDED
#include <QDialog>
#include "CustomWindow.h"
#include "ui_InitializtionDialog.h"
#include <InitializationWorker.h>

class InitializationDialog : public BaseWindow<QDialog>, Ui::InitializationDialog
{
	Q_OBJECT
protected:
	QPushButton* getCloseBtn() override;
	QWidget* getTitleBar() override;
	QWidget* centralWidget() override;
	QLabel* getTitleLabel() override;
	QLabel* getTitleIcon() override;
public:
	InitializationDialog(QWidget* parent = 0);
public slots:	
	int exec();
	void onProgressChanged(int, QString);
private:
	QThread* m_pInitThread;
	InitializationWorker* m_pWorker;
};

#endif