#include "Receipt.h"

Receipt::Receipt() :
	m_Amount(0)
{
}

void Receipt::SetTotalAmount(const Amount a)
{
	m_Amount = a;
}

void Receipt::AddPayerByAmount(const Person::IDType id, const Amount a)
{
	m_Payers.insert( std::pair<Person::IDType,Amount>(id, a) );
}

void Receipt::AddDebtorByAmount(const Person::IDType id, const Amount a)
{
	m_Debtors.insert( std::pair<Person::IDType,Amount>(id, a) );
}

void Receipt::RemovePayer(const Person::IDType id)
{
	m_Payers.erase( id );
}

void Receipt::RemoveDebtor(const Person::IDType id)
{
	m_Debtors.erase( id );
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

QString const&		Receipt::GetDescription() const
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