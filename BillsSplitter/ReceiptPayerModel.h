#pragma once

#include <QAbstractItemModel>
#include "Person.h"
#include "Group.h"
#include "Receipt.h"

class ReceiptPayerModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	ReceiptPayerModel(Group*, Receipt*, QObject *parent = 0);
	virtual ~ReceiptPayerModel();
	
	virtual QModelIndex	index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex	parent(const QModelIndex &parent) const;
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	void AddPayer( Person::IDType id, Amount a );

protected:
	QVariant GetPayerAmount(const QModelIndex &index) const;
	QVariant GetPayerName(const QModelIndex &index) const;
	QVariant GetData(const QModelIndex &index) const;

private:
	Group *m_pGroup;
	Receipt *m_pReceipt;
};