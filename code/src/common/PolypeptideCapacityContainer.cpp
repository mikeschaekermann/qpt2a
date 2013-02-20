#include "PolypeptideCapacityContainer.h"

PolypeptideCapacityContainer * PolypeptideCapacityContainer::getInstance()
{
	if (instance == nullptr)
	{
		instance = new PolypeptideCapacityContainer();
	}
	return instance;
}

void PolypeptideCapacityContainer::setPolypeptidesPerStandardCell(unsigned int polypeptidesPerStandardCell)
{
	this->polypeptidesPerStandardCell = polypeptidesPerStandardCell;
}

void PolypeptideCapacityContainer::setPolypeptidesPerBoneCell(unsigned int polypeptidesPerBoneCell)
{
	this->polypeptidesPerBoneCell = polypeptidesPerBoneCell;
}

void PolypeptideCapacityContainer::setPolypeptidesPerStemCell(unsigned int polypeptidesPerStemCell)
{
	this->polypeptidesPerStemCell = polypeptidesPerStemCell;
}

unsigned int PolypeptideCapacityContainer::getNumberOfPolypeptidesAllowed()
{
	return 
		NumberOfStandardCells * polypeptidesPerStandardCell +
		NumberOfBoneCells * polypeptidesPerBoneCell +
		polypeptidesPerStemCell;
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

PolypeptideCapacityContainer::PolypeptideCapacityContainer() {}
PolypeptideCapacityContainer::PolypeptideCapacityContainer(PolypeptideCapacityContainer const &) {}
PolypeptideCapacityContainer * PolypeptideCapacityContainer::operator=(PolypeptideCapacityContainer const &) { return nullptr; }

PolypeptideCapacityContainer * PolypeptideCapacityContainer::instance = nullptr;