
#ifndef RaitingDialog_INCLUDED
#define RaitingDialog_INCLUDED
#include "ui_RaitingDialog.h"
#include <QDialog>
#include "CustomWindow.h"
class QNetworkAccessManager;
class QNetworkReply;
class RatingWidget;

class RaitingDialog : public BaseWindow<QDialog>, Ui::RaitingDialog
{
	Q_OBJECT

	QNetworkAccessManager* m_pNetworkAccessManager;
	QString getMacAddress();
	QList<RatingWidget*> m_raitingWidgets;
	void setupRaitingControls();
protected:
	QPushButton* getCloseBtn() override;
	QWidget* getTitleBar() override;
	QWidget* centralWidget() override;
	QLabel* getTitleLabel() override;
	QLabel* getTitleIcon() override;
public:
	enum RATINGS
	{
		SPEED,
		DESIGN,
		USABILITY,
		POSSIBILITIES,
		RATING_COUNT
	};
	
	RaitingDialog(QWidget* parent);
public slots:
	void sendRaiting();
	void OnRaitingSent(QNetworkReply*);

};

#endif
