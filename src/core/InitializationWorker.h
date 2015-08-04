#ifndef InitializationWorker_INCLUDED
#define InitializationWorker_INCLUDED
#include <QObject>

class InitializationWorker : public QObject
{
	Q_OBJECT
signals:
	void progress(int p, QString item);
	void finished();
public slots:
	void PeformInit();

};

#endif