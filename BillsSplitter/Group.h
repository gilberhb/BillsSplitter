#pragma once

#include "Person.h"
#include "Receipt.h"
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/ptr_container/serialize_ptr_vector.hpp>
#include <vector>

typedef boost::ptr_vector< Receipt >	ReceiptCollection;

class Group
{
public:
	Group();
	void addGroupMember(Person* p);
	void removeGroupMember(Person::IDType id);
	size_t size() const;
	Person& member(size_t);
	bool	memberCanBeRemoved(Person::IDType id) const;

	template <class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & m_groupMembers;
	}

	
private:
	boost::ptr_vector< Person >				m_groupMembers;
	boost::ptr_vector< ReceiptCollection >	m_Collections;
};