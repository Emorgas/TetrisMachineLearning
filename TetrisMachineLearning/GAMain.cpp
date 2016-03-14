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
		temp->alleles[0] = ((float)rand() - RAND_MAX / 2);
		temp->alleles[1] = ((float)rand() - RAND_MAX / 2);
		temp->alleles[2] = ((float)rand() - RAND_MAX / 2);
		temp->alleles[3] = ((float)rand() - RAND_MAX / 2);
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

	while (newPop.size() < GA_POPSIZE - 1)
	{
		Chromosome* parent1 = LinearRankSelection();
		Chromosome* parent2 = LinearRankSelection();

		newPop.emplace_back(UniformCrossover(parent1, parent2));
	}
	Chromosome* temp = new Chromosome();
	temp->alleles[0] = _population.at(9)->alleles[0];
	temp->alleles[1] = _population.at(9)->alleles[1];
	temp->alleles[2] = _population.at(9)->alleles[2];
	temp->alleles[3] = _population.at(9)->alleles[3];
	newPop.emplace_back(temp); //Implementing Elitism

	for each (Chromosome* c in _population)
	{
		delete c;
	}
	_population.clear();
	_population = newPop;
}

Chromosome* GAMain::UniformCrossover(Chromosome* p1, Chromosome* p2)
{
	Chromosome* child = new Chromosome();
	int alelleCrossover = rand() % 4;

	for (int i = 0; i < 4; i++)
		child->alleles[i] = p1->alleles[i];

	child->alleles[alelleCrossover] = p2->alleles[alelleCrossover];
	alelleCrossover = rand() % 4;
	child->alleles[alelleCrossover] = p2->alleles[alelleCrossover];
	child->fitness = 0;
	return child;
}

Chromosome* GAMain::SinglePointCrossover(Chromosome* p1, Chromosome* p2)
{
	Chromosome* temp = new Chromosome();
	float crossoverPoint = rand() % 3;

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
	for (int i = 1; i != GA_POPSIZE; i++)
	{
		totalFitness += i;
	}
	int chosenFitness = rand() %  totalFitness;
	int currentFitness = 0;
	int add = 0;
	while (currentFitness < chosenFitness && currentFitness < totalFitness && add < GA_POPSIZE - 1)
	{
		currentFitness += add;
		add++;
	}


	return _population.at(add);
}

void GAMain::SortPopulationByFitness()
{
	Chromosome* temp;
	for (int i = 0; i < GA_POPSIZE; i++)
	{
		for (int z = i + 1; z < GA_POPSIZE; z++)
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