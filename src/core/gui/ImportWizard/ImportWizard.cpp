#include "ImportWizard.h"
#include "ImportIntroPage.h"
#include "ChoseDataPage.h"
#include "ImportPage.h"
#include "ImportFinishPage.h"
#include <QDesktopServices>
#include <QUrl>
ImportWizard::ImportWizard(QWidget* parent)
{
	setPage(Intro, new ImportIntroPage);
	setPage(ChoseData, new ChoseDataPage);
	setPage(Import, new ImportPage);
	setPage(Finish, new ImportFinishPage);
	setStartId(Intro);
	setWizardStyle(ModernStyle);
	QList<WizardButton> button_layout;
	button_layout << HelpButton << Stretch <<
		NextButton << FinishButton;
	setButtonLayout(button_layout);
	setOption(NoBackButtonOnLastPage);
	setOption(NoCancelButton);
	QObject* helpButton = reinterpret_cast<QObject*>(button(HelpButton));
	connect(helpButton, SIGNAL(clicked()), this, SLOT(NavigateHelp()));
}

void ImportWizard::NavigateHelp()
{
	QDesktopServices::openUrl(QUrl("http://cutetorrent.info/importing/"));
}
