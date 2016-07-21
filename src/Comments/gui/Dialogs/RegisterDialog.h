#ifndef RegisterDialog_INCLUDED
#define RegisterDialog_INCLUDED
#include <QDialog>
#include <gui/Dialogs/CustomWindow.h>
#include "ui_RegisterDialog.h"

class RegisterDialog : public BaseWindow<QDialog>, Ui::RegisterDialog
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
	RegisterDialog(QWidget* parent = NULL);
private slots:
	void OnAccepted();
};

#endif

