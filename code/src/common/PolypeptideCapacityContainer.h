#pragma once

#define POLYCAPACITY PolypeptideCapacityContainer::getInstance()

class PolypeptideCapacityContainer
{
public:
	unsigned int NumberOfPolypeptides;
	unsigned int NumberOfStandardCells;
	unsigned int NumberOfBoneCells;

	static PolypeptideCapacityContainer * getInstance();

	unsigned int getNumberOfPolypeptidesAllowed();
	unsigned int getNumberOfPolypeptidesExisting();
	/// return is a percentage
	float getExistingPerAllowed();
	/// returns how many places are left
	int getRemainingNumberOfPolypeptidesAllowed();
private:
	static PolypeptideCapacityContainer * instance;

	const float percentageUsable;
	const unsigned int polypeptidesPerStandardCell;
	const unsigned int polypeptidesPerBoneCell;
	const unsigned int polypeptidesPerStemCell;

	PolypeptideCapacityContainer();
	PolypeptideCapacityContainer(PolypeptideCapacityContainer const &);
	PolypeptideCapacityContainer * operator=(PolypeptideCapacityContainer const &);
};