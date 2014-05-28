#pragma once

#include "Person.h"
#include <map>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/serialization/map.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/date_time/gregorian/greg_serialize.hpp>
#include "ConvertToAmount.h"

class Receipt
{
public:
	typedef std::map<Person::IDType,Amount>	AmountMap;
	Receipt();

	void SetTotalAmount(const Amount);
	void AddPayerByAmount(const Person::IDType, const Amount);
	void AddDebtorByAmount(const Person::IDType, const Amount);
	void RemovePayer(const Person::IDType);
	void RemoveDebtor(const Person::IDType);
	
	const AmountMap&	GetPayers() const;
	const AmountMap&	GetDebtors() const;

	template<class Archive>
	void serialize(Archive &ar, int version = 0) {
		ar & m_Amount;
		ar & m_Payers;
		ar & m_Allocation;
		ar & m_MerchantID;
		ar & m_Desc;
		ar & m_Category;
	}

private:
	Amount	m_Amount;
	std::map<Person::IDType, Amount> m_Payers;
	std::map<Person::IDType, Amount> m_Debtors;

	int			m_MerchantID;
	QString		m_Desc;
	int			m_Category;
};