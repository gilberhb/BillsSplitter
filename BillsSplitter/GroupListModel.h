#pragma once

#include <QAbstractListModel>
#include "Person.h"
#include <vector>

class Group;

class GroupListModel : public QAbstractListModel
{
	Q_OBJECT
public:
	GroupListModel(Group *g, QObject *parent = 0);
	virtual ~GroupListModel();
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = 0) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual void sort(int column , Qt::SortOrder o = Qt::AscendingOrder);

	void addMember(Person *p);
	void removeMember(const QModelIndex& index);
	Person& GetMember(const QModelIndex& index) const;
	bool memberCanBeRemoved(const QModelIndex &index) const;
	
private:
	Group *m_pGroup;
	std::vector<int>		m_sortVector;
};