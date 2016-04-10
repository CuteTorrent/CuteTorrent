#ifndef IMPORTWIZARD_H
#define IMPORTWIZARD_H
#include <QWizard>

class ImportWizard :
	public QWizard
{
	Q_OBJECT
public:
	enum ImportResult
	{
		None,
		Success,
		Canceled,
		Failed
	};

	enum Pages
	{
		Intro,
		ChoseData,
		Import,
		Finish
	};

	ImportWizard(QWidget* parent = 0);
private slots:
	void NavigateHelp();
};

#endif

