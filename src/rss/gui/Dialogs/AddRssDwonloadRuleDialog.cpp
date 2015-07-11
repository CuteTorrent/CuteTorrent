#include "AddRssDwonloadRuleDialog.h"
#include "RssManager.h"
#include "RssFeed.h"
#include "FaviconDownloader.h"
#include "QRssDisplayModel.h"
#include "QRssFilterModel.h"
#include "QRssItemDelegate.h"
#include <QSizePolicy>
#include <QApplicationSettings.h>

AddRssDwonloadRuleDialog::AddRssDwonloadRuleDialog(QWidget* parent, DiaologMode mode) : BaseWindow(OnlyCloseButton, AllowResize, parent), m_currentRule(new RssDownloadRule())
{
	m_inited = false;
	setupUi(this);
	setupStatusBar();
	setupTitle(mode);
	setupCustomWindow();
	setupWindowIcons();
	setupTreeView();
	FillRssFeedsList();
	FillComboboxes();
	m_inited = true;
	onChange();
}

QWidget* AddRssDwonloadRuleDialog::centralWidget()
{
	return m_centralWidget;
}

QPushButton* AddRssDwonloadRuleDialog::getCloseBtn()
{
	return pbClose;
}

QWidget* AddRssDwonloadRuleDialog::getTitleBar()
{
	return titleBar;
}

QLabel* AddRssDwonloadRuleDialog::getTitleIcon()
{
	return tbMenu;
}

QLabel* AddRssDwonloadRuleDialog::getTitleLabel()
{
	return LTitle;
}

void AddRssDwonloadRuleDialog::FillRssFeedsList()
{
	RssManagerPtr pManager = RssManager::getInstance();
	FaviconDownloaderPtr pIconDownloader = FaviconDownloader::getInstance();
	QList<RssFeed*> pFeeds = pManager->feeds();

	foreach(RssFeed* pFeed, pFeeds)
	{
		QListWidgetItem* pItem = new QListWidgetItem(pFeed->displayName(true));
		pItem->setCheckState(Qt::Unchecked);
		pItem->setData(Qt::UserRole, qVariantFromValue(pFeed->uid()));
		pItem->setIcon(pIconDownloader->getFavicon(pFeed->url().toString()));
		m_pFeedsListWidget->addItem(pItem);
	}

	connect(m_pFeedsListWidget, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(onFeedListItemChanged(QListWidgetItem*)));
}

void AddRssDwonloadRuleDialog::onFeedListItemChanged(QListWidgetItem* pItem)
{
	QUuid feedUid = pItem->data(Qt::UserRole).value<QUuid>();

	if (pItem->checkState() == Qt::Checked)
	{
		m_currentRule->addFeed(feedUid);
	}
	else
	{
		m_currentRule->removeFeed(feedUid);
	}

	onChange(true);
}

void AddRssDwonloadRuleDialog::onChange(bool directCall)
{
	if (m_inited)
	{
		if (!directCall)
		{
			QString ruleName = m_pRuleNameEdit->text();
			RssDownloadRule::DownloadRuleType ruleType = RssDownloadRule::EMPTY;
			QRegExp::PatternSyntax searchType = QRegExp::RegExp2;
			QString searchString = m_pSearchStringEdit->text();

			if (m_pRuleTypeCombobx->currentIndex() > -1)
			{
				ruleType = static_cast<RssDownloadRule::DownloadRuleType>(m_pRuleTypeCombobx->itemData(m_pRuleTypeCombobx->currentIndex(), Qt::UserRole).toInt());
			}

			if (m_pSearchTypeComboBox->currentIndex() > -1)
			{
				searchType = static_cast<QRegExp::PatternSyntax>(m_pSearchTypeComboBox->itemData(m_pSearchTypeComboBox->currentIndex(), Qt::UserRole).toInt());
			}

			m_currentRule->setUseStaticSavePath(m_pUseSavePathGroupBox->isChecked());
			m_currentRule->setStaticSavePath(m_pSavePathEdit->text());
			m_currentRule->setUseGroupFilters(m_pUseGroupsCheckBox->isChecked());
			m_currentRule->setName(ruleName);
			m_currentRule->setPattern(searchString);
			m_currentRule->setRuleType(ruleType);

			if (searchType != QRegExp::RegExp2)
			{
				m_currentRule->setPatternType(searchType);
			}
		}

		bool ok;
		QString result = m_currentRule->validate(ok);
		QPushButton* okButton = buttonBox->button(QDialogButtonBox::Ok);

		if (okButton)
		{
			okButton->setEnabled(ok);
		}

		m_pStatusBar->showMessage(result);

		if (ok)
		{
			m_pRssFilterModel->setRuleFilter(m_currentRule);
		}
	}
}

void AddRssDwonloadRuleDialog::FillComboboxes()
{
	m_pRuleTypeCombobx->addItem(tr("DOWNLOAD_RULE"), RssDownloadRule::DOWNLOAD_RULE);
	m_pRuleTypeCombobx->addItem(tr("SELECT_FILE_RULE"), RssDownloadRule::SELECT_FILE_RULE);
	m_pRuleTypeCombobx->setCurrentIndex(-1);
	m_pSearchTypeComboBox->addItem(tr("SIMPLE_STRING"), QRegExp::FixedString);
	m_pSearchTypeComboBox->addItem(tr("WILDCARD"), QRegExp::Wildcard);
	m_pSearchTypeComboBox->addItem(tr("REGEXP"), QRegExp::RegExp);
	m_pSearchTypeComboBox->setCurrentIndex(-1);
}

void AddRssDwonloadRuleDialog::setupTitle(DiaologMode mode)
{
	switch (mode)
	{
		case EDIT:
			setTitle(tr("EDIT_RSS_DWONLOAD_RULE_DIALOG"));
			break;

		case CREATE:
			setTitle(tr("ADD_RSS_DWONLOAD_RULE_DIALOG"));
			break;

		default:
			break;
	}
}

void AddRssDwonloadRuleDialog::setupStatusBar()
{
	m_pStatusBar = new QStatusBar(this);
	m_pStatusBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	m_pStatusBar->setMinimumHeight(20);
	m_pStatusBar->setMaximumHeight(20);
	m_windowLayout->addWidget(m_pStatusBar);
}

void AddRssDwonloadRuleDialog::setupTreeView()
{
	QRssDisplayModel* pModel = new QRssDisplayModel(m_pFilteredFeedsView, this, false);
	m_pRssFilterModel = new QRssFilterModel(this);
	m_pRssFilterModel->setSourceModel(pModel);
	m_pFilteredFeedsView->setModel(m_pRssFilterModel);
	m_pFilteredFeedsView->setItemDelegate(new QRssItemDelegate(this));
}

void AddRssDwonloadRuleDialog::FillDataFromDownloadRule()
{
	m_inited = false;
	m_pUseSavePathGroupBox->setChecked(m_currentRule->UseStaticSavePath());
	m_pSavePathEdit->setText(m_currentRule->StaticSavePath());
	bool useGroupFilters = m_currentRule->UseGroupFilters();
	m_pUseGroupsCheckBox->setChecked(useGroupFilters);
	m_pRuleNameEdit->setText(m_currentRule->Name());
	m_pSearchStringEdit->setText(m_currentRule->Pattern());

	for (int i = 0; i < m_pRuleTypeCombobx->count(); i++)
	{
		if (static_cast<RssDownloadRule::DownloadRuleType>(m_pRuleTypeCombobx->itemData(i).toInt()) == m_currentRule->RuleType())
		{
			m_pRuleTypeCombobx->setCurrentIndex(i);
			break;
		}
	}

	for (int i = 0; i < m_pSearchTypeComboBox->count(); i++)
	{
		if (static_cast<QRegExp::PatternSyntax>(m_pSearchTypeComboBox->itemData(i).toInt()) == m_currentRule->PatternType())
		{
			m_pSearchTypeComboBox->setCurrentIndex(i);
			break;
		}
	}

	for (int i = 0; i < m_pFeedsListWidget->count(); i++)
	{
		QListWidgetItem* item = m_pFeedsListWidget->item(i);

		if (m_currentRule->MatchFeed(item->data(Qt::UserRole).value<QUuid>()))
		{
			item->setCheckState(Qt::Checked);
		}
	}

	m_pRssFilterModel->setRuleFilter(m_currentRule);
	m_inited = true;
}

RssDownloadRule* AddRssDwonloadRuleDialog::getFinalRule()
{
	return m_currentRule;
}

void AddRssDwonloadRuleDialog::setDownloadRule(RssDownloadRule* rule)
{
	delete m_currentRule;
	m_currentRule = new RssDownloadRule(*rule);
	FillDataFromDownloadRule();
	onChange();
}

void AddRssDwonloadRuleDialog::onApplyRule()
{
	done(Accepted);
}

void AddRssDwonloadRuleDialog::onCancel()
{
	done(Rejected);
}

void AddRssDwonloadRuleDialog::onUpdateRuleTypeHint()
{
	RssDownloadRule::DownloadRuleType ruleType = RssDownloadRule::EMPTY;

	if (m_pRuleTypeCombobx->currentIndex() > -1)
	{
		ruleType = static_cast<RssDownloadRule::DownloadRuleType>(m_pRuleTypeCombobx->itemData(m_pRuleTypeCombobx->currentIndex(), Qt::UserRole).toInt());
	}

	switch (ruleType)
	{
		case RssDownloadRule::EMPTY:
			break;

		case RssDownloadRule::SELECT_FILE_RULE:
		{
			m_pRuleTypeHintLabel->setText(tr("SELECT_FILE_RULE_TYPE_HINT"));
			break;
		}

		case RssDownloadRule::DOWNLOAD_RULE:
		{
			m_pRuleTypeHintLabel->setText(tr("DOWNLOAD_RULE_TYPE_HINT"));
			break;
		}

		default:
			break;
	}
}

void AddRssDwonloadRuleDialog::onBrowseStaticPath()
{
	QString lastDir = QApplicationSettings::getInstance()->valueString("System", "LastSaveTorrentDir", QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation));
	QString dir = QFileDialog::getExistingDirectory(this, tr("DIALOG_OPEN_DIR"),
	              lastDir,
	              QFileDialog::ShowDirsOnly
	              | QFileDialog::DontResolveSymlinks);
}
