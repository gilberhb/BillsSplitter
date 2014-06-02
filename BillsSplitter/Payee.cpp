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

bool Payee::operator==(Payee const& other) const
{
	return (this->m_ID == other.m_ID);
}

bool Payee::operator==(IDType id) const
{
	return (this->m_ID == id);
}
