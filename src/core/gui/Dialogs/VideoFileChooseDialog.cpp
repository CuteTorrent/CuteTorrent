#include "VideoFileChooseDialog.h"
#include "StyleEngene.h"

QPushButton* VideoFileChooseDialog::getCloseBtn()
{
	return pbClose;
}

QWidget* VideoFileChooseDialog::getTitleBar()
{
	return titleBar;
}

QWidget* VideoFileChooseDialog::centralWidget()
{
	return m_centralWidget;
}

QLabel* VideoFileChooseDialog::getTitleLabel()
{
	return LTitle;
}

QLabel* VideoFileChooseDialog::getTitleIcon()
{
	return tbMenu;
}

void VideoFileChooseDialog::setupConnections() const
{
	connect(buttonBox, SIGNAL(accepted()), SLOT(accepted()));
	connect(buttonBox, SIGNAL(rejected()), SLOT(rejected()));
}

void VideoFileChooseDialog::accepted()
{
	QModelIndex index = listView->currentIndex();
	m_choosenPath = m_videoItemsModel->data(index, Qt::UserRole).toString();
	QDialog::accepted();
	close();
}

void VideoFileChooseDialog::rejected()
{
	QDialog::rejected();
	close();
}

VideoFileChooseDialog::VideoFileChooseDialog(QStringList files, QWidget* parent)
	: BaseWindow<QDialog>(FullTitle, NoResize, parent)

{
	setupUi(this);
	setupCustomWindow();
	setupWindowIcons();
	setupConnections();
	m_videoItemsModel = new QStandardItemModel(files.count(), 1, this);
	for (int i = 0; i < files.count(); i++)
	{
		QModelIndex index = m_videoItemsModel->index(i, 0);
		QFileInfo fileInfo(files[i]);
		m_videoItemsModel->setData(index, StyleEngene::fileIcons[StyleEngene::VIDEO], Qt::DecorationRole);
		m_videoItemsModel->setData(index, fileInfo.fileName(), Qt::DisplayRole);
		m_videoItemsModel->setData(index, fileInfo.absoluteFilePath(), Qt::UserRole);
	}
	listView->setModel(m_videoItemsModel);
}

QString VideoFileChooseDialog::choosenPath() const
{
	return m_choosenPath;
}

