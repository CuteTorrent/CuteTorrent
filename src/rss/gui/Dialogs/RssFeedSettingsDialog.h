#include <QtCore>
#include <QtGui>
#include "CustomWindow.h"
#include "ui_RssFeedSettingsDialog.h"
class RssFeed;

class RssFeedSettingsDialog : public BaseWindow<QDialog>, private Ui::RssSettings
{
	Q_OBJECT
protected:
	void changeEvent(QEvent* event) override;
	QPushButton* getCloseBtn() override;
	QWidget* getTitleBar() override;
	QWidget* centralWidget() override;
	QLabel* getTitleLabel() override;
	QLabel* getTitleIcon() override;
	void accept() override;
private slots:
	void onAddRow();
	void onRemoveRow();
public:
	RssFeedSettingsDialog(QWidget* parent = 0, int flags = 0);
	~RssFeedSettingsDialog();
	void SetFeed(RssFeed*);
	void FillInData();
private:
	RssFeed* m_pFeed;
};

