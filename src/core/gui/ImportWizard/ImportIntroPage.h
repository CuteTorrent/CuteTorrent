#ifndef IMPORT_INTROPAGE_H
#define IMPORT_INTROPAGE_H
#include <QWizard>
class BaseImporter;
class QSignalMapper;
class ImportIntroPage : public QWizardPage
{

private:
	Q_OBJECT

	Q_PROPERTY(BaseImporter* CurrentImporter READ getCurrentImporter)
	BaseImporter* m_pCurrentImporter;
	QSignalMapper* m_ptorrentImportersMapper;
public:
	ImportIntroPage(QWidget* parent = 0);
	void initializePage() override;
	void cleanupPage() override;
	int nextId() const override;
	BaseImporter* getCurrentImporter() const;
	bool isComplete() const override;
private slots:
	void setImporter(QString name);

};


#endif
