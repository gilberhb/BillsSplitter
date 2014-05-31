#pragma once

#include <QAbstractItemModel>
#include "Group.h"

class ReceiptCollectionModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	ReceiptCollectionModel( Group *g, QObject *parent = 0 );
	
	virtual QModelIndex	index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex	parent(const QModelIndex &parent) const;
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
private:
	Group *m_pGroup;
};