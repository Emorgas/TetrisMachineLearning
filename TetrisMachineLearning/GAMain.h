#pragma once

#include <ctime>
#include <vector>
#include <iostream>	
#include "Definitions.h"

struct Chromosome
{
	float alleles[5]; // rowsCleared, closedHoles, boardMaxHeight, surfaceRoughness, altitudeDelta
	int fitness; //The average game score
};

class GAMain
{
private:
	int _generation;
	std::vector<Chromosome*> _population;
public:
	GAMain();
	~GAMain();
	void InitialisePopulation();
	void BeginNewGeneration();
	void GenerateChildren();
	Chromosome* UniformCrossover(Chromosome* p1, Chromosome* p2);
	Chromosome* RandomCrossover(Chromosome* p1, Chromosome* p2);
	Chromosome* SinglePointCrossover(Chromosome* p1, Chromosome* p2);
	Chromosome* LinearRankSelection();
	void SortPopulationByFitness();

	Chromosome* GetChromosome(int index) { return _population.at(index); }
	void SetChromosomeFitness(int index, int value) { _population.at(index)->fitness = value; }
};

