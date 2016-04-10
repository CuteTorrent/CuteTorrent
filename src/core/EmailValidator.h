#ifndef EMAILVALIDATOR_H
#define EMAILVALIDATOR_H

#include <QValidator>

QT_BEGIN_NAMESPACE
class QRegExp;
QT_END_NAMESPACE

class EmailValidator : public QValidator
{
	Q_OBJECT
public:
	explicit EmailValidator(QObject* parent = 0);
	State validate(QString& text, int& pos) const override;
	void fixup(QString& text) const override;

private:
	const QRegExp m_validMailRegExp;
	const QRegExp m_intermediateMailRegExp;
};

#endif // EMAILVALIDATOR_H


