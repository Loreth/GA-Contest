#pragma once
#include <utility>
#include <random>
using namespace std;

class CIndividual
{
public:
	CIndividual(int genotypeLength, mt19937 &randEngine, bool generateRandom);
	CIndividual(const CIndividual& other);
	~CIndividual();
	pair<CIndividual*, CIndividual*> crossover(CIndividual* other);
	vector<int>* getGenotype();
	double getFitness();
	void setFitness(double fitness);
	bool getFitnessAlreadyCalculated();
	void setFitnessAlreadyCalculated(bool alreadyCalculated);
	void generateRandomGenotype();

private:
	vector<int>* genotype;
	int genotypeLength;
	double fitness;
	bool fitnessAlreadyCalculated;
	mt19937 &randEngine;
};