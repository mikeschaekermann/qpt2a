#pragma once

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

template <class T>
string concate(string str, T value)
{
	return str + static_cast<ostringstream*>( &(ostringstream() << value) )->str();
}