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

void ReceiptCollectionModel::AddCollection(const QString& name)
{
	if (m_pGroup->TestAddReceiptCollection(name)) {
		beginInsertRows(QModelIndex(), m_pGroup->GetNumberOfReceiptCollections(), m_pGroup->GetNumberOfReceiptCollections());
		m_pGroup->AddReceiptCollection(name);
		endInsertRows();
	} else {
		throw std::runtime_error("Can't add new collection with that name.");
	}
}

QModelIndex const&	ReceiptCollectionModel::GetCollectionIndex(QModelIndex const& index)
{
	if (index == QModelIndex())
		throw std::runtime_error("Invalid index passed to GetCollection");
	else if (index.parent() == QModelIndex())
		return index;
	else return GetCollectionIndex(index.parent());
}

void ReceiptCollectionModel::AddReceipt(const Receipt& R, QModelIndex const& selected)
{
	//find the modelindex for the receiptcollection
	QModelIndex collection = GetCollectionIndex(selected);
	beginInsertRows( collection, rowCount(collection), rowCount(collection) );
	m_pGroup->GetReceiptCollection(collection.row()).AddReceipt(R);
	endInsertRows();
}

void ReceiptCollectionModel::DeleteCollection(const QModelIndex& index)
{
	beginRemoveRows( QModelIndex(), index.row(), index.row() );
	m_pGroup->RemoveReceiptCollection( index.row() );
	endRemoveRows();
}

void ReceiptCollectionModel::DeleteReceipt(const QModelIndex& index)
{
	int collectionIndex = index.parent().row();
	beginRemoveRows( index.parent(), index.row(), index.row() );
	m_pGroup->GetReceiptCollection(collectionIndex).RemoveReceipt( index.row() );
	endRemoveRows();
}

void ReceiptCollectionModel::DeleteItem(const QModelIndex& index)
{
	if (index == QModelIndex()) {
		return;
	} else if (index.parent() == QModelIndex()) {//this is a whole collection!
		DeleteCollection(index);
	} else {
		DeleteReceipt(index);
	}
}