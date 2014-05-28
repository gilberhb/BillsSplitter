#include <iostream>
#include <boost/multiprecision/cpp_int.hpp>
#include <QApplication>
#include <QMessageBox>
#include "MainWindow.h"
#include "Person.h"
#include "Group.h"
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <QLocale>

// include headers that implement a archive in simple text format
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
//#include <fstream>

#include <boost/locale.hpp>
#include <fstream>

using std::cout;
using std::cin;
using std::endl;
using std::ofstream;
using std::ifstream;

using boost::multiprecision::cpp_dec_float_50;

int main(int argc, char* argv[])
{
	QApplication app (argc, argv);
	MainWindow mw;
	mw.show();

	try {
		cpp_dec_float_50 x = cpp_dec_float_50("3,14159");
	} catch (std::exception& e) {
		cout << e.what() << endl;
	}

	return app.exec();
}