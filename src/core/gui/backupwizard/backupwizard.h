#ifndef BACKUPWIZARD_H
#define BACKUPWIZARD_H

#include <QWizard>
#include <QtGui>
class BackupWizard : public QWizard
{
	Q_OBJECT
public:
	explicit BackupWizard(QWidget* parent = 0);
	enum { Page_Intro , Page_CreateBakup , Page_ApplyBakup , Page_Finish };
signals:

public slots:

private:
	QRadioButton* careateRadioButton;
	QRadioButton* applyRadioButton;
};

#endif // BACKUPWIZARD_H
