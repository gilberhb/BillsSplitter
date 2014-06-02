#pragma once

#include "Person.h"
#include "Receipt.h"
#include "Payee.h"
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/ptr_container/serialize_ptr_vector.hpp>
#include <vector>
#include <QString>
#include "ReceiptCollection.h"

class Group
{
public:
	Group();
	void addGroupMember(Person* p);
	void addGroupPayee(Payee* p);
	const Payee& GetOrAddPayeeByName(QString name);
	int			 GetOrAddCategoryByName(QString name);

	void removeGroupMember(Person::IDType id);
	size_t size() const;
	Person& member(size_t);
	Payee&	payee(size_t);
	const QString&	category(size_t) const;
	const Payee& payee(size_t) const;
	void removePayee(Payee::IDType id);

	const Person&   GetMemberByID(Person::IDType id) const;
	Person&			GetMemberByID(Person::IDType id);
	const Payee&    GetPayeeByID(Payee::IDType id) const;
	Payee&			GetPayeeByID(Payee::IDType id);
	bool			memberCanBeRemoved(Person::IDType id) const;
	bool			payeeCanBeRemoved(Payee::IDType id) const;
	const std::vector<QString>&	GetCategoryList() const;

	int GetNumberOfMembers() const;
	int GetNumberOfPayees() const;
	int GetNumberOfReceiptCollections() const;
	const ReceiptCollection&	GetReceiptCollection(int) const;
	ReceiptCollection&	GetReceiptCollection(int);
	void				RemoveReceiptCollection(int);
	void				AddReceiptCollection(QString const& name);
	bool				TestAddReceiptCollection(QString const& name) const;
	bool				TestAddPayee(QString const& name) const;

	QString	formatReceiptDescription(Receipt const& r);

	template <class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & m_groupMembers;
		ar & m_Collections;
		ar & m_Payees;
		ar & m_Categories;
	}

protected:
	Person::IDType	nextMemberID() const;
	Payee::IDType   nextPayeeID() const;
	
private:
	boost::ptr_vector< Person >				m_groupMembers;
	boost::ptr_vector< ReceiptCollection >	m_Collections;
	boost::ptr_vector< Payee >				m_Payees;
	std::vector<QString>					m_Categories;
};