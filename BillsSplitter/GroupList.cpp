#include "GroupList.h"
#include "GroupListModel.h"
#include "Group.h"
#include <iostream>
#include <stdexcept>
#include <QInputDialog>

using std::cerr;
using std::endl;

GroupList::GroupList(QWidget *parent) :
	QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.pushButton_Add, SIGNAL(clicked()), this, SLOT(slot_onAddGroupMember()));
	connect(ui.pushButton_Remove, SIGNAL(clicked()), this, SLOT(slot_onRemoveGroupMember()));
}

void GroupList::setGroup(Group *g)
{
	if (g == NULL)
		throw std::runtime_error("Cannot set null group");
	std::cerr << "Group pointer received in GroupList..." << std::endl;
	GroupListModel *model = new GroupListModel(g, this);
	ui.listView->setModel(model);
	m_pGroup = g;
}

void GroupList::slot_onAddGroupMember()
{
	try {
		QString name = QInputDialog::getText(this, tr("Create New Group Member"), tr("Name: "));
		if (name != QString())
			dynamic_cast<GroupListModel*>(ui.listView->model())->addMember( new Person(name) );
	} catch (std::exception &e) {
		cerr << e.what() << endl;
	}
}

void GroupList::slot_onRemoveGroupMember()
{
	try {
		QModelIndexList indices = ui.listView->selectionModel()->selectedRows();
		if (indices.size() > 0)
			dynamic_cast<GroupListModel*>(ui.listView->model())->removeMember( indices.front() );
	} catch (std::exception &e) {
		cerr << e.what() << endl;
	}
}