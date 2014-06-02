#include "ReceiptUI.h"
#include "AddPayerDialog.h"
#include "Group.h"
#include <iostream>
#include "ReceiptPayerModel.h"
#include <QCompleter>
#include <QStringListModel>
#include <algorithm>

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
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(slot_onRemovePayer()));

	ReceiptPayerModel *model = new ReceiptPayerModel(g, &m_Receipt, this);
	ui.tableView->setModel(model);
	ui.tableView->showGrid();
	ui.tableView->horizontalHeader()->setSectionResizeMode( 0, QHeaderView::Stretch );
	ui.tableView->horizontalHeader()->setSectionResizeMode( 1, QHeaderView::Stretch );

	ui.tableView->setSelectionBehavior( QAbstractItemView::SelectRows );
	ui.tableView->setSelectionMode( QAbstractItemView::SingleSelection );

	boost::gregorian::date day = boost::gregorian::day_clock::local_day();
	SetDate(day);

	SetupPayeeCompleter();
	SetupCategoryCompleter();
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
	QModelIndex selected = ui.tableView->selectionModel()->currentIndex();
	if (selected != QModelIndex()) {
		ReceiptPayerModel *m = dynamic_cast<ReceiptPayerModel*>( ui.tableView->model() );
		if (m) {
			m->RemovePayer(selected);
		}
	}
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

void ReceiptUI::SetPayee(Payee::IDType id)
{
	QString payee = m_pGroup->GetPayeeByID(id).GetName();
	ui.lineEdit_Payee->setText(payee);
}

void ReceiptUI::SetDescription(QString s)
{
	ui.lineEdit->setText(s);
}

void ReceiptUI::SetupPayeeCompleter()
{
	QCompleter *completer = new QCompleter(this);
	QStringListModel	*slm = new QStringListModel(this);
	QStringList	sl;
	for (int i = 0; i < m_pGroup->GetNumberOfPayees(); ++i) {
		sl.push_back( m_pGroup->payee(i).GetName() );
	}
	slm->setStringList(sl);
	completer->setModel(slm);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	ui.lineEdit_Payee->setCompleter(completer);
}

void ReceiptUI::SetupCategoryCompleter()
{
	QCompleter *completer = new QCompleter(this);
	QStringListModel	*slm = new QStringListModel(this);
	QStringList	sl;
	std::copy( std::begin( m_pGroup->GetCategoryList() ),
				std::end( m_pGroup->GetCategoryList() ),
				std::back_inserter(sl) );
	slm->setStringList(sl);
	completer->setModel(slm);
	completer->setCaseSensitivity(Qt::CaseInsensitive);
	ui.lineEdit_Category->setCompleter(completer);
}

Payee::IDType	 ReceiptUI::ResolvePayeeID()
{
	const Payee &p = m_pGroup->GetOrAddPayeeByName( ui.lineEdit_Payee->text() );
	return p.GetID();
}

int	 ReceiptUI::ResolveCategoryID()
{
	int cat = m_pGroup->GetOrAddCategoryByName( ui.lineEdit_Category->text() );
	return cat;
}

boost::gregorian::date	ToBoostDate(QDate const& date)
{
	boost::gregorian::date date_out (date.year(), date.month(), date.day() );
	return date_out;
}

int	 ReceiptUI::exec()
{
	int result = QDialog::exec();
	//validate the data input
	m_Receipt.SetPayeeID( ResolvePayeeID() );
	m_Receipt.SetDescription( ui.lineEdit->text() );
	m_Receipt.SetCategory( ResolveCategoryID() );
	m_Receipt.SetDate( ToBoostDate(ui.dateEdit->date()) );

	return result;
}