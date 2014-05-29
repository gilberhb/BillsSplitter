#include "Payee.h"

Payee::Payee(QString const& name) :
m_Name(name)
{

}

Payee::Payee()
{

}

const QString& Payee::GetName() const
{
	return m_Name;
}

Payee::IDType	Payee::GetID() const
{
	return m_ID;
}

void Payee::SetName(QString const& name)
{
	m_Name = name;
}

void Payee::SetID(IDType id)
{
	m_ID = id;
}

bool operator<(Payee const& p1, Payee const& p2)
{
	return (p1.GetID() < p2.GetID());
}