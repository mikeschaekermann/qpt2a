#pragma once

class IIdentifiable
{
public:
	static unsigned int getNewId();
protected:
	IIdentifiable();
private:
	static unsigned int nextId;
};

