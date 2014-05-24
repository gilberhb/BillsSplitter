#pragma once

#include <QAbstractListModel>
#include "Person.h"

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

	void addMember(Person *p);
	void removeMember(const QModelIndex& index);
	
private:
	Group *m_pGroup;

};