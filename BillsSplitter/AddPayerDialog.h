#pragma once

#include <QDialog>
#include "ui_AddPayerDialog.h"
#include "Person.h"

class Group;

class AddPayerDialog : public QDialog
{
	Q_OBJECT
public:
	AddPayerDialog(Group *g, QWidget *parent = 0);
	Person::IDType	GetSelectedID() const;

protected:
	void SetupComboBox();

private:
	Ui::AddPayerDialog ui;
	Group *m_pGroup;
};