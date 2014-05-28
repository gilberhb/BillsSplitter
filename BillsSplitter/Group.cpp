#include "Group.h"

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <algorithm>
#include <iostream>

using std::cout;
using std::endl;

Group::Group()
{
}

void Group::addGroupMember(Person *p)
{
	m_groupMembers.push_back( p );
}

void Group::removeGroupMember(Person::IDType ID)
{
	if (memberCanBeRemoved(ID))
		m_groupMembers.erase( std::remove( std::begin(m_groupMembers), std::end(m_groupMembers), ID), 
		                  std::end(m_groupMembers) );
	else
		throw std::runtime_error("Member cannot be removed because receipts have already been added which reference this user.");
}

size_t Group::size() const
{
	return m_groupMembers.size();
}

Person& Group::member(size_t i)
{
	return m_groupMembers[i];
}


bool MemberIsNotOnReceipt(Person::IDType ID, const Receipt& R )
{
	if (R.GetDebtors().count(ID) == 0 &&
		R.GetPayers().count(ID) == 0)
		return true;
	else
		return false;
}

bool NoReferencesInCollection(Person::IDType ID, const ReceiptCollection& rc)
{
	boost::function<bool (const Receipt&)> MemberIsNotOnReceiptFn = 
		boost::bind( MemberIsNotOnReceipt, ID, _1);
	return std::all_of( rc.cbegin(), rc.cend(), MemberIsNotOnReceiptFn );
}

bool Group::memberCanBeRemoved(Person::IDType ID) const
{
	boost::function<bool (const ReceiptCollection&)> MemberHasNoReferences = 
		boost::bind( NoReferencesInCollection, ID, _1);
	bool result = std::all_of( m_Collections.cbegin(), m_Collections.cend(), MemberHasNoReferences);
	if (result == true)
		cout << "member can be removed" << endl;
	else
		cout << "member cannot be removed" << endl;
	return result;
}