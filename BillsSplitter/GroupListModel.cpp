#include "GroupListModel.h"
#include "Group.h"
#include <iostream>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <algorithm>

using std::cout;
using std::endl;
using std::cerr;

GroupListModel::GroupListModel(Group *g, QObject *parent) :
	QAbstractListModel(parent),
	m_pGroup(g)
{
	sort(0, Qt::AscendingOrder);
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
		int memberNumber = m_sortVector.at(index.row());
		return QVariant( m_pGroup->member(memberNumber).getName() );
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
	
	sort(0, Qt::AscendingOrder);
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
	int memberNumber = m_sortVector.at(row);
	m_pGroup->removeGroupMember( m_pGroup->member(memberNumber).getID() );
	endRemoveRows();
	sort(0, Qt::AscendingOrder);
}

std::vector<int>	IntegerSequence(int i1, int i2)
{
	std::vector<int>	v (i2 - i1 + 1);
	for (int i = i1; i < i2; ++i)
		v[i - i1] = i;
	return v;
}

bool SortFunction(int i1, int i2, Group *g)
{
	QString name1 = g->member(i1).getName();
	QString name2 = g->member(i2).getName();

	bool result = (name1.compare(name2) < 0) ? true : false;
	return result;
}

void GroupListModel::sort( int column, Qt::SortOrder o )
{
	typedef std::vector<int>::iterator Iterator;
	boost::function< bool(int,int) > sortFun =
		boost::bind(&SortFunction, _1, _2, m_pGroup);
	m_sortVector = IntegerSequence(1,m_pGroup->size());
	std::sort( m_sortVector.begin(), m_sortVector.end(), sortFun );
}