#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <stdarg.h>

using namespace std;

template<typename T>
string concatenate(string str, T value)
{
	return (str += static_cast<ostringstream*>( &(ostringstream() << value) )->str());
}