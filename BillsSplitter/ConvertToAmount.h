#pragma once
#include <locale>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <sstream>
#include <QString>

typedef boost::multiprecision::cpp_dec_float_50	Amount; //decimal floating point with 50 digits of accuracy

Amount StringToAmount(QString const& string);