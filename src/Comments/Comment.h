#ifndef COMMENT_INCLUDED
#define COMMENT_INCLUDED
#include <QString>
#include <QMetaType>

class Comment
{
public:
	Comment();
	QString authorName;
	QString message;
	QString infoHash;
	float rating;
	QString date;
	uint id;
	bool isEditable;
};

inline Comment::Comment(): rating(0), id(0), isEditable(false)
{
}

Q_DECLARE_METATYPE(Comment);

#endif

