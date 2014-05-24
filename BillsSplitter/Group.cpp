#include "Group.h"
#include <algorithm>

Group::Group()
{
}

void Group::addGroupMember(Person *p)
{
	m_groupMembers.push_back( p );
}

void Group::removeGroupMember(Person::IDType ID)
{
	m_groupMembers.erase( std::remove( std::begin(m_groupMembers), std::end(m_groupMembers), ID), 
		                  std::end(m_groupMembers) );
}

unsigned int Group::size() const
{
	return m_groupMembers.size();
}

Person& Group::member(size_t i)
{
	return m_groupMembers[i];
}