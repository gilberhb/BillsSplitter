#pragma once

#include "Person.h"
#include "Payee.h"
#include <map>
#include <vector>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/serialization/map.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/date_time/gregorian/formatters.hpp>
#include <boost/date_time/gregorian/gregorian_io.hpp>
#include <boost/date_time/gregorian/greg_serialize.hpp>

#include "ConvertToAmount.h"
#include <tuple>

class Receipt
{
public:
	typedef std::map<Person::IDType, Amount>	AmountMap;
	Receipt();

	void AddPayerByAmount(const Person::IDType, const Amount);
	void AddDebtorByAmount(const Person::IDType, const Amount);
	void RemovePayer(const Person::IDType);
	void RemoveDebtor(const Person::IDType);

	void SetPayeeID(const int PayeeID);
	void SetDescription(QString const& desc);
	void SetCategory(const int CategoryID);
	void SetDate(const boost::gregorian::date &d);
	
	const AmountMap&	GetPayers() const;
	const AmountMap&	GetDebtors() const;
	const Payee::IDType	GetPayeeID() const;
	const Amount		GetTotalAmount() const;
	QString				GetDescription() const;
	boost::gregorian::date	GetDate() const;

	template<class Archive>
	void serialize(Archive &ar, int version = 0) {
		ar & m_Payers;
		ar & m_Debtors;
		ar & m_PayeeID;
		ar & m_Desc;
		ar & m_Category;
		ar & m_Date;
	}

private:
	AmountMap m_Payers;
	AmountMap m_Debtors;

	Payee::IDType			m_PayeeID;
	QString					m_Desc;
	int						m_Category;
	boost::gregorian::date	m_Date;
};
