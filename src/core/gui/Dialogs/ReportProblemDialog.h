#ifndef REPORT_PROBLEM_DIALOG
#define REPORT_PROBLEM_DIALOG

#include <QDialog>
#include "ui_ReportProblemDialog.h"
#include "CustomWindow.h"
#include <QNetworkAccessManager>

class ReportProblemDialog : public BaseWindow<QDialog>, Ui::ReportProblemDialog
{
	Q_OBJECT
protected:
	QPushButton* getCloseBtn() override;
	QWidget* getTitleBar() override;
	QWidget* centralWidget() override;
	QLabel* getTitleLabel() override;
	QLabel* getTitleIcon() override;
public:
	ReportProblemDialog(QWidget* parent = 0);
public slots:
	void SendReport();
	void OnFinished(QNetworkReply*);
private:
	bool checkFields();
	QNetworkAccessManager* m_pNetManager;
};
#endif

