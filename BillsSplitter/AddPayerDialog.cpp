#include "AddPayerDialog.h"
#include "Group.h"
#include "ConvertToAmount.h"

AddPayerDialog::AddPayerDialog(Group *g, QWidget *parent) :
QDialog(parent),
m_pGroup(g)
{
	if (g == NULL)
		throw std::runtime_error("Null group passed to AddPayerDialog");
	ui.setupUi(this);

	SetupComboBox();
}

Person::IDType AddPayerDialog::GetSelectedID() const
{
	return static_cast<Person::IDType>( ui.comboBox->currentData().toInt() );
}

Amount AddPayerDialog::GetAmount() const
{
	return StringToAmount( ui.lineEdit->text() );
}


void AddPayerDialog::SetupComboBox()
{
	int N = m_pGroup->size();
	for (int i = 0; i < N; ++i) {
		ui.comboBox->addItem(m_pGroup->member(i).getName(), QVariant(m_pGroup->member(i).getID()));
	}
}