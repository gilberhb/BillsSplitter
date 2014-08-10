#include "ReceiptCollectionModel.h"

ReceiptCollectionModel::ReceiptCollectionModel( Group *g, QObject *parent ) :
QAbstractItemModel(parent),
m_pGroup(g)
{

}


QModelIndex	ReceiptCollectionModel::index(int row, int column, const QModelIndex &parent) const
{
	return createIndex(row, column, static_cast<quintptr>(parent.row() + 1) ); //if the internalID is not zero, then the item is a Receipt
}

QModelIndex	ReceiptCollectionModel::parent(const QModelIndex &parent) const
{
	if (parent == QModelIndex())
		return QModelIndex();
	else if (parent.internalId() == 0) //This is a ReceiptCollection
		return QModelIndex();
	else                               //we were passed a receipt
		return createIndex(parent.internalId() - 1, 0, static_cast<quintptr>(0) ); 
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

void ReceiptCollectionModel::AddCollection(const QString& name_in)
{
	QString name = name_in.trimmed();
	if (name == QString())
		return;
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

ReceiptCollection& ReceiptCollectionModel::GetReceiptCollection(const QModelIndex& index)
{
	if (index == QModelIndex() || index.parent() != QModelIndex()) {
		throw std::runtime_error("GetReceipt passed a QModelIndex that doesn't refer to a ReceiptCollection.");
	} else {
		int collectionIndex = index.row();

		return m_pGroup->GetReceiptCollection(collectionIndex);
	}
}

Receipt& ReceiptCollectionModel::GetReceipt(const QModelIndex& index)
{
	if (index == QModelIndex() || index.parent() == QModelIndex()) { //this is a collection, not a receipt
		throw std::runtime_error("GetReceipt passed a QModelIndex that doesn't refer to a receipt.");
	} else {
		int collectionIndex = index.parent().row();
		int receiptIndex = index.row();
		return m_pGroup->GetReceiptCollection(collectionIndex).GetReceipt(receiptIndex);
	}
}

void ReceiptCollectionModel::ChangeReceipt(const QModelIndex& index, Receipt& r)
{
	if (index == QModelIndex() || index.parent() == QModelIndex()) { //this is a collection, not a receipt
		throw std::runtime_error("ChangeReceipt passed a QModelIndex that doesn't refer to a receipt.");
	} else {
		int collectionIndex = index.parent().row();
		int receiptIndex = index.row();
		m_pGroup->GetReceiptCollection(collectionIndex).GetReceipt(receiptIndex) = r;
		QVector<int> roles;
		roles.push_back( Qt::DisplayRole | Qt::UserRole );
		emit dataChanged( index, index, roles );
	}
}
