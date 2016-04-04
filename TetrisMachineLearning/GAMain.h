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
	int fitness; //The average game score
};

class GAMain
{
private:
	int _generation = 0;
	std::vector<Chromosome*> _population;
	std::string _populationDataFilename;
public:
	GAMain(std::string filename);
	~GAMain();
	void InitialisePopulation();
	void InitialisePopulationFromFile(std::string filename, int generation = -1);
	void BeginNewGeneration();
	void GenerateChildren();
	void OutputStatisticsToFile();
	void OutputGenerationToFile();
	Chromosome* UniformCrossover(Chromosome* p1, Chromosome* p2);
	Chromosome* RandomCrossover(Chromosome* p1, Chromosome* p2);
	Chromosome* SinglePointCrossover(Chromosome* p1, Chromosome* p2);
	Chromosome* LinearRankSelection();
	void SortPopulationByFitness();

	Chromosome* GetChromosome(int index) { return _population.at(index); }
	int GetGeneration() { return _generation; }
	void SetChromosomeFitness(int index, int value) { _population.at(index)->fitness = value; }
};

