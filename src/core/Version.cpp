#include "Version.h"
#include "versionInfo.h"
#include <QStringList>
char* Version::getVersionStr()
{
	return VER_FILE_VERSION_STR;
}

short Version::Major()
{
	return VERSION_MAJOR;
}

short Version::Minor()
{
	return VERSION_MINOR;
}

short Version::Revision()
{
	return VERSION_REVISION;
}

short Version::Build()
{
	return VERSION_BUILD;
}

const Version& Version::CurrentVersion()
{
	static Version curent(getVersionStr());
	return curent;
}

Version::Version(const QString& versionString): m_versionString(versionString)
{
	m_floatVal = versionToFloat(m_versionString);
}

QString Version::v2fhelper(QString v, QString stuff, float* version, float weight)
{
	float f = 0;
	QStringList parts = v.split(stuff, QString::SkipEmptyParts);
	if (2 != parts.size())
	{
		return v;
	}
	f = v.toFloat();
	version[4] = weight;
	version[5] = f;
	return parts[0];
}

float Version::versionToFloat(QString v)
{
	float version[] = {
		0, 0, 0, 0, // 4-part numerical revision
			4, // alpha, beta, rc or (default) final
			0, // alpha, beta or RC version revision
			1 // Pre or (default) final
	};
	QStringList parts = v.split( "pre" , QString::SkipEmptyParts);
	if (2 == parts.size())
	{
		version[6] = 0;
		v = parts[0];
	}

	v = v2fhelper(v, "a", version, 1);
	v = v2fhelper(v, "b", version, 2);
	v = v2fhelper(v, "rc", version, 3);
	parts = v.split(".");
	for (int i = 0; i < std::min(parts.size(), 4); i++)
	{
		bool ok;
		float f = parts[i].toFloat(&ok);
		if (ok)
		{
			version[i] = f;
		}
		
	}
	float ver = version[0];
	ver += version[1] / 100.0f;
	ver += version[2] / 10000.0f;
	ver += version[3] / 1000000.0f;
	ver += version[4] / 100000000.0f;
	ver += version[5] / 10000000000.0f;
	ver += version[6] / 1000000000000.0f;
	return ver;
}

bool operator<(const Version& lhs, const Version& rhs)
{
	return lhs.m_floatVal < rhs.m_floatVal;
}

bool operator<=(const Version& lhs, const Version& rhs)
{
	return !(rhs < lhs);
}

bool operator>(const Version& lhs, const Version& rhs)
{
	return rhs < lhs;
}

bool operator>=(const Version& lhs, const Version& rhs)
{
	return !(lhs < rhs);
}