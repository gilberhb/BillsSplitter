#include "MainWindow.h"
#include "Group.h"
#include "GroupListDialog.h"
#include "PayeeListDialog.h"
#include "ReceiptUI.h"
#include <QFileDialog>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <fstream>
#include <qfile.h>
#include <QDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <iostream>
#include <QShortcut>

using boost::archive::text_iarchive;
using boost::archive::text_oarchive;
using std::ifstream;
using std::cerr;
using std::endl;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	m_pGroup(new Group())
{
	ui.setupUi(this);


	m_pDataModel.reset( new ReceiptCollectionModel(m_pGroup.get()) );
	ui.treeView->setModel( m_pDataModel.get() );
	ui.treeView->setHeaderHidden(true);
	ui.treeView->setContextMenuPolicy(Qt::CustomContextMenu);

	connectSlots();
}

MainWindow::~MainWindow()
{
}

void MainWindow::connectSlots()
{
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionGroup_Members, SIGNAL(triggered()), this, SLOT(slot_editGroupMembers()));
	connect(ui.actionReceipt, SIGNAL(triggered()), this, SLOT(slot_addReceipt()));
	connect(ui.actionLoad_Group, SIGNAL(triggered()), this, SLOT(slot_OpenGroup()));
	connect(ui.actionSave_Group, SIGNAL(triggered()), this, SLOT(slot_SaveGroup()));
	connect(ui.actionCollection, SIGNAL(triggered()), this, SLOT(slot_addCollection()));
	connect(ui.actionPayees, SIGNAL(triggered()), this, SLOT(slot_EditPayees()));

	QShortcut *shortcut = new QShortcut(this);
	shortcut->setKey( Qt::Key_Delete );
	connect( shortcut, SIGNAL(activated()), this, SLOT(slot_DeletePressed()) );
	connect(ui.treeView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(slot_MakeContextMenu(const QPoint &)));

}

void MainWindow::slot_editGroupMembers()
{
	try {
		GroupListDialog dlg (m_pGroup.get(), this); //pass a pointer to the current group
		dlg.setModal(true);					        //set modal 
		dlg.exec();                                 //create the dialog window
	} catch (std::exception &e) {
		Q_UNUSED(e);
	}
}

void MainWindow::slot_addReceipt()
{
	try {
		ReceiptUI	dlg (m_pGroup.get(), this);
		int result = dlg.exec();

		Receipt R = dlg.GetReceipt();
		//let the data model handle the addition of the receipt
		m_pDataModel->AddReceipt( R, ui.treeView->selectionModel()->currentIndex() );

	} catch (std::exception &e) {
		cerr << e.what() << endl;
	}
}

void MainWindow::slot_OpenGroup()
{
	try {
		QString fileName = QFileDialog::getOpenFileName(this, tr("Open Group"), QDir::home().absolutePath(), QString::fromUtf8(".grp") );
		if (fileName != QString()) {
			std::ifstream file (fileName.toLocal8Bit(), std::ios::in);
			text_iarchive ar (file);
			ui.treeView->setModel(0);
			m_pGroup.reset( new Group() );
			ar & (*m_pGroup); //read into the group
			m_pDataModel.reset( new ReceiptCollectionModel(m_pGroup.get()) );
			ui.treeView->setModel(m_pDataModel.get());
		}
	} catch (std::exception& e) {
		cerr << e.what() << endl;
	}
}

void MainWindow::slot_SaveGroup()
{
	try {
		QString fileName = QFileDialog::getSaveFileName(this, tr("Save Group"), QDir::home().absolutePath(), QString::fromUtf8(".grp") );
		if (fileName != QString()) {
			std::ofstream file (fileName.toLocal8Bit(), std::ios::out);
			text_oarchive ar (file);
			ar & (*m_pGroup); //write group into the archive
		}
	} catch (std::exception& e) {
		cerr << e.what() << endl;
	}
}

void MainWindow::slot_addCollection()
{
	try {
		QString newCollectionName = QInputDialog::getText(this, tr("Create new collection"), tr("Collection Name:"));
		m_pDataModel->AddCollection(newCollectionName);
	} catch (std::exception& e) {
		cerr << e.what() << endl;
	}
}

void MainWindow::slot_EditPayees()
{
	try {
		PayeeListDialog	dlg (m_pGroup.get());
		dlg.exec();
	} catch (std::exception& e) {
		cerr << e.what() << endl;
	}
}

void MainWindow::slot_DeletePressed()
{
	try {
		QModelIndex selected = ui.treeView->selectionModel()->currentIndex();
		if (selected != QModelIndex()) {
			int result = QMessageBox::question(this,tr("Delete?"),tr("Are you sure you want to delete the selected item?"),QMessageBox::No,QMessageBox::Yes);
			if (result == QMessageBox::Yes) {
				m_pDataModel->DeleteItem( selected );
			}
		}
	} catch (std::exception& e) {
		cerr << e.what() << endl;
	}
}

void MainWindow::slot_onBalanceCollection()
{
	try {
		QModelIndex index = ui.treeView->selectionModel()->currentIndex();
		ReceiptCollectionModel *model = dynamic_cast<ReceiptCollectionModel*>( ui.treeView->model() );
		if (model && index != QModelIndex() ) {
			ReceiptCollection& rc = model->GetReceiptCollection(index);
			cerr << &rc << endl;
		}
	} catch (std::exception& e) {
		cerr << e.what() << endl;
	}
}

void MainWindow::slot_onEditReceipt()
{
	try {
		QModelIndex index = ui.treeView->selectionModel()->currentIndex();
		ReceiptCollectionModel *model = dynamic_cast<ReceiptCollectionModel*>( ui.treeView->model() );
		if (model && index != QModelIndex() ) {
			Receipt& r = model->GetReceipt( index );
			ReceiptUI ui (m_pGroup.get());
			ui.SetReceiptData(r);
			ui.exec();

			Receipt newReceipt = ui.GetReceipt();
			model->ChangeReceipt(index, newReceipt); //let the model do this so that the view can update
		}
	} catch (std::exception& e) {
		cerr << e.what() << endl;
	}
}

void MainWindow::slot_MakeContextMenu(const QPoint& pt)
{
	QMenu menu;

	try {
		if ( ui.treeView->rect().contains( pt ) ) {
			ReceiptCollectionModel *model = dynamic_cast<ReceiptCollectionModel*>( ui.treeView->model() );
			if (model) {
				QModelIndex index = ui.treeView->selectionModel()->currentIndex();
				if (index != QModelIndex()) {
					QModelIndex collection = model->GetCollectionIndex( ui.treeView->selectionModel()->currentIndex() );
					if (collection == ui.treeView->selectionModel()->currentIndex() ) {
						menu.addAction(tr("Balance Collection"),this,SLOT(slot_onBalanceCollection()));
						QPoint ptGlobal = ui.treeView->mapToGlobal(pt);
						menu.exec(ptGlobal);
					} else {
						menu.addAction(tr("Edit Receipt"),this,SLOT(slot_onEditReceipt()));
						QPoint ptGlobal = ui.treeView->mapToGlobal(pt);
						menu.exec(ptGlobal);
					}
				}
			}
		}
	} catch (std::exception& e) {
		cerr << e.what() << endl;
	}
}
