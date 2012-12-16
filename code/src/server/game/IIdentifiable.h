#pragma once
class IIdentifiable
{
public:
	static unsigned int getNewId()
	{
		return nextId++;
	}
protected:
	IIdentifiable() {}
private:
	static unsigned int nextId;
};

unsigned int IIdentifiable::nextId = 0;

