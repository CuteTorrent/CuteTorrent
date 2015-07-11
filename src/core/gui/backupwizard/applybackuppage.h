#ifndef APPLYBACKUPPAGE_H
#define APPLYBACKUPPAGE_H


#include <QtGui>
//#include <private/qzipreader_p.h>
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
	QString pathResumeData;
	//QZipReader* zipReader;
	QString GetLongestCommonSubstr(QString a, QString  b);
	//bool parseData(QZipReader* reader);
	QStringList GetLongestCommonSubstr(QStringList strings);
	void ApplyBackup() const;
public slots:
	void browseButtonClicked();
};

#endif // APPLYBACKUPPAGE_H
