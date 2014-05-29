#pragma once

#include "Person.h"
#include "Receipt.h"
#include "Payee.h"
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/ptr_container/serialize_ptr_vector.hpp>
#include <vector>
#include <tuple>
#include <QString>

typedef boost::ptr_vector< Receipt >	ReceiptCollection;

class Group
{
public:
	Group();
	void addGroupMember(Person* p);
	void addGroupPayee(Payee* p);
	void removeGroupMember(Person::IDType id);
	size_t size() const;
	Person& member(size_t);
	const Person&   GetMemberByID(Person::IDType id) const;
	Person&   GetMemberByID(Person::IDType id);
	bool	memberCanBeRemoved(Person::IDType id) const;
	bool	payeeCanBeRemoved(Payee::IDType id) const;

	template <class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & m_groupMembers;
		ar & m_Collections;
		ar & m_Payees;
	}

protected:
	Person::IDType	nextMemberID() const;
	Payee::IDType   nextPayeeID() const;
	
private:
	boost::ptr_vector< Person >				m_groupMembers;
	boost::ptr_vector< ReceiptCollection >	m_Collections;
	boost::ptr_vector< Payee >				m_Payees;
};