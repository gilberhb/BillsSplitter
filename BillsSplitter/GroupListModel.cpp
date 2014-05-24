#include "GroupListModel.h"
#include "Group.h"
#include <iostream>

using std::cout;
using std::endl;
using std::cerr;

GroupListModel::GroupListModel(Group *g, QObject *parent) :
	QAbstractListModel(parent),
	m_pGroup(g)
{

}

GroupListModel::~GroupListModel()
{
	std::cerr << "List Model Deleted..." << std::endl;
}

int GroupListModel::rowCount(const QModelIndex &parent) const
{
	return m_pGroup->size();
}

int GroupListModel::columnCount(const QModelIndex &parent) const
{
	return 1;
}

QVariant GroupListModel::data(QModelIndex const& index, int role) const
{
	if (role == Qt::DisplayRole) {
		return QVariant( m_pGroup->member(index.row()).getName() );
	} else {
		return QVariant();
	}
}

QVariant GroupListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (section == 0 && orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return QVariant(tr("Member Name"));
	else 
		return QVariant();
}

void GroupListModel::addMember(Person *p)
{
	beginInsertRows(QModelIndex(), m_pGroup->size(), m_pGroup->size());
	m_pGroup->addGroupMember(p);
	endInsertRows();
}

//remove the member represented by the QModelIndex given
void GroupListModel::removeMember( const QModelIndex& index )
{
	if (index == QModelIndex())
		return;
	int row = index.row();
	if (row < 0 || row >= m_pGroup->size())
		throw std::runtime_error( (QString("Cannot remove member with row number: ") + QString::number(row)).toStdString() );
	
	beginRemoveRows(QModelIndex(), row, row);
	m_pGroup->removeGroupMember( m_pGroup->member(row).getID() );
	endRemoveRows();
}