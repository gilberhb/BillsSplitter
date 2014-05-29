#include "GroupList.h"
#include "GroupListModel.h"
#include "Group.h"
#include <iostream>
#include <stdexcept>
#include <QInputDialog>
#include <QMessageBox>

using std::cerr;
using std::endl;

GroupList::GroupList(QWidget *parent) :
	QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.pushButton_Add, SIGNAL(clicked()), this, SLOT(slot_onAddGroupMember()));
	connect(ui.pushButton_Remove, SIGNAL(clicked()), this, SLOT(slot_onRemoveGroupMember()));

	ui.pushButton_Remove->setEnabled(false);
}

void GroupList::setGroup(Group *g)
{
	if (g == NULL)
		throw std::runtime_error("Cannot set null group");
	std::cerr << "Group pointer received in GroupList..." << std::endl;
	GroupListModel *model = new GroupListModel(g, this);
	ui.listView->setModel(model);

	connect(ui.listView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), 
		this, SLOT(slot_onListSelectionChanged(const QModelIndex&, const QModelIndex&)));
	m_pGroup = g;
}

void GroupList::slot_onAddGroupMember()
{
	try {
		QString name = QInputDialog::getText(this, tr("Create New Group Member"), tr("Name: "));
		if (name.trimmed() != QString()) {
			GroupListModel *model = dynamic_cast<GroupListModel*>(ui.listView->model());
			if (model)
				model->addMember( new Person(name.trimmed()) );
		}
	} catch (std::exception &e) {
		QMessageBox::warning(this, tr("Warning"), QString::fromStdString(e.what()) );
	}
}

void GroupList::slot_onRemoveGroupMember()
{
	try {
		QModelIndexList indices = ui.listView->selectionModel()->selectedRows();
		if (indices.size() > 0) {
			GroupListModel *model = dynamic_cast<GroupListModel*>(ui.listView->model());
			if (model)
				model->removeMember( indices.front() );
		}
	} catch (std::exception &e) {
		cerr << e.what() << endl;
	}
}

void GroupList::slot_onListSelectionChanged(const QModelIndex& current, const QModelIndex&)
{
	GroupListModel *model = dynamic_cast<GroupListModel*>(ui.listView->model());
	if (model && current != QModelIndex()) {
		if (model->memberCanBeRemoved(current))
			ui.pushButton_Remove->setEnabled(true);
		else
			ui.pushButton_Remove->setEnabled(false);
	}
}