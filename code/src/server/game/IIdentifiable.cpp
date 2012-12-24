#include "IIdentifiable.h"

unsigned int IIdentifiable::nextId = 0;

unsigned int IIdentifiable::getNewId()
{
	return nextId++;
}

IIdentifiable::IIdentifiable() {}

