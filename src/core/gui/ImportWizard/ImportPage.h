#ifndef IMPORT_PAGE_INCLUDED
#define IMPORT_PAGE_INCLUDED
#include <QWizard>
#include "ImportWizard.h"
class QSpacerItem;
class QLineEdit;
class QGridLayout;
class QLabel;
class QProgressBar;

class ImportPage : public QWizardPage
{
	Q_OBJECT
	QGridLayout* gridLayout;
	QLabel* label;
	QProgressBar* progressBar;
	QSpacerItem* verticalSpacer;
	QSpacerItem* verticalSpacer_2;
	Q_PROPERTY(QString ImportErrors READ getImportErrors)
	Q_PROPERTY(int ImportResult READ getImportResult)
	QString m_importErrors;
	ImportWizard::ImportResult m_importResult;
	QString getImportErrors() const;
	int getImportResult() const;
public:
	ImportPage(QWidget* parent = 0);
	void initializePage() override;
	void cleanupPage() override;
	int nextId() const override;
	bool isComplete() const override;
private slots:
	void progressCallback(QString text, int progress);
	void collectErrors(QString name, QString error);
};

#endif

