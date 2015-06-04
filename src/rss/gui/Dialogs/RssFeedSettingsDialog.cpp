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
	QHash<QString, QString> cookies;

	for (int i = 0; i < m_pCoociesTabWidget->rowCount(); i++)
	{
		QTableWidgetItem* keyItem = m_pCoociesTabWidget->item(i, 0);
		QString key = keyItem->text();
		QTableWidgetItem* valueItem = m_pCoociesTabWidget->item(i, 1);
		QString value = valueItem->text();
		cookies.insert(key, value);
	}

	m_pFeed->setCoookies(cookies);
	QDialog::accept();
}

void RssFeedSettingsDialog::onAddRow()
{
	m_pCoociesTabWidget->insertRow(m_pCoociesTabWidget->rowCount());
}

void RssFeedSettingsDialog::onRemoveRow()
{
	QList<QTableWidgetSelectionRange> selectedRanges = m_pCoociesTabWidget->selectedRanges();

	for (int i = 0; i < selectedRanges.length(); i++)
	{
		QTableWidgetSelectionRange range = selectedRanges.at(i);

		for (int j = range.bottomRow(); j >= range.topRow(); j--)
		{
			m_pCoociesTabWidget->removeRow(j);
		}
	}
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
	QList<QNetworkCookie> cookies = m_pFeed->coookies();
	m_pCoociesTabWidget->setRowCount(cookies.size());
	for (int i = 0; i < cookies.size(); i++)
	{
		QNetworkCookie cookie = cookies[i];
		m_pCoociesTabWidget->setItem(i, 0, new QTableWidgetItem(QString::fromUtf8(cookie.name())));
		m_pCoociesTabWidget->setItem(i, 1, new QTableWidgetItem(QString::fromUtf8(cookie.value())));
	}
}



