#include "ReceiptUI.h"
#include "AddPayerDialog.h"
#include "Group.h"
#include <iostream>
#include "ReceiptPayerModel.h"

using std::cerr;
using std::endl;


ReceiptUI::ReceiptUI(Group *g, QWidget *parent) :
QDialog(parent),
m_pGroup(g)
{
	if (g == NULL)
		throw std::runtime_error("Null group passed to ReceiptUI");
	ui.setupUi(this);

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(slot_onAddPayer()));
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(slot_onRemovePayer()));

	ReceiptPayerModel *model = new ReceiptPayerModel(g, &m_Receipt, this);
	ui.tableView->setModel(model);
	ui.tableView->showGrid();
	ui.tableView->horizontalHeader()->setSectionResizeMode( 0, QHeaderView::Stretch );
	ui.tableView->horizontalHeader()->setSectionResizeMode( 1, QHeaderView::Stretch );
}


void ReceiptUI::AddPayer( Person::IDType id, Amount a )
{
	ReceiptPayerModel *m = dynamic_cast<ReceiptPayerModel*>( ui.tableView->model() );
	if (m)
		m->AddPayer(id, a);
}

void ReceiptUI::slot_onAddPayer()
{
	try {
		AddPayerDialog d (m_pGroup, this);
		d.setModal(true);
		int result = d.exec();
		if (result == QDialog::Accepted) {
			AddPayer( d.GetSelectedID(), d.GetAmount() );
		}
	} catch (std::runtime_error& e) {
		cerr << e.what() << endl;
	}
}

void ReceiptUI::slot_onRemovePayer()
{

}



void ReceiptUI::SetReceiptData(const Receipt& r)
{
	m_Receipt = r;
	SetDate(m_Receipt.GetDate());
	//SetCategory(int)
	SetPayee(m_Receipt.GetPayeeID());
	SetDescription(m_Receipt.GetDescription());
}

Receipt ReceiptUI::GetReceipt() const
{

	return m_Receipt;
}

void ReceiptUI::SetDate(boost::gregorian::date const& d)
{
	QDate date (d.year(), d.month(), d.day());
	ui.dateEdit->setDate(date);
	cerr << "Set date" << endl;
}

void ReceiptUI::SetCategory(int)
{

}

void ReceiptUI::SetPayee(Payee::IDType)
{

}

void ReceiptUI::SetDescription(QString s)
{
	ui.lineEdit->setText(s);
}