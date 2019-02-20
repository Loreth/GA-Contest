#include "CGeneticAlgorithm.h"
#include <algorithm> 

CGeneticAlgorithm::CGeneticAlgorithm(int populationSize, double crossoverProb, double mutationProb, mt19937 &randEngine, CEvaluator &evaluator)
	: evaluator(evaluator), randEngine(randEngine)
{
	this->populationSize = populationSize;
	this->crossoverProb = crossoverProb;
	this->mutationProb = mutationProb;
	this->population = new vector<CIndividual*>;
	expectedMutations = populationSize * evaluator.iGetNumberOfBits() * mutationProb;
}

CGeneticAlgorithm::~CGeneticAlgorithm()
{
	deletePopulation();
}

vector<int> CGeneticAlgorithm::getBestResult()
{
	return bestResult;
}

void CGeneticAlgorithm::runIteration()
{
	calculatePopulationFitness();
	sortPopulation();
	//cut weaker half of population
	while (population->size() > populationSize)
	{
		delete population->at(population->size() - 1);
		population->pop_back();
	}
	calculateBestResult();
	generateNewPopulation();
}

void CGeneticAlgorithm::initialize()
{
	bestFitness = -DBL_MAX;
	bestResult.clear();
	generateRandomPopulation();
}

void CGeneticAlgorithm::generateRandomPopulation()
{
	for (int i = 0; i < populationSize; i++)
	{
		population->push_back(new CIndividual(evaluator.iGetNumberOfBits(), randEngine, true));
	}
}

void CGeneticAlgorithm::calculateBestResult()
{
	if (bestFitness < population->at(0)->getFitness())
	{
		bestFitness = population->at(0)->getFitness();
		bestResult = *population->at(0)->getGenotype();
	}
}

void CGeneticAlgorithm::mutateNewIndividuals()
{
	uniform_int_distribution<> positionDistr(0, evaluator.iGetNumberOfBits() - 1);
	uniform_int_distribution<> individualDistr(populationSize, 2 * populationSize - 1);

	int position;
	int individualIndex;

	for (int i = 0; i < expectedMutations; i++)
	{
		position = positionDistr(randEngine);
		individualIndex = individualDistr(randEngine);
		population->at(individualIndex)->getGenotype()->at(position) = !population->at(individualIndex)->getGenotype()->at(position);
		population->at(individualIndex)->setFitnessAlreadyCalculated(false);
	}
}

void CGeneticAlgorithm::calculatePopulationFitness()
{
	for (int i = 0; i < population->size(); i++)
	{
		if (population->at(i)->getFitnessAlreadyCalculated() == false)
		{
			population->at(i)->setFitness(evaluator.dEvaluate(population->at(i)->getGenotype()));
			population->at(i)->setFitnessAlreadyCalculated(true);
		}
	}
}

void CGeneticAlgorithm::generateNewPopulation()
{
	uniform_real_distribution<> distrProb(0, 1);

	for (int i = 0; i < populationSize / 2; i++)
	{
		CIndividual* firstParent = population->at(i);
		CIndividual* secondParent = population->at(populationSize - i - 1);

		//Random Offspring Generation - if parents are equall
		if (*firstParent->getGenotype() == *secondParent->getGenotype())
		{
			secondParent->generateRandomGenotype();
		}

		if (crossoverProb > distrProb(randEngine))
		{
			pair<CIndividual*, CIndividual*> children = firstParent->crossover(secondParent);
			population->push_back(children.first);
			population->push_back(children.second);
		}
		else
		{
			population->push_back(new CIndividual(*firstParent));
			population->push_back(new CIndividual(*secondParent));
		}
	}

	mutateNewIndividuals();
}


void CGeneticAlgorithm::deletePopulation()
{
	for (int i = 0; i < population->size(); i++)
	{
		delete population->at(i);
	}
	population->clear();
	delete population;
}

void CGeneticAlgorithm::sortPopulation()
{
	sort(population->begin(), population->end(), compareCIndividualPointers);
}

bool CGeneticAlgorithm::compareCIndividualPointers(CIndividual * first, CIndividual * second)
{
	return (first->getFitness() > second->getFitness());
}


