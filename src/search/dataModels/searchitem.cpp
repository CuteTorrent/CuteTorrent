#include "searchitem.h"


QString SearchItem::getName()
{
	return name;
}

QString SearchItem::getPattern()
{
	return pattern;
}

void SearchItem::setName(QString newName)
{
	name = newName;
}

void SearchItem::setPattern(QString newPattern)
{
	pattern = newPattern;
}
