#include "backupwizard.h"
#include "intropage.h"
#include "createbackuppage.h"
#include "applybackuppage.h"
#include "finishpage.h"
BackupWizard::BackupWizard(QWidget* parent) :
	QWizard(parent)
{
	setPage(Page_CreateBakup, new CreateBackupPage);
	setPage(Page_ApplyBakup, new ApplyBackupPage);
	setPage(Page_Intro, new IntroPage);
	setPage(Page_Finish, new FinishPage);
	setStartId(Page_Intro);
	setWizardStyle(ClassicStyle);
	setOption(NoBackButtonOnLastPage);
	setOption(NoCancelButton);
}

