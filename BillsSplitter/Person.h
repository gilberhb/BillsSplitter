#pragma once

#include <QString>
// include headers that implement a archive in simple text format
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/string.hpp>

template<class Archive>
inline void serialize( Archive& ar, QString& s, const unsigned int file_version )
{
	boost::serialization::split_free(ar, s, file_version);
}

template <class Archive>
void save(Archive& ar, const QString& str, const int version)
{
	ar & str.toStdString();
}

template <class Archive>
void load(Archive& ar, QString& str, const int version)
{
	std::string stdstr;
	ar & stdstr;
	str = QString::fromStdString( stdstr );
}

class Person
{
public:
	typedef unsigned int IDType;

	explicit Person(QString const& name);
	explicit Person(const Person& other);
	Person();
	
	QString getName() const { return m_name; }
	IDType getID() const { return m_ID; }

	template <class Archive>
	void serialize(Archive &ar, const int version) {
		ar & m_name;
		ar & m_ID;
	}

	bool operator==(Person const& other);
	bool operator==(IDType id);
private:

	QString m_name;
	IDType	m_ID;

};