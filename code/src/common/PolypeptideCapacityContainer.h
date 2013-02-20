#pragma once

#define POLYCAPACITY PolypeptideCapacityContainer::getInstance()

class PolypeptideCapacityContainer
{
public:
	unsigned int NumberOfPolypeptides;
	unsigned int NumberOfStandardCells;
	unsigned int NumberOfBoneCells;

	static PolypeptideCapacityContainer * getInstance();

	unsigned int getNumberOfPolypeptidesAllowed() const;
	unsigned int getNumberOfPolypeptidesExisting() const;
	/// return is a percentage
	float getExistingPerAllowed() const;
	/// returns how many places are left
	int getRemainingNumberOfPolypeptidesAllowed() const;
	/// returns a flag indicating whether
	/// the poly capacity is full,
	/// i.e. no more polys can be added
	bool isFull() const;

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