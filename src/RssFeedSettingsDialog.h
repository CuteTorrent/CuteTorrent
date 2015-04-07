
#include <QtCore>
#include <QtGui>
#include "CustomWindow.h"
#include "ui_RssFeedSettingsDialog.h"
class RssFeed;
class RssFeedSettingsDialog : public BaseWindow<QDialog>, private Ui::RssSettings
{
	Q_OBJECT
protected:
	void changeEvent(QEvent* event);
	virtual QPushButton* getCloseBtn() override;
	virtual QWidget* getTitleBar() override;
	virtual QWidget* centralWidget() override;
	virtual QLabel* getTitleLabel() override;
	virtual QLabel* getTitleIcon() override;
public:
	RssFeedSettingsDialog(QWidget* parent = 0, int flags = 0);
	~RssFeedSettingsDialog();
	void SetFeed(RssFeed*);
	void FillInData();
private:
	RssFeed* m_pFeed;

};