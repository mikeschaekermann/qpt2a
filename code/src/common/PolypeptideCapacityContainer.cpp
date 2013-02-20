#include "PolypeptideCapacityContainer.h"
#include "../common/ConfigurationDataHandler.h"

PolypeptideCapacityContainer::PolypeptideCapacityContainer():
	percentageUsable(CONFIG_FLOAT("data.polypeptide.percentageUsable")),
	polypeptidesPerStandardCell(CONFIG_INT("data.polypeptide.maxPerStandardCell")),
	polypeptidesPerStemCell(CONFIG_INT("data.polypeptide.maxPerStemCell")),
	polypeptidesPerBoneCell(CONFIG_INT("data.polypeptide.maxPerBoneCell"))
{
}

PolypeptideCapacityContainer::PolypeptideCapacityContainer(PolypeptideCapacityContainer const &):
	percentageUsable(CONFIG_FLOAT("data.polypeptide.percentageUsable")),
	polypeptidesPerStandardCell(CONFIG_INT("data.polypeptide.maxPerStandardCell")),
	polypeptidesPerStemCell(CONFIG_INT("data.polypeptide.maxPerStemCell")),
	polypeptidesPerBoneCell(CONFIG_INT("data.polypeptide.maxPerBoneCell"))
{
}

PolypeptideCapacityContainer * PolypeptideCapacityContainer::getInstance()
{
	if (instance == nullptr)
	{
		instance = new PolypeptideCapacityContainer();
	}
	return instance;
}

unsigned int PolypeptideCapacityContainer::getNumberOfPolypeptidesAllowed()
{
	unsigned int totalNumber =	NumberOfStandardCells * polypeptidesPerStandardCell + 
								NumberOfBoneCells * polypeptidesPerBoneCell +
								polypeptidesPerStemCell;

	unsigned int relativeNumber = ceil(totalNumber * percentageUsable);

	return relativeNumber;
}

unsigned int PolypeptideCapacityContainer::getNumberOfPolypeptidesExisting()
{
	return NumberOfPolypeptides;
}

float PolypeptideCapacityContainer::getExistingPerAllowed()
{
	return getNumberOfPolypeptidesExisting() / getNumberOfPolypeptidesAllowed();
}

int PolypeptideCapacityContainer::getRemainingNumberOfPolypeptidesAllowed()
{
	return getNumberOfPolypeptidesAllowed() - getNumberOfPolypeptidesExisting();
}

PolypeptideCapacityContainer * PolypeptideCapacityContainer::operator=(PolypeptideCapacityContainer const &) { return nullptr; }

PolypeptideCapacityContainer * PolypeptideCapacityContainer::instance = nullptr;