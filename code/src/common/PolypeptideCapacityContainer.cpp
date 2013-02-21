#include "PolypeptideCapacityContainer.h"
#include "../common/ConfigurationDataHandler.h"

PolypeptideCapacityContainer::PolypeptideCapacityContainer():
	percentageUsable(CONFIG_FLOAT("data.polypeptide.percentageUsable")),
	polypeptidesPerStandardCell(CONFIG_INT("data.polypeptide.maxPerStandardCell")),
	polypeptidesPerStemCell(CONFIG_INT("data.polypeptide.maxPerStemCell")),
	polypeptidesPerBoneCell(CONFIG_INT("data.polypeptide.maxPerBoneCell")),
	NumberOfPolypeptides(0),
	NumberOfStandardCells(0),
	NumberOfBoneCells(0)
{
}

unsigned int PolypeptideCapacityContainer::getNumberOfPolypeptidesAllowed() const
{
	unsigned int totalNumber =	NumberOfStandardCells * polypeptidesPerStandardCell + 
								NumberOfBoneCells * polypeptidesPerBoneCell +
								polypeptidesPerStemCell;

	unsigned int relativeNumber = unsigned int(ceil(totalNumber * percentageUsable));

	return relativeNumber;
}

unsigned int PolypeptideCapacityContainer::getNumberOfPolypeptidesExisting() const
{
	return NumberOfPolypeptides;
}

float PolypeptideCapacityContainer::getExistingPerAllowed() const
{
	return (float)getNumberOfPolypeptidesExisting() / (float)getNumberOfPolypeptidesAllowed();
}

int PolypeptideCapacityContainer::getRemainingNumberOfPolypeptidesAllowed() const
{
	return getNumberOfPolypeptidesAllowed() - getNumberOfPolypeptidesExisting();
}

bool PolypeptideCapacityContainer::isFull() const
{
	return (getRemainingNumberOfPolypeptidesAllowed() <= 0);
}