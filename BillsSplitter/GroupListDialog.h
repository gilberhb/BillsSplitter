#pragma once

#include "ui_GroupListDialog.h"

class Group;

class GroupListDialog : public QDialog
{
	Q_OBJECT
public:
	GroupListDialog(Group *g, QWidget *parent = 0);

private:
	Ui::Dialog	ui;
};