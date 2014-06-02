#pragma once

#include <vector>
#include <QString>
#include "QStringSerialize.h"
#include <boost/serialization/vector.hpp>
#include "Receipt.h"

class ReceiptCollection
{
public:
	ReceiptCollection();
	~ReceiptCollection();
	ReceiptCollection(QString name);

	void SetName(QString name);
	QString GetName() const;

	void AddReceipt(const Receipt &r);
	Receipt& GetReceipt(int);
	const Receipt& GetReceipt(int) const;
	int NumberOfReceipts() const;
	void RemoveReceipt(size_t);

	template <class Archive>
	void serialize(Archive &ar, const unsigned int version = 0)
	{
		ar & m_Receipts;
		ar & m_Name;
	}

	std::vector< Receipt >::const_iterator cbegin() const;
	std::vector< Receipt >::const_iterator cend() const;

	bool operator==(const ReceiptCollection& other) const; 

private:
	std::vector< Receipt >			m_Receipts;
	QString							m_Name;
};