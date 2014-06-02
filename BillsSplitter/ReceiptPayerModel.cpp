#include "ReceiptPayerModel.h"
#include <iostream>
#include <QLocale>
#include <QString>

using std::cerr;
using std::endl;

ReceiptPayerModel::ReceiptPayerModel(Group *g, Receipt *r, QObject *parent) :
m_pGroup(g),
m_pReceipt(r),
QAbstractItemModel(parent)
{

}

ReceiptPayerModel::~ReceiptPayerModel()
{
	cerr << "ReceiptPayerModel destroyed" << endl;
}

QModelIndex	ReceiptPayerModel::index(int row, int column, const QModelIndex &parent ) const
{
	return createIndex(row, column, static_cast<void*>(0) ); //this model is flat, so don't bother with the pointer
}

QModelIndex	ReceiptPayerModel::parent(const QModelIndex &parent) const
{
	return QModelIndex();
}

int ReceiptPayerModel::rowCount(const QModelIndex &parent) const
{
	return m_pReceipt->GetPayers().size();
}

int ReceiptPayerModel::columnCount(const QModelIndex &parent) const
{
	return 2; //names and amounts
}

void ReceiptPayerModel::RemovePayer( const QModelIndex &index )
{
	if (index == QModelIndex())
		return;
	if ( index.row() >= 0 && index.row() < m_pReceipt->GetPayers().size() ) {
		Receipt::AmountMap::const_iterator iter = m_pReceipt->GetPayers().begin();
		std::advance( iter, index.row() );
		beginResetModel();
		m_pReceipt->RemovePayer( iter->first );
		endResetModel();
	}
}

QVariant ReceiptPayerModel::GetPayerName(const QModelIndex &index) const
{
	//Person::IDType id = std::get<0>( m_pReceipt->GetPayers().at( index.row() ) );
	Receipt::AmountMap::const_iterator iter = m_pReceipt->GetPayers().begin();
	std::advance( iter, index.row() );
	return QVariant( m_pGroup->GetMemberByID( iter->first ).getName() );
}

QVariant ReceiptPayerModel::GetPayerAmount(const QModelIndex &index) const
{
	Receipt::AmountMap::const_iterator iter = m_pReceipt->GetPayers().begin();
	std::advance( iter, index.row() );
	QLocale locale;
	return QVariant( locale.toCurrencyString( iter->second.convert_to<double>() ) );
}

QVariant ReceiptPayerModel::GetData(const QModelIndex &index) const
{
	if (index.column() == 0)
		return GetPayerName(index);
	else if (index.column() == 1)
		return GetPayerAmount(index);
	else
		return QVariant();
}

QVariant ReceiptPayerModel::data(const QModelIndex &index, int role ) const
{
	if (role == Qt::DisplayRole && index != QModelIndex()) {
		return GetData(index);
	} else {
		return QVariant();
	}
}

QVariant ReceiptPayerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		if (section == 0)
			return QVariant( tr("Name") );
		else if (section == 1)
			return QVariant( tr("Amount") );
		else
			return QVariant();
	} else {
		return QVariant();
	}
}

void ReceiptPayerModel::AddPayer( Person::IDType id, Amount a )
{
	if (m_pReceipt->GetPayers().count(id) == 0) {
		beginResetModel();
		m_pReceipt->AddPayerByAmount(id, a);
		endResetModel();
	} else {
		m_pReceipt->AddPayerByAmount(id, a);
	}
}