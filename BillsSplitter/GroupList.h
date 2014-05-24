#pragma once

#include "ui_GroupList.h"

class Group;

class GroupList : public QWidget
{
	Q_OBJECT
public:
	GroupList(QWidget *parent = 0);
	void setGroup(Group *g);

protected slots:
	void slot_onAddGroupMember();
	void slot_onRemoveGroupMember();

private:
	Ui::Form	ui;
	Group		*m_pGroup;
};