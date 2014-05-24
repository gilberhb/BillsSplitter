#pragma once

#include "Person.h"
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/serialize_ptr_vector.hpp>

class Group
{

public:
	Group();
	void addGroupMember(Person* p);
	void removeGroupMember(Person::IDType id);
	unsigned int size() const;
	Person& member(size_t);

	template <class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & m_groupMembers;
	}

private:
	boost::ptr_vector<Person>	m_groupMembers;
};