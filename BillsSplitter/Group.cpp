#include "Group.h"

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <algorithm>
#include <iostream>
#include <QLocale>
#include <QDate>

using std::cout;
using std::endl;

Group::Group()
{
}

Person::IDType Group::nextMemberID() const 
{
	boost::ptr_vector<Person>::const_iterator iter = std::max_element( m_groupMembers.begin(), m_groupMembers.end() );
	if (iter != m_groupMembers.end())
		return Person::IDType( iter->getID() + 1 );
	else
		return 0;
}

Payee::IDType Group::nextPayeeID() const 
{
	boost::ptr_vector<Payee>::const_iterator iter = std::max_element( m_Payees.begin(), m_Payees.end() );
	if (iter != m_Payees.end())
		return Payee::IDType( iter->GetID() + 1 );
	else
		return 0;
}

bool PersonNameIsNot(QString name, const Person &p)
{
	if ( p.getName() == name )
		return false;
	else
		return true;
}

bool PayeeNameIsNot(QString name, const Payee &p)
{
	if ( p.GetName() == name )
		return false;
	else
		return true;
}

void Group::addGroupMember(Person *p)
{
	if (std::all_of(m_groupMembers.begin(), m_groupMembers.end(), 
		            boost::bind( PersonNameIsNot, p->getName(), _1 ) ) ) 
	{
		p->SetID(nextMemberID());
		m_groupMembers.push_back( p );
	} else {
		throw std::runtime_error("Someone else with that name already exists in the group, so no group member will be added.");
	}
}

void Group::addGroupPayee(Payee *p)
{
	if (std::all_of(m_Payees.begin(), m_Payees.end(), 
					boost::bind( PayeeNameIsNot, p->GetName(), _1 ) ) ) 
	{
		p->SetID(nextPayeeID());
		m_Payees.push_back( p );
	} else {
		throw std::runtime_error("Someone else with that name already exists in the group, so no group member will be added.");
	}
}

void Group::removeGroupMember(Person::IDType ID)
{
	if (memberCanBeRemoved(ID))
		m_groupMembers.erase( std::remove( std::begin(m_groupMembers), std::end(m_groupMembers), ID), 
		                  std::end(m_groupMembers) );
	else
		throw std::runtime_error("Member cannot be removed because receipts have already been added which reference this user.");
}

void Group::removePayee(Payee::IDType ID)
{
	if (payeeCanBeRemoved(ID))
		m_Payees.erase( std::remove( std::begin(m_Payees), std::end(m_Payees), ID), 
		                  std::end(m_Payees) );
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
	if ( R.GetPayers().count(ID) == 0 &&
		R.GetDebtors().count(ID) == 0 )
		return true;
	else
		return false;
}

bool PayeeIsNotOnReceipt(Person::IDType ID, const Receipt& R )
{
	if (R.GetPayeeID() != ID)
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

bool NoReferencesInCollection(Payee::IDType ID, const ReceiptCollection& rc)
{
	boost::function<bool (const Receipt&)> PayeeIsNotOnReceiptFn = 
		boost::bind( PayeeIsNotOnReceipt, 
		             ID, _1);
	return std::all_of( rc.cbegin(), rc.cend(), PayeeIsNotOnReceiptFn );
}

bool Group::memberCanBeRemoved(Person::IDType ID) const
{
	boost::function<bool (const ReceiptCollection&)> MemberHasNoReferences = 
		boost::bind( static_cast< bool (*)(Person::IDType, ReceiptCollection const&) >(NoReferencesInCollection),
		             ID, _1);
	bool result = std::all_of( m_Collections.cbegin(), m_Collections.cend(), MemberHasNoReferences);
	if (result == true)
		cout << "member can be removed" << endl;
	else
		cout << "member cannot be removed" << endl;
	return result;
}

bool Group::payeeCanBeRemoved(Payee::IDType ID) const
{
	boost::function<bool (const ReceiptCollection&)> payeeHasNoReferences = 
		boost::bind( static_cast< bool (*)(Payee::IDType, ReceiptCollection const&) >(NoReferencesInCollection),
		             ID, _1);
	bool result = std::all_of( m_Collections.cbegin(), m_Collections.cend(), payeeHasNoReferences);
	if (result == true)
		cout << "payee can be removed" << endl;
	else
		cout << "payee cannot be removed" << endl;
	return result;
}

bool MemberIDMatch(Person::IDType id, const Person& p)
{
	if (p.getID() == id)
		return true;
	else
		return false;
}

const Person&   Group::GetMemberByID(Person::IDType id) const
{
	boost::ptr_vector<Person>::const_iterator  iter = std::find_if( m_groupMembers.begin(), m_groupMembers.end(),
																 boost::bind(MemberIDMatch, id, _1) );
	if (iter == m_groupMembers.end())
		throw std::runtime_error("Could not find person with given id.");
	else
		return *iter;
}

Person&   Group::GetMemberByID(Person::IDType id)
{
	boost::ptr_vector<Person>::iterator  iter = std::find_if( m_groupMembers.begin(), m_groupMembers.end(),
																 boost::bind(MemberIDMatch, id, _1) );
	if (iter == m_groupMembers.end())
		throw std::runtime_error("Could not find person with given id.");
	else
		return *iter;
}

int Group::GetNumberOfReceiptCollections() const
{
	return m_Collections.size();
}

const ReceiptCollection&	Group::GetReceiptCollection(int i) const
{
	return m_Collections.at(i);
}

ReceiptCollection&	Group::GetReceiptCollection(int i)
{
	return m_Collections.at(i);
}

static
QDate	ToQtDate( boost::gregorian::date date )
{
	QDate date_out ( date.year(), date.month(), date.day() );
	return date_out;
}

QString	Group::formatReceiptDescription(Receipt const& r)
{
	QLocale locale;
	QString str = locale.toCurrencyString( r.GetTotalAmount().convert_to<double>() );
	str.append(": ");
	str.append( GetPayeeByID( r.GetPayeeID() ).GetName() );
	str.append(": ");
	str.append( r.GetDescription() );
	str.append(", ");
	str.append( ToQtDate( r.GetDate() ).toString( locale.dateFormat( QLocale::ShortFormat ) ) );
	return str;
}

bool CollectionNameEquals(const ReceiptCollection& rc, QString const& name)
{
	if (rc.GetName() == name)
		return true;
	else
		return false;
}

void Group::AddReceiptCollection(QString const& name)
{
	if ( TestAddReceiptCollection(name) )
		m_Collections.push_back( new ReceiptCollection(name) );
	else 
		throw std::runtime_error("Can't add receipt collection with a name that's already been used for a previous receipt collection");
}

bool Group::TestAddReceiptCollection(QString const& name) const
{
	if ( std::any_of(m_Collections.begin(), m_Collections.end(), 
		            boost::bind(CollectionNameEquals, _1, name)) )
		return false;
	else {
		return true;
	}
}

int Group::GetNumberOfMembers() const
{
	return m_groupMembers.size();
}

int Group::GetNumberOfPayees() const
{
	return m_Payees.size();
}

const Payee& Group::payee(size_t i) const
{
	return m_Payees.at(i);
}

Payee& Group::payee(size_t i)
{
	return m_Payees.at(i);
}

bool Group::TestAddPayee(const QString& name) const
{
	if (std::all_of(m_Payees.begin(), m_Payees.end(), 
					boost::bind( PayeeNameIsNot, name, _1 ) ) ) 
		return true;
	else 
		return false;

}

bool PayeeIDMatch(Payee::IDType id, const Payee& p)
{
	if (p.GetID() == id)
		return true;
	else
		return false;
}

bool PayeeNameMatch(QString name, const Payee& p)
{
	if (p.GetName() == name)
		return true;
	else
		return false;
}


const Payee&   Group::GetOrAddPayeeByName(QString name) 
{
	//first try to find it
	boost::ptr_vector<Payee>::const_iterator iter;
	iter = std::find_if( boost::begin(m_Payees), boost::end(m_Payees), 
		                 boost::bind( PayeeNameMatch, name, _1 ) );
	if (iter != boost::end(m_Payees)) { //found it
		return *iter;
	}
	else {
		addGroupPayee( new Payee(name) );
		return GetOrAddPayeeByName(name); //now go back and try again
	}
}

const Payee&   Group::GetPayeeByID(Payee::IDType id) const
{
	boost::ptr_vector<Payee>::const_iterator iter;
	iter = std::find_if( boost::begin(m_Payees), boost::end(m_Payees), 
		                 boost::bind( PayeeIDMatch, id, _1 ) );
	if (iter != boost::end(m_Payees))
		return *iter;
	else
		throw std::runtime_error("Cannot find payee with id given.");

}

Payee&			Group::GetPayeeByID(Payee::IDType id)
{
	boost::ptr_vector<Payee>::iterator iter;
	iter = std::find_if( boost::begin(m_Payees), boost::end(m_Payees), 
		                 boost::bind( PayeeIDMatch, id, _1 ) );
	if (iter != boost::end(m_Payees))
		return *iter;
	else
		throw std::runtime_error("Cannot find payee with id given.");
}

int				Group::GetOrAddCategoryByName(QString name)
{
	std::vector<QString>::iterator iter;
	iter = std::find( std::begin(m_Categories), std::end(m_Categories), name);
	size_t index = std::distance( std::begin(m_Categories), iter );
	if ( index < m_Categories.size() )
		return index;
	else {
		m_Categories.push_back(name);
		return m_Categories.size()-1;
	}
}

const QString&	Group::category(size_t index) const
{
	return m_Categories.at(index);
}

const std::vector<QString>&	Group::GetCategoryList() const
{
	return m_Categories;
}

void				Group::RemoveReceiptCollection(int index)
{
	boost::ptr_vector< ReceiptCollection >::iterator iter = boost::begin(m_Collections);
	std::advance( iter, index );
	m_Collections.erase( iter );
}
