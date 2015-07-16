#ifndef APPLYBACKUPPAGE_H
#define APPLYBACKUPPAGE_H


#include <QtGui>
#include "quazipfile.h"
class ApplyBackupPage : public QWizardPage
{
	Q_OBJECT
public:
	explicit ApplyBackupPage(QWidget* parent = 0);
	int	nextId() const;
private:
	QLabel* messageLabel;
	QGridLayout* gridLayout;
	QLineEdit* backupPathLineEdit;
	QPushButton* browsePushButton;
	QCheckBox* drivesCheckBox;
	QComboBox* drivesComboBox;
	QGroupBox* changePathGroupBox;
	QGridLayout* gridLayout_2;
	QTableWidget* tableWidget;
	QStringList GetLongestCommonSubstr(QStringList strings);
	void ApplyBackup() const;
public slots:
	void browseButtonClicked();
};

#endif // APPLYBACKUPPAGE_H
