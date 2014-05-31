#include "Receipt.h"
#include <QLocale>
#include <algorithm>
#include <numeric>

Receipt::Receipt() 
{
}


void Receipt::AddPayerByAmount(const Person::IDType id, const Amount a)
{
	m_Payers[id] = a;		 
}

void Receipt::AddDebtorByAmount(const Person::IDType id, const Amount a)
{
	m_Debtors[id] = a;
}

void Receipt::RemovePayer(const Person::IDType id)
{
	m_Payers.erase(id);
}

void Receipt::RemoveDebtor(const Person::IDType id)
{
	m_Debtors.erase(id);
}

const Receipt::AmountMap&	Receipt::GetPayers() const
{
	return m_Payers;
}

const Receipt::AmountMap&	Receipt::GetDebtors() const
{
	return m_Debtors;
}

void Receipt::SetPayeeID(const int PayeeID)
{
	m_PayeeID = PayeeID;
}

const Payee::IDType	Receipt::GetPayeeID() const
{
	return m_PayeeID;
}

QString	Receipt::GetDescription() const
{
	return m_Desc;
}

void Receipt::SetDescription(QString const& desc)
{
	m_Desc = desc;
}

void Receipt::SetCategory(const int CategoryID)
{
	m_Category = CategoryID;
}

Amount operator+(const Amount& a1, Receipt::AmountMap::const_iterator payer)
{
	return a1 + payer->second;
}

Amount StripID(const std::pair<Person::IDType, Amount> p)
{
	return std::get<1>(p);
}

const Amount Receipt::GetTotalAmount() const
{
	std::vector<Amount> amounts;
	std::transform( m_Payers.cbegin(), m_Payers.cend(), std::back_inserter(amounts), StripID);
	return std::accumulate( amounts.begin(), amounts.end(), Amount(0));
}

void Receipt::SetDate(const boost::gregorian::date &d)
{
	m_Date = d;
}

boost::gregorian::date	Receipt::GetDate() const
{
	return m_Date;
}