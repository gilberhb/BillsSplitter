#include "ReceiptUI.h"
#include "AddPayerDialog.h"
#include "Group.h"
#include <iostream>

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
}

const Receipt&	ReceiptUI::GetReceipt() const
{
	return m_Receipt;
}

void ReceiptUI::AddPayer( Person::IDType id )
{
	cerr << "ID: " << id << endl;
}

void ReceiptUI::slot_onAddPayer()
{
	AddPayerDialog d (m_pGroup, this);
	d.setModal(true);
	int result = d.exec();
	if (result == QDialog::Accepted) {
		AddPayer( d.GetSelectedID() );
	}
}

void ReceiptUI::slot_onRemovePayer()
{

}