#pragma once

#include <QString>
// include headers that implement a archive in simple text format
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include "QStringSerialize.h"

class Person
{
public:
	typedef unsigned int IDType;

	explicit Person(QString const& name);
	explicit Person(const Person& other);

	
	QString getName() const { return m_name; }
	IDType getID() const { return m_ID; }
	void SetName(QString const&);
	void SetID(IDType id);

	template <class Archive>
	void serialize(Archive &ar, const int version) {
		ar & m_name;
		ar & m_ID;
	}

	bool operator==(Person const& other);
	bool operator==(IDType id);
private:
	explicit Person();
	QString m_name;
	IDType	m_ID;

};

bool operator<(const Person&, const Person&);