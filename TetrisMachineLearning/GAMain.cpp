#include "GAMain.h"



GAMain::GAMain(std::string filename)
{
	_populationDataFilename = filename;
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
		for (int a = 0; a < GA_NUM_OF_ALLELES; a++)
		{
			temp->alleles[a] = (rand() % 200 - 100);
		}
		temp->fitness = 0.0f;
		_population.emplace_back(temp);
	}

	std::ofstream out(_populationDataFilename + ".txt");
	if (out.is_open() == false)
	{
		std::cout << "File creation Error: File could not be created!" << std::endl;
		return;
	}
	out << "G" << _generation << " " << GA_POPSIZE << std::endl;

	for (int i = 0; i < GA_POPSIZE; i++)
	{
		for (int a = 0; a < GA_NUM_OF_ALLELES; a++)
		{
			out << _population.at(i)->alleles[a] << " ";
		}
		out << std::endl;
	}
	out.close();

}

void GAMain::InitialisePopulationFromFile(std::string filename, int generation)
{
	filename.append(".txt");
	std::ifstream in(filename);
	if (in.is_open() == false)
	{
		std::cout << "File Read Error: File could not be opened!" << std::endl;
		return;
	}
	float totalGenerations = 0;
	char inChar;
	while (!in.eof())
	{
		in >> inChar;
		if (inChar == 'G')
		{
			totalGenerations++;
		}
	}

	in.close();
	in.open(filename);

	while (!in.eof())
	{
		in >> inChar;
		if (inChar == 'G')
		{
			int gen;
			in >> gen;
			if (gen == generation || gen == totalGenerations - 1)
			{
				int numChromosomes;
				in >> numChromosomes;
				for (int i = 0; i < numChromosomes; i++)
				{
					Chromosome* temp = new Chromosome();
					for (int a = 0; a < GA_NUM_OF_ALLELES; a++)
					{
						in >> temp->alleles[a];
					}
					temp->fitness = 0;
					_population.emplace_back(temp);
				}
				if (numChromosomes < GA_POPSIZE)
				{
					for (int i = numChromosomes; i < GA_POPSIZE; i++)
					{
						Chromosome* temp = new Chromosome();
						for (int a = 0; a < GA_NUM_OF_ALLELES; a++)
						{
							temp->alleles[a] = (rand() % 200 - 100);
						}
						temp->fitness = 0.0f;
						_population.emplace_back(temp);
					}
				}
				_generation = gen;
				in.close();
				return;
			}
		}
	}
}

void GAMain::BeginNewGeneration()
{

	SortPopulationByFitness();
	
	OutputStatisticsToFile();

	GenerateChildren();
	_generation++;
	OutputGenerationToFile();
}

void GAMain::OutputStatisticsToFile()
{
	std::ofstream out(_populationDataFilename + "Statistics.txt", std::ios::app);
	out << std::endl << "Generation: " << _generation << " Top Score was: " << _population.at(GA_POPSIZE - 1)->fitness << std::endl;
	out << "Generation: " << _generation << " Lowest Score was: " << _population.at(0)->fitness << std::endl;
	out.close();
}

void GAMain::GenerateChildren()
{
	std::vector<Chromosome*> newPop;

	while (newPop.size() < GA_POPSIZE - 1)
	{
		Chromosome* parent1 = LinearRankSelection();
		Chromosome* parent2 = LinearRankSelection();

		newPop.emplace_back(SinglePointCrossover(parent1, parent2));
	}
	Chromosome* temp = new Chromosome();
	for (int i = 0; i < GA_NUM_OF_ALLELES; i++)
	{
		temp->alleles[i] = _population.at(GA_POPSIZE - 1)->alleles[i];
	}
	newPop.emplace_back(temp); //Implementing Elitism

	for each (Chromosome* c in _population)
	{
		delete c;
	}
	_population.clear();
	_population = newPop;
}

void GAMain::OutputGenerationToFile()
{
	std::ofstream out(_populationDataFilename + ".txt", std::ios::app);

	out << std::endl << "G" << _generation << " " << GA_POPSIZE << std::endl;

	for (int i = 0; i < GA_POPSIZE; i++)
	{
		for (int a = 0; a < GA_NUM_OF_ALLELES; a++)
		{
			out << _population.at(i)->alleles[a] << " ";
		}
		out << std::endl;
	}
	out.close();
}

Chromosome* GAMain::UniformCrossover(Chromosome* p1, Chromosome* p2)
{
	Chromosome* child = new Chromosome();
	int alelleCrossover = rand() % GA_NUM_OF_ALLELES;

	for (int i = 0; i < GA_NUM_OF_ALLELES; i++)
		child->alleles[i] = p1->alleles[i];

	child->alleles[alelleCrossover] = p2->alleles[alelleCrossover];
	alelleCrossover = rand() % GA_NUM_OF_ALLELES;
	child->alleles[alelleCrossover] = p2->alleles[alelleCrossover];
	child->fitness = 0;
	return child;
}

Chromosome* GAMain::RandomCrossover(Chromosome* p1, Chromosome* p2)
{
	int alleleParent = 0;
	Chromosome* temp = new Chromosome();

	for (int i = 0; i < GA_NUM_OF_ALLELES; i++)
	{
		alleleParent = rand() % 2;
		if (alleleParent == 0)
			temp->alleles[i] = p1->alleles[i];
		else
			temp->alleles[i] = p2->alleles[i];
	}
	temp->fitness = 0;

	int mutationChance = rand() % (int)(1.0 / GA_MUTATION);
	if (mutationChance == 0)
	{
		std::cout << "Mutaton Occurred!" << std::endl;
		int alleleToMutate = rand() % 5;
		temp->alleles[alleleToMutate] = (rand() % 200 - 100);
	}

	return temp;
}

Chromosome* GAMain::SinglePointCrossover(Chromosome* p1, Chromosome* p2)
{
	Chromosome* temp = new Chromosome();
	int crossoverPoint = rand() % GA_NUM_OF_ALLELES;

	for (int i = 0; i < GA_NUM_OF_ALLELES; i++)
	{
		if (i < crossoverPoint)
		{
			temp->alleles[i] = p1->alleles[i];
		}
		else if (i >= crossoverPoint)
		{
			temp->alleles[i] = p2->alleles[i];
		}
	}
	temp->fitness = 0;

	int mutationChance = rand() % (int)(1.0 / GA_MUTATION);
	if (mutationChance == 0)
	{
		int alleleToMutate = rand() % 5;
		temp->alleles[alleleToMutate] = (rand() % 200 - 100);
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
	int chosenFitness = rand() % totalFitness;
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