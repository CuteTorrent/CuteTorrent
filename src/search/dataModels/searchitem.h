#ifndef SEARCHITEM_H
#define SEARCHITEM_H

#include <QObject>

class SearchItem
{
public:
	QString getName();
	QString getPattern();
	void setName(QString newName);
	void setPattern(QString newPattern);
private:
	QString name, pattern;
};

#endif // SEARCHITEM_H


