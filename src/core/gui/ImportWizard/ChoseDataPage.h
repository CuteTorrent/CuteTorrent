#ifndef CHOSE_DATAPAGE_INCLUDED
#define CHOSE_DATAPAGE_INCLUDED
#include <QWizard>
class QSpacerItem;
class QLineEdit;
class QGridLayout;
class QLabel;

class ChoseDataPage : public QWizardPage
{
	Q_OBJECT
	QGridLayout* gridLayout;
	QLabel* label;
	QPushButton* pushButton;
	QLineEdit* lineEdit;
	QSpacerItem* verticalSpacer;
	Q_PROPERTY(QString CustomDataPath READ getCustomDataPath)
	QString m_customDataPath;
public:
	explicit ChoseDataPage(QWidget* parent = 0);
	bool isComplete() const override;
	int nextId() const override;
	QString getCustomDataPath() const;
private slots:
	void ChooseFile();
};

#endif

