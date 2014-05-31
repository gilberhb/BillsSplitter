#pragma once

#include <QDialog>
#include "ui_ReceiptUI.h"
#include "Person.h"
#include "Receipt.h"

class Group;

class ReceiptUI : public QDialog
{
	Q_OBJECT
public:
	ReceiptUI(Group *g, QWidget *parent = 0);
	void SetReceiptData(const Receipt &r);
	Receipt	GetReceipt() const;

protected slots:
	void slot_onAddPayer();
	void slot_onRemovePayer();

protected:
	void AddPayer(Person::IDType id, Amount a);
	void FormatReceipt();
	void SetDate(boost::gregorian::date const& d);
	void SetCategory(int);
	void SetPayee(Payee::IDType);
	void SetDescription(QString);

private:
	Ui::ReceiptDialog ui;
	Group			 *m_pGroup;
	Receipt			  m_Receipt;
};