#include "Emailvalidator.h"

EmailValidator::EmailValidator(QObject *parent) 
	: QValidator(parent)
	, m_validMailRegExp("[a-z0-9._%+-]+@[a-z0-9.-]+\\.[a-z]{2,4}")
	, m_intermediateMailRegExp("[a-z0-9._%+-]*@?[a-z0-9.-]*\\.?[a-z]*")
{
}

QValidator::State EmailValidator::validate(QString &text, int &pos) const
{
	Q_UNUSED(pos)

		fixup(text);

	if (m_validMailRegExp.exactMatch(text))
		return Acceptable;
	if (m_intermediateMailRegExp.exactMatch(text))
		return Intermediate;

	return Invalid;
}

void EmailValidator::fixup(QString &text) const
{
	text = text.trimmed().toLower();
}