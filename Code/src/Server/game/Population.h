/**
* Tree Structure for Cells
* Every Cell is for itself a tree
*/

#pragma once

#include "Cell.h"

class Population
{
private:
	struct Branch
	{
		Cell* m_cell;
		Branch** m_children;
		unsigned int m_childrenSize;
	};

	Branch m_root;
public:
	class Indexer
	{
	private:
		Branch* m_current;
	public:
		bool hasChildren()
		{
			return m_current->m_childrenSize > 0;
		}
	};
	Population() { };

	void setRoot(const Cell& cell)
	{
		Cell* c = new Cell(cell);
		m_root.m_cell = c;
	}

	const Cell& getRoot() const
	{
		return *m_root.m_cell;
	}

	unsigned int size() const
	{
		return 0;
	}
};