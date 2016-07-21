#ifndef LoginDialog_INCLUDED
#define LoginDialog_INCLUDED
#include <QDialog>
#include <gui/Dialogs/CustomWindow.h>
#include "ui_LoginDialog.h"

class LoginDialog : public BaseWindow<QDialog>, Ui::LoginDialog
{
protected:
	QPushButton* getCloseBtn() override;
	QWidget* getTitleBar() override;
	QWidget* centralWidget() override;
	QLabel* getTitleLabel() override;
	QLabel* getTitleIcon() override;
private:
	Q_OBJECT
public:
	LoginDialog(QWidget* parent = NULL);
public slots:
	void OnAccepted();
};


#endif

