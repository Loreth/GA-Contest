#include "CIndividual.h"

CIndividual::CIndividual(int genotypeLength, mt19937 &randEngine, bool generateRandom)
	: randEngine(randEngine)
{
	this->genotypeLength = genotypeLength;
	genotype = new vector<int>;
	fitness = 0;
	fitnessAlreadyCalculated = false;
	if (generateRandom)
	{
		generateRandomGenotype();
	}
}

CIndividual::CIndividual(const CIndividual & other)
	: randEngine(other.randEngine)
{
	genotypeLength = other.genotypeLength;
	genotype = new vector<int>(*other.genotype);
	fitness = other.fitness;
	fitnessAlreadyCalculated = other.fitnessAlreadyCalculated;
}

CIndividual::~CIndividual()
{
	delete genotype;
}

//annular crossover (half of ring)
pair<CIndividual*, CIndividual*> CIndividual::crossover(CIndividual* other)
{
	uniform_int_distribution<> distributionStartPoint(0, genotypeLength - 1);
	int crossoverStartPoint = distributionStartPoint(randEngine);

	CIndividual* firstChild = new CIndividual(*this);
	CIndividual* secondChild = new CIndividual(*other);
	firstChild->setFitnessAlreadyCalculated(false);
	secondChild->setFitnessAlreadyCalculated(false);
	int end = crossoverStartPoint + genotypeLength / 2;
	if (end > genotypeLength)
	{
		for (int i = crossoverStartPoint; i < genotypeLength; i++) {
			firstChild->genotype->at(i) = other->genotype->at(i);
			secondChild->genotype->at(i) = this->genotype->at(i);
		}

		for (int i = 0; i < end - genotypeLength; i++) {
			firstChild->genotype->at(i) = other->genotype->at(i);
			secondChild->genotype->at(i) = this->genotype->at(i);
		}
	}
	else
	{
		for (int i = crossoverStartPoint; i < end; i++) {
			firstChild->genotype->at(i) = other->genotype->at(i);
			secondChild->genotype->at(i) = this->genotype->at(i);
		}
	}

	return pair<CIndividual*, CIndividual*>(firstChild, secondChild);
}

vector<int>* CIndividual::getGenotype()
{
	return genotype;
}

double CIndividual::getFitness()
{
	return fitness;
}

void CIndividual::setFitness(double fitness)
{
	this->fitness = fitness;
}

bool CIndividual::getFitnessAlreadyCalculated()
{
	return fitnessAlreadyCalculated;
}

void CIndividual::setFitnessAlreadyCalculated(bool alreadyCalculated)
{
	fitnessAlreadyCalculated = alreadyCalculated;
}

void CIndividual::generateRandomGenotype()
{
	uniform_int_distribution<> distribution(0, 1);

	for (int i = 0; i < genotypeLength; i++)
	{
		genotype->push_back(distribution(randEngine));
	}
}
