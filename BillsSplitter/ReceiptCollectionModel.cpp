#include "ReceiptCollectionModel.h"

ReceiptCollectionModel::ReceiptCollectionModel( Group *g, QObject *parent ) :
QAbstractItemModel(parent),
m_pGroup(g)
{

}


QModelIndex	ReceiptCollectionModel::index(int row, int column, const QModelIndex &parent) const
{
	return createIndex(row, column, static_cast<quint32>(parent.row() + 1) ); //if the internalID is not zero, then the item is a Receipt
}

QModelIndex	ReceiptCollectionModel::parent(const QModelIndex &parent) const
{
	if (parent == QModelIndex())
		return QModelIndex();
	else if (parent.internalId() == 0) //This is a ReceiptCollection
		return QModelIndex();
	else                               //we were passed a receipt
		return createIndex(parent.internalId() - 1, 0, static_cast<quint32>(0) ); 
}

int ReceiptCollectionModel::rowCount(const QModelIndex &parent) const
{
	if (parent == QModelIndex())
		return m_pGroup->GetNumberOfReceiptCollections();
	else if (parent.internalId() != 0) {
		return 0;
	} else {                             //internalID is zero, but it's not an invalid QModelIndex
		int index = parent.row();
		return m_pGroup->GetReceiptCollection(index).NumberOfReceipts();
	}
}

int ReceiptCollectionModel::columnCount(const QModelIndex &parent) const
{
	return 1;
}

QVariant ReceiptCollectionModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole) {
		if (index.parent() == QModelIndex() ) { //this is a ReceiptCollection 
			return m_pGroup->GetReceiptCollection( index.row() ).GetName();
		} else if (index.parent().internalId() == 0) { //this is a Receipt
			Receipt r = m_pGroup->GetReceiptCollection( index.internalId()-1 ).GetReceipt( index.row() );
			return m_pGroup->formatReceiptDescription(r);
		} else {
			return QVariant();
		}
	} else {
		return QVariant();
	}
}
