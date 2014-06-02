#pragma once

#include <QDialog>
#include "ui_PayeeListDialog.h"
class Group;

class PayeeListDialog : public QDialog
{
	Q_OBJECT
public:
	PayeeListDialog(Group *g, QWidget *parent = 0);

public slots:
	void slot_onAddPayee();
	void slot_onRemovePayee();
	void slot_onUpdateSelection(const QModelIndex&, const QModelIndex& );

protected:
	void InitModel();

private:
	Ui::PayeeListDialog	ui;
	Group *m_pGroup;
};