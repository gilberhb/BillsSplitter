#include "GroupListDialog.h"

GroupListDialog::GroupListDialog(Group *g, QWidget *parent) :
	QDialog(parent)
{
	ui.setupUi(this);
	ui.widget->setGroup(g);
}

