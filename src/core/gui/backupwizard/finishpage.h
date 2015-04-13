#ifndef FINISHPAGE_H
#define FINISHPAGE_H

#include <QtGui>

class FinishPage : public QWizardPage
{
	Q_OBJECT
public:
	explicit FinishPage(QWidget* parent = 0);
	int	nextId() const;
signals:

public slots:
private:
	QLabel* messageLabel;
	QVBoxLayout* layout;
};

#endif // FINISHPAGE_H
