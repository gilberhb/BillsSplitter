#include "ConvertToAmount.h"
#include <QLocale>
#include <tuple>
#include <iostream>

static QChar HYPHEN (45); //negative sign in unicode
static QChar MINUS  (8722);

using std::cerr;
using std::endl;

typedef std::tuple<Amount,QString::const_iterator>	ParseResult;
static
const Amount&	ResultAmount(ParseResult const& a)
{
	return std::get<0>(a);
}

QString::const_iterator	ResultIter(ParseResult const& a)
{
	return std::get<1>(a);
}

static
ParseResult ParseWholePart(QString::const_iterator begin, QString::const_iterator end)
{
	Amount a (0);
	QString::const_iterator iter;
	for (iter = begin; iter != end; iter++) {
		if (iter->isDigit()) {
			a *= Amount(10);					//scale up
			a = a + Amount(iter->digitValue()); //add
		} else if (*iter == QLocale().groupSeparator()) {
			//eat it
		} else if (*iter == QLocale().decimalPoint()) {
			iter++; //skip the decimal point
			return ParseResult(a, iter);
		} else {
			throw std::runtime_error("Invalid currency string");
		}
	}
	return ParseResult(a,iter);
}

static
ParseResult ParseDecimalPart(ParseResult const& begin, QString::const_iterator end)
{
	Amount multiplier ("0.1"); //exactly 1 tenth
	Amount decrement ("0.1"); //exactly 1 tenth
	Amount a = ResultAmount(begin);
	QString::const_iterator iter;
	for (iter = ResultIter(begin); iter != end; iter++) {
		if (iter->isDigit()) {
			a += Amount(iter->digitValue())*multiplier;
			multiplier *= decrement;
		} else {
			throw std::runtime_error("Invalid currency string");
		}
	}
	return ParseResult(a,iter);
}

static
ParseResult ParseSign(QString::const_iterator first)
{
	if ( *first == MINUS || *first == HYPHEN ) { //negative sign
		return ParseResult(Amount(-1),first+1);
	} else {
		return ParseResult(Amount(1),first);
	}
}

static 
Amount AcceptString(QString::const_iterator begin, QString::const_iterator end)
{
	ParseResult sign = ParseSign(begin);
	ParseResult result = ParseWholePart(ResultIter(sign), end);
	if ( ResultIter(result) != end )
		result = ParseDecimalPart(result,end);
	return ResultAmount(sign)*ResultAmount(result);
}

QString RemoveCurrencySymbol(QString string)
{
	int count = string.count(QLocale().currencySymbol());
	if (count == 1)
		return string.remove(QLocale().currencySymbol());
	else if (count == 0)
		return string;
	else
		throw std::runtime_error("Invalid currency string");
}

Amount StringToAmount(QString const& string)
{
	if (string == QString() || string == QString(""))
		throw std::runtime_error("Cannot convert empty string");
	QString str = string.trimmed();
	str = RemoveCurrencySymbol(str);
	str = str.trimmed();

	return AcceptString(str.begin(), str.end());
}