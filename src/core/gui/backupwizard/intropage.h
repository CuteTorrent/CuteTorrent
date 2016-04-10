#ifndef INTROPAGE_H
#define INTROPAGE_H

#include <QWizardPage>
#include <QtGui>

class IntroPage : public QWizardPage
{
	Q_OBJECT
public:
	IntroPage(QWidget* parent = 0);
	// bool validatePage();
	int nextId() const;

private:
	QRadioButton* careateRadioButton;
	QRadioButton* applyRadioButton;

	// QWizardPage interface
public:
	bool validatePage();
};

#endif // INTROPAGE_H


