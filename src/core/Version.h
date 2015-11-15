#ifndef _Version_INCLUDED_
#define _Version_INCLUDED_
#include <QString>

class Version
{
	friend bool operator<(const Version& lhs, const Version& rhs);
	friend bool operator<=(const Version& lhs, const Version& rhs);
	friend bool operator>(const Version& lhs, const Version& rhs);
	friend bool operator>=(const Version& lhs, const Version& rhs);
	friend bool operator==(const Version& lhs, const Version& rhs);
public:
	static char* getVersionStr();
	static short Major();
	static short Minor();
	static short Revision();
	static short Build();
	static const Version& CurrentVersion();
	explicit Version(const QString& versionString);


private:
	QString m_versionString;
	float m_floatVal;
	QString v2fhelper(QString v, QString stuff, float* version, float weight);
	float versionToFloat(QString versionString);
};
#endif
