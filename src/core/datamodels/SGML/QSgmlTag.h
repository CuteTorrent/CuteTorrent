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
#include <QMetaType>
#include <QHash>
#include <QRegExp>

class QSgmlTag : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QSgmlTag* Parent READ GetParent WRITE SetParent)
	Q_PROPERTY(QList<QSgmlTag*> Children READ GetChildren WRITE SetChildren)
	Q_PROPERTY(QString Name READ GetName WRITE SetName)
	Q_PROPERTY(QString Value READ GetValue WRITE SetValue)
	Q_PROPERTY(int Type READ GetType WRITE SetType)
	Q_PROPERTY(int StartTagPos READ GetStartTagPos WRITE SetStartTagPos)
	Q_PROPERTY(int StartTagLength READ GetStartTagLength WRITE SetStartTagLength)
	Q_PROPERTY(int EndTagPos READ GetEndTagPos WRITE SetEndTagPos)
	Q_PROPERTY(int EndTagLength READ GetEndTagLength WRITE SetEndTagLength)
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
public slots:
	bool checkAttribute(QString AtrName, QString AtrValue);
	QSgmlTag& getNextSibling(void);
	QSgmlTag& getPreviousSibling(void);
	QSgmlTag& getNextElement(void);
	QSgmlTag& getPreviousElement(void);
	QString getAttributeValue(QString Key);
	void resetLevel(void);
	bool hasAttribute(QString AtrName);
	QString GetName();
	void SetName(QString vla);
	QSgmlTag* GetParent();
	void SetParent(QSgmlTag* val);
	QSgmlTaglist GetChildren();
	void SetChildren(QSgmlTaglist val);
	QString GetValue();
	void SetValue(QString val);
	int GetType();
	void SetType(int val);
	int GetStartTagPos();
	void SetStartTagPos(int val);
	int GetStartTagLength();
	void SetStartTagLength(int val);
	int GetEndTagPos();
	void SetEndTagPos(int val);
	int GetEndTagLength();
	void SetEndTagLength(int val);
	QSgmlTag* addChild(QString InnerTag, TagType eType);

private:
	void SetType(const QString& InnerTag);
	void SetNameAttributes(const QString& InnerTag);
};

extern QSgmlTag NoTag;
Q_DECLARE_METATYPE(QSgmlTag*)
Q_DECLARE_METATYPE(QList<QSgmlTag*>)

#endif // QSGMLTAG_CPP

