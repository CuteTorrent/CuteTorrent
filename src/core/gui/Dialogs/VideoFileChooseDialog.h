#ifndef VIDEOFILE_CHOOSE_DIALOG_INCLUDED
#define VIDEOFILE_CHOOSE_DIALOG_INCLUDED

#include "ui_VideoFileChoose.h"
#include <CustomWindow.h>
#include <QStandardItemModel>

class VideoFileChooseDialog : public BaseWindow<QDialog>, public Ui::VideoFileChooseDialog
{
	Q_OBJECT
public:

	VideoFileChooseDialog(QStringList files, QWidget* parent = 0);
	QString choosenPath() const;
protected:
	QPushButton* getCloseBtn() override;
	QWidget* getTitleBar() override;
	QWidget* centralWidget() override;
	QLabel* getTitleLabel() override;
	QLabel* getTitleIcon() override;
private:
	QStandardItemModel* m_videoItemsModel;
	void setupConnections() const;
	QString m_choosenPath;
private slots:
	void accepted();
	void rejected();
};

#endif

