#pragma once
#include "CIndividual.h"
#include "Evaluator.h"
#include <vector>
#include <random>
#include <algorithm>

class CGeneticAlgorithm
{
public:
	CGeneticAlgorithm(int populationSize, double crossoverProb, double mutationProb, mt19937 &randEngine, CEvaluator &evaluator);
	~CGeneticAlgorithm();
	vector<int> getBestResult();
	void generateRandomPopulation();
	void runIteration();
	void initialize();

private:
	int populationSize;
	double crossoverProb;
	double mutationProb;
	vector<int> bestResult;
	double bestFitness;
	void calculateBestResult();
	vector<CIndividual*>* population;
	mt19937 &randEngine;
	CEvaluator &evaluator;
	int expectedMutations;
	void mutateNewIndividuals();

	void calculatePopulationFitness();
	void generateNewPopulation();
	void deletePopulation();
	void sortPopulation();
	//required to sort pointers
	static bool compareCIndividualPointers(CIndividual* first, CIndividual*  second);
};

