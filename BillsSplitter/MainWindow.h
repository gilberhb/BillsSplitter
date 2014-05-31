#pragma once

#include <QMainWindow>
#include "ui_mainWindow.h"
#include <boost/smart_ptr/scoped_ptr.hpp>
#include "ReceiptCollectionModel.h"

class Group;

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget *parent = 0);
	virtual ~MainWindow();

public slots:
	void slot_editGroupMembers();
	void slot_addReceipt();
	void slot_OpenGroup();
	void slot_SaveGroup();

private:
	explicit MainWindow(const MainWindow& other); //no copying!
	void connectSlots();
	
	Ui::MainWindow ui;
	boost::scoped_ptr<Group>	m_pGroup;
	boost::scoped_ptr<ReceiptCollectionModel>	m_pDataModel;
};