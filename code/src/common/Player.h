#pragma once

#include "../common/Config.h"

#include <string>
#include <vector>
#include <map>

#include "StemCell.h"

using namespace std;

class Player
{

private:
	unsigned int m_uiId;
	string m_strName;
	
public:

	Player(unsigned int id, string name, Vec2f position) :
	  m_uiId(id),
	  m_strName(name)
	{
	}

	unsigned int getId() const { return m_uiId; }
	const string& getName() const { return m_strName; }
};