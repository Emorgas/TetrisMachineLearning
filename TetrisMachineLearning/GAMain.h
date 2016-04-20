#pragma once

#include <ctime>
#include <vector>
#include <iostream>	
#include "Definitions.h"
#include <fstream>
#include <iostream>

struct Chromosome
{
	float alleles[5]; // rowsCleared, closedHoles, boardMaxHeight, surfaceRoughness, altitudeDelta
	unsigned long fitness; //The average game score N.B. Changed to use lines cleared instead
};

class GAMain
{
private:
	int _generation = 0;
	int _selectionMethod = 0;
	int _crossoverMethod = 0;
	std::vector<Chromosome*> _population;
	std::string _populationDataFilename;
public:
	GAMain(std::string filename, int crossover, int selection);
	~GAMain();
	void InitialisePopulation();
	void InitialisePopulationFromFile(std::string filename, int generation = -1);
	void BeginNewGeneration();
	void GenerateChildren();
	void OutputStatisticsToFile();
	void OutputGenerationToFile();
	Chromosome* RandomCrossover(Chromosome* p1, Chromosome* p2);
	Chromosome* SinglePointCrossover(Chromosome* p1, Chromosome* p2);
	Chromosome* TournamentSelection();
	Chromosome* LinearRankSelection();
	void SortPopulationByFitness();

	Chromosome* GetChromosome(int index) { return _population.at(index); }
	int GetGeneration() { return _generation; }
	void SetChromosomeFitness(int index, unsigned long value) { _population.at(index)->fitness = value; }
};

