
#ifndef FINISH_PAGE_INCLUDED
#define FINISH_PAGE_INCLUDED
#include <QWizard>
class QGridLayout;
class QLabel;
class QPlainTextEdit;
class ImportFinishPage : public QWizardPage
{
	Q_OBJECT
public:
	void initializePage() override;
private:
	QGridLayout *gridLayout;
	QLabel *resultLabel;
	QPlainTextEdit *errorDisplay;

public:
	explicit ImportFinishPage(QWidget* parent = 0);
};

#endif
