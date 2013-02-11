#include "Helper.h"

using namespace std;

string stringify(ostream & in)
{
	return static_cast<ostringstream*>(&in)->str();
}