#include "GAMain.h"



GAMain::GAMain()
{
}


GAMain::~GAMain()
{
}

void GAMain::InitialisePopulation()
{
	Chromosome* temp;
	for (int i = 0; i < GA_POPSIZE; i++)
	{
		temp = new Chromosome();
		temp->alleles[0] = ((float)rand() / RAND_MAX * 100.0f - 50.0f);
		temp->alleles[1] = ((float)rand() / RAND_MAX * 100.0f - 50.0f);
		temp->alleles[2] = ((float)rand() / RAND_MAX * 100.0f - 50.0f);
		temp->alleles[3] = ((float)rand() / RAND_MAX * 100.0f - 50.0f);
		temp->fitness = 0.0f;
		_population.emplace_back(temp);
	}
}

void GAMain::BeginNewGeneration()
{

	SortPopulationByFitness();

	GenerateChildren();
}

void GAMain::GenerateChildren()
{
	std::vector<Chromosome*> newPop;

	while (newPop.size() < GA_POPSIZE)
	{
		Chromosome* parent1 = LinearRankSelection();
		Chromosome* parent2 = LinearRankSelection();

		newPop.emplace_back(SinglePointCrossover(parent1, parent2));
	}

	for each (Chromosome* c in _population)
	{
		delete c;
	}
	_population.clear();
	_population = newPop;
}

Chromosome* GAMain::SinglePointCrossover(Chromosome* p1, Chromosome* p2)
{
	Chromosome* temp = new Chromosome();
	int crossoverPoint = (int)rand() / RAND_MAX * 2;

	if (crossoverPoint == 0)
	{
		temp->alleles[0] = p1->alleles[0];
		temp->alleles[1] = p2->alleles[1];
		temp->alleles[2] = p2->alleles[2];
		temp->alleles[3] = p2->alleles[3];
	}
	else if (crossoverPoint == 1)
	{
		temp->alleles[0] = p1->alleles[0];
		temp->alleles[1] = p1->alleles[1];
		temp->alleles[2] = p2->alleles[2];
		temp->alleles[3] = p2->alleles[3];
	}
	else if (crossoverPoint == 2)
	{
		temp->alleles[0] = p1->alleles[0];
		temp->alleles[1] = p1->alleles[1];
		temp->alleles[2] = p1->alleles[2];
		temp->alleles[3] = p2->alleles[3];
	}

	temp->fitness = 0;

	int mutationChance = rand() % (int)(1.0 / GA_MUTATION);
	if (mutationChance == 0)
	{
		int alleleToMutate = rand() % 4;
		temp->alleles[alleleToMutate] = ((float)rand() / RAND_MAX * 100.0f - 50.0f);
	}
	return temp;
}

Chromosome* GAMain::LinearRankSelection()
{
	int totalFitness = 0;
	for (int i = 1; i == GA_POPSIZE; i++)
	{
		totalFitness += i;
	}

	return _population.at((int)rand() / RAND_MAX * totalFitness);
}

void GAMain::SortPopulationByFitness()
{
	Chromosome* temp;
	for (int i = 0; i < GA_POPSIZE; i++)
	{
		for (int z = 1; z < GA_POPSIZE - 1; z++)
		{
			if (_population.at(i)->fitness > _population.at(z)->fitness)
			{
				temp = _population.at(i);
				_population.at(i) = _population.at(z);
				_population.at(z) = temp;
			}
		}
	}
}