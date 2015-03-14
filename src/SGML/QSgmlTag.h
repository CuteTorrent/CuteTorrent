/*------------------------------------------------------------------------------------------
QSgmlTag helps to handle a SGML/HTML-Tag
Copyright (C) 2010  Andreas Lehmann

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

QSgmlTag.h  Copyright (C) 2010  Andreas Lehmann
This program comes with ABSOLUTELY NO WARRANTY.
This is free software, and you are welcome to redistribute it
under certain conditions.
------------------------------------------------------------------------------------------*/

#ifndef QSGMLTAG_CPP
#define QSGMLTAG_CPP

#include <QHash>
#include <QRegExp>

class QSgmlTag
{
public:
	typedef QList<QSgmlTag*>         QSgmlTaglist;
	typedef QHash<QString, QString>   QSgmlAtrHash;

	enum TagType
	{
		eVirtualBeginTag,
		eVirtualEndTag,
		eStartTag,
		eEndTag,
		eStandalone,
		eDoctype,
		eCdata,
		eComment,
		eStartEmpty,
		eNoTag,
	};

	int Level;

	QSgmlTag* Parent;
	QSgmlTaglist Children;

	QSgmlAtrHash Attributes;
	QString Name;
	QString Value;
	TagType Type;
	int StartTagPos;
	int StartTagLength;
	int EndTagPos;
	int EndTagLength;

	QSgmlTag(void);
	QSgmlTag(const QString& InnerTag);
	QSgmlTag(const QString& InnerTag, TagType eType, QSgmlTag* tParent);
	~QSgmlTag(void);

	bool checkAttribute(QString AtrName, QString AtrValue);
	QSgmlTag& getNextSibling(void);
	QSgmlTag& getPreviousSibling(void);
	QSgmlTag& getNextElement(void);
	QSgmlTag& getPreviousElement(void);
	QString getAttributeValue(QString Key);
	void resetLevel(void);
	bool hasAttribute(QString AtrName);

	QSgmlTag* addChild(QString InnerTag, TagType eType);

private:
	void SetType(const QString& InnerTag);
	void SetNameAttributes(const QString& InnerTag);
};

extern QSgmlTag NoTag;

#endif // QSGMLTAG_CPP

