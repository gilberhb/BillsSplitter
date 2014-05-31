#pragma once

#include <QString>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include "QStringSerialize.h"

class Payee
{
public:
	explicit Payee();
	typedef int IDType;
	Payee(QString const& name);

	void SetName(QString const& name);
	void SetID(IDType id);
	const QString& GetName() const;
	IDType	GetID() const;

	template <class Archive>
	void serialize(Archive &ar, const int version) {
		ar & m_Name;
		ar & m_ID;
	}

private:
	
	QString m_Name;
	IDType	m_ID;
};

//Comparison based on ID number
bool operator<(Payee const&, Payee const&);