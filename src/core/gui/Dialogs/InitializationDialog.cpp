#include "InitializationDialog.h"

InitializationDialog::InitializationDialog(QWidget* parent)
	: BaseWindow<QDialog>(MinimizeModeOff, NoResize, parent)
	, Ui::InitializationDialog()
	
{
	m_pInitThread = new QThread;
	m_pWorker = new InitializationWorker();
	setupUi(this);
	setupCustomWindow();
	setupWindowIcons();
	pbClose->setEnabled(false);
	m_pWorker->moveToThread(m_pInitThread);
	connect(m_pInitThread, SIGNAL(started()), m_pWorker, SLOT(PeformInit()));
	connect(m_pWorker, SIGNAL(progress(int,QString)), SLOT(onProgressChanged(int,QString)), Qt::QueuedConnection);
	connect(m_pWorker, SIGNAL(finished()), m_pInitThread, SLOT(quit()));
	connect(m_pWorker, SIGNAL(finished()), m_pWorker, SLOT(deleteLater()));
	connect(m_pInitThread, SIGNAL(finished()), m_pInitThread, SLOT(deleteLater()));
	connect(m_pInitThread, SIGNAL(finished()), SLOT(close()));
}

int InitializationDialog::exec()
{
	if (!m_pInitThread->isRunning())
	{
		m_pInitThread->start(QThread::NormalPriority);
	}
	return QDialog::exec();
}

void InitializationDialog::onProgressChanged(int val, QString item)
{
	progressBar->setValue(val);
	stateLabel->setText(tr("TORRENT %1 RESUMED").arg(item));
}

QPushButton* InitializationDialog::getCloseBtn()
{
	return pbClose;
}

QWidget* InitializationDialog::getTitleBar()
{
	return titleBar;
}

QWidget* InitializationDialog::centralWidget()
{
	return m_centralWidget;
}

QLabel* InitializationDialog::getTitleLabel()
{
	return LTitle;
}

QLabel* InitializationDialog::getTitleIcon()
{
	return tbMenu;
}