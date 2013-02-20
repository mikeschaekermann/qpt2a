#pragma once

#define POLYCAPACITY PolypeptideCapacityContainer::getInstance()

class PolypeptideCapacityContainer
{
public:
	unsigned int NumberOfPolypeptides;
	unsigned int NumberOfStandardCells;
	unsigned int NumberOfBoneCells;

	static PolypeptideCapacityContainer * getInstance();

	void setPolypeptidesPerStandardCell(unsigned int polypeptidesPerStandardCell);
	void setPolypeptidesPerBoneCell(unsigned int polypeptidesPerBoneCell);
	void setPolypeptidesPerStemCell(unsigned int polypeptidesPerStemCell);

	unsigned int getNumberOfPolypeptidesAllowed();
	unsigned int getNumberOfPolypeptidesExisting();
	/// return is a percentage
	float getExistingPerAllowed();
	/// returns how many places are left
	int getRemainingNumberOfPolypeptidesAllowed();
private:
	static PolypeptideCapacityContainer * instance;

	unsigned int polypeptidesPerStandardCell;
	unsigned int polypeptidesPerBoneCell;
	unsigned int polypeptidesPerStemCell;

	PolypeptideCapacityContainer();
	PolypeptideCapacityContainer(PolypeptideCapacityContainer const &);
	PolypeptideCapacityContainer * operator=(PolypeptideCapacityContainer const &);
};