#include "Person.h"

static unsigned int nextID = 0;

Person::Person() :
	m_ID( nextID++ )
{
}

Person::Person(const QString& str) :
	m_name (str),
	m_ID( nextID++ )
{

}

Person::Person(const Person& other) :
	m_name (other.m_name),
	m_ID (other.m_ID)
{

}

bool Person::operator==(Person const& other)
{
	return (this->m_ID == other.m_ID);
}

bool Person::operator==(Person::IDType id)
{
	return (this->m_ID == id);
}