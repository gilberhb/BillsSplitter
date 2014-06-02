#include "PayeeListDialog.h"
#include "Group.h"
#include <QStringListModel>
#include <QShortcut>
#include <QInputDialog>
#include <QMessageBox>

PayeeListDialog::PayeeListDialog(Group *g, QWidget *parent) :
QDialog(parent),
m_pGroup(g)
{
	ui.setupUi(this);
	QStringListModel	*model = new QStringListModel(this);
	ui.listView->setModel(model);
	InitModel();

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(slot_onAddPayee()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(slot_onRemovePayee()));

	QShortcut *shortcut = new QShortcut( Qt::Key_Delete, this );
	connect(shortcut, SIGNAL(activated()), this, SLOT(slot_onRemovePayee()));

	ui.pushButton_2->setEnabled(false);
	connect(ui.listView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&,const QModelIndex&)),
		    this, SLOT(slot_onUpdateSelection(const QModelIndex&,const QModelIndex&)));
}

void PayeeListDialog::InitModel()
{
	QStringListModel *model = dynamic_cast<QStringListModel*>( ui.listView->model() );

	if (model) {
		for (int i = 0; i < m_pGroup->GetNumberOfPayees(); ++i) {
			model->insertRow(i);
			QMap<int,QVariant> data;
			data[Qt::DisplayRole] = m_pGroup->payee(i).GetName();
			data[Qt::UserRole] = QVariant( m_pGroup->payee(i).GetID() );
			model->setItemData( model->index(i), data );
		}
	}
}

void PayeeListDialog::slot_onAddPayee()
{
	QString name = QInputDialog::getText(this, tr("Add New Payee"), tr("Name:") );
	name = name.trimmed();
	if (m_pGroup->TestAddPayee(name)) {
		m_pGroup->addGroupPayee( new Payee(name) );
		if ( ui.listView->model()->insertRow( m_pGroup->GetNumberOfPayees()-1 ) ) {
			QMap<int,QVariant>	data;
			data[Qt::DisplayRole] = name;
			data[Qt::UserRole] = m_pGroup->payee( m_pGroup->GetNumberOfPayees()-1 ).GetID();
			ui.listView->model()->setItemData( ui.listView->model()->index( m_pGroup->GetNumberOfPayees()-1, 0 ),
												data );
		} else {
			QMessageBox::warning(this,tr("Warning"),tr("Payee could not be added."));
		}
	} else {
		QMessageBox::warning(this,tr("Warning"),tr("New Payee not added because that name already exists in the list of Payees"));
	}
}

void PayeeListDialog::slot_onRemovePayee()
{
	QModelIndex index = ui.listView->selectionModel()->currentIndex();
	QVariant	data = ui.listView->model()->data(index, Qt::UserRole);
	Payee::IDType id = static_cast<Payee::IDType>(data.toInt());
	if ( m_pGroup->payeeCanBeRemoved( id ) ) {
		m_pGroup->removePayee( id );
		ui.listView->model()->removeRow( index.row() );
	}
}

void PayeeListDialog::slot_onUpdateSelection(const QModelIndex &current, const QModelIndex&)
{
	if (current == QModelIndex()) {
		ui.pushButton_2->setEnabled(false);
	} else {
		QVariant	data = ui.listView->model()->data(current, Qt::UserRole);
		Payee::IDType id = static_cast<Payee::IDType>(data.toInt());
		if ( m_pGroup->payeeCanBeRemoved( id ) ) {
			ui.pushButton_2->setEnabled(true);
		} else {
			ui.pushButton_2->setEnabled(false);
		}
	}
}