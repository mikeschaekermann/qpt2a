#pragma once
class IIdentifiable
{
public:
	static unsigned int getNewId()
	{
		return nextId++;
	}
private:
	static unsigned int nextId;
};

unsigned int IIdentifiable::nextId = 0;

