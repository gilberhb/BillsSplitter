#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>
#include <QApplication>
#include <QMessageBox>
#include "MainWindow.h"
#include "Person.h"
#include "Group.h"

// include headers that implement a archive in simple text format
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <fstream>

using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;

int main(int argc, char* argv[])
{
	QApplication app (argc, argv);
	MainWindow mw;
	mw.show();

	//const char* textstr = "Hµnter";
	{
		Group g;
		g.addGroupMember( new Person("Hunter G") );
		g.addGroupMember( new Person("Phil S") );
		ofstream stream ("test.txt");
		boost::archive::text_oarchive ar (stream);
		ar & g;
	}

	{
		Group g;
		ifstream stream ("test.txt");
		boost::archive::text_iarchive ar (stream);
		ar & g;
		QMessageBox::warning(0,"Warning",QString("Group members: ") + QString::number(g.size()) + QString("<br />Names: ") + g.member(0).getName() + QString(",") + g.member(1).getName());
	}

	//{
	//	Person p ( QString::fromUtf8(textstr) );
	//	ofstream stream ("test.txt");
	//	boost::archive::text_oarchive ar (stream);
	//	ar & p;
	//	QMessageBox::warning(0, "Warning 1", p.getName() + QString(" ") + QString::number(p.getID()) );
	//}

	//{
	//	ifstream stream ("test.txt");
	//	boost::archive::text_iarchive ar (stream);
	//	Person p;
	//	ar & p;
	//	QMessageBox::warning(0, "Warning 2", p.getName() + QString(" ") + QString::number(p.getID()) );
	//}
	return app.exec();
}