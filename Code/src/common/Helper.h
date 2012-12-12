#pragma once

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

template <class T>
string concatenate(string str, T value)
{
	return str + static_cast<ostringstream*>( &(ostringstream() << value) )->str();
}