#include "RssFeedSettingsDialog.h"
#include "RssFeed.h"
RssFeedSettingsDialog::RssFeedSettingsDialog(QWidget* parent /*= 0*/, int flags /*= 0*/) : BaseWindow(BaseWindow::OnlyCloseButton, BaseWindow::NoResize, parent), m_pFeed(NULL)
{
	setupUi(this);
	setupCustomWindow();
	setupWindowIcons();
}

QWidget* RssFeedSettingsDialog::centralWidget()
{
	return m_centralWidget;
}

QPushButton* RssFeedSettingsDialog::getCloseBtn()
{
	return pbClose;
}

QWidget* RssFeedSettingsDialog::getTitleBar()
{
	return titleBar;
}

QLabel* RssFeedSettingsDialog::getTitleIcon()
{
	return tbMenu;
}

void RssFeedSettingsDialog::accept()
{
	if (m_pFeed == nullptr)
	{
		return;
	}
	m_pFeed->setTll(m_pRefreshRateSpinBox->value());
}

QLabel* RssFeedSettingsDialog::getTitleLabel()
{
	return LTitle;
}

void RssFeedSettingsDialog::changeEvent(QEvent* event)
{
	if (event->type() == QEvent::LanguageChange)
	{
		retranslateUi(this);
	}
}

RssFeedSettingsDialog::~RssFeedSettingsDialog()
{
}

void RssFeedSettingsDialog::SetFeed(RssFeed* pFeed)
{
	m_pFeed = pFeed;
	FillInData();
}

void RssFeedSettingsDialog::FillInData()
{
	if (m_pFeed == nullptr)
	{
		return;
	}

	m_pRssUrlEdit->setText(m_pFeed->url().toString());
	m_pRefreshRateSpinBox->setValue(m_pFeed->ttl());
}



