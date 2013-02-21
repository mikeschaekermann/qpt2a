#include "PolypeptideCapacityContainer.h"
#include "../common/ConfigurationDataHandler.h"

boost::mutex PolypeptideCapacityContainer::instanceMutex;

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

PolypeptideCapacityContainer::PolypeptideCapacityContainer(PolypeptideCapacityContainer const &):
	percentageUsable(CONFIG_FLOAT("data.polypeptide.percentageUsable")),
	polypeptidesPerStandardCell(CONFIG_INT("data.polypeptide.maxPerStandardCell")),
	polypeptidesPerStemCell(CONFIG_INT("data.polypeptide.maxPerStemCell")),
	polypeptidesPerBoneCell(CONFIG_INT("data.polypeptide.maxPerBoneCell")),
	NumberOfPolypeptides(0),
	NumberOfStandardCells(0),
	NumberOfBoneCells(0)
{
}

PolypeptideCapacityContainer * PolypeptideCapacityContainer::getInstance()
{
	instanceMutex.lock();
	if (instance == nullptr)
	{
		instance = new PolypeptideCapacityContainer();
	}
	instanceMutex.unlock();

	return instance;
}

void PolypeptideCapacityContainer::releaseInstance()
{
	instanceMutex.lock();
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
	instanceMutex.unlock();
}

unsigned int PolypeptideCapacityContainer::getNumberOfPolypeptidesAllowed() const
{
	unsigned int totalNumber =	NumberOfStandardCells * polypeptidesPerStandardCell + 
								NumberOfBoneCells * polypeptidesPerBoneCell +
								polypeptidesPerStemCell;

	unsigned int relativeNumber = ceil(totalNumber * percentageUsable);

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

PolypeptideCapacityContainer * PolypeptideCapacityContainer::operator=(PolypeptideCapacityContainer const &) { return nullptr; }

PolypeptideCapacityContainer * PolypeptideCapacityContainer::instance = nullptr;