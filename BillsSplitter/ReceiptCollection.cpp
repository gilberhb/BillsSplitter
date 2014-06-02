#include "ReceiptCollection.h"
#include <algorithm>
#include <iostream>

using std::cerr;
using std::endl;

ReceiptCollection::ReceiptCollection()
{
}

ReceiptCollection::~ReceiptCollection()
{
	cerr << "Deleted ReceiptCollection" << endl;
}


ReceiptCollection::ReceiptCollection(QString name) :
m_Name(name)
{
}

void ReceiptCollection::SetName(QString name)
{
	m_Name = name;
}

QString ReceiptCollection::GetName() const
{
	return m_Name;
}

void ReceiptCollection::AddReceipt(const Receipt &r)
{
	m_Receipts.push_back(r);
}

std::vector< Receipt >::const_iterator ReceiptCollection::cbegin() const
{
	return m_Receipts.cbegin();
}

std::vector< Receipt >::const_iterator ReceiptCollection::cend() const
{
	return m_Receipts.cend();
}

int ReceiptCollection::NumberOfReceipts() const
{
	return m_Receipts.size();
}

Receipt& ReceiptCollection::GetReceipt(int i) 
{
	return m_Receipts.at(i);
}

const Receipt& ReceiptCollection::GetReceipt(int i) const
{
	return m_Receipts.at(i);
}

bool ReceiptCollection::operator==(const ReceiptCollection& other) const
{
	return (this->GetName() == other.GetName());
}

void ReceiptCollection::RemoveReceipt(size_t index)
{
	std::vector<Receipt>::iterator iter = std::begin(m_Receipts);
	std::advance(iter, index);
	m_Receipts.erase( iter );
}