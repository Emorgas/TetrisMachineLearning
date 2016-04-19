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
			temp->alleles[a] = (rand() % 2000 - 1000);
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
							temp->alleles[a] = (rand() % 2000 - 1000);
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
	std::ofstream out(_populationDataFilename + "Statistics_E.txt", std::ios::app);
	out << std::endl << "Generation: " << _generation << " Top Score was: " << _population.at(GA_POPSIZE - 1)->fitness << std::endl;
	out << "Generation: " << _generation << " Lowest Score was: " << _population.at(0)->fitness << std::endl;
	out.close();
}

void GAMain::GenerateChildren()
{
	std::vector<Chromosome*> newPop;

	while (newPop.size() < GA_POPSIZE)
	{
		Chromosome* parent1 = TournamentSelection();
		Chromosome* parent2 = TournamentSelection();

		newPop.emplace_back(SinglePointCrossover(parent1, parent2));
	}

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
	Chromosome* temp = new Chromosome();

	for (int i = 0; i < GA_NUM_OF_ALLELES; i++)
	{
		int parentToUse = (int)rand() % 2;
		if (parentToUse = 0)
			temp->alleles[i] = p1->alleles[i];
		else if (parentToUse = 1)
			temp->alleles[i] = p2->alleles[i];
		else
			std::cout << "Error in uniform crossover!!" << std::endl;
	}
	temp->fitness = 0;

	double mutationChance;
	for (int i = 0; i < GA_NUM_OF_ALLELES; i++)
	{
		mutationChance = ((double)rand() / (RAND_MAX));
		if (mutationChance <= GA_MUTATION)
		{
			temp->alleles[i] = (rand() % 2000 - 1000);
		}
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

	double mutationChance;
	for (int i = 0; i < GA_NUM_OF_ALLELES; i++)
	{
		mutationChance = ((double)rand() / (RAND_MAX));
		if (mutationChance <= GA_MUTATION)
		{
			temp->alleles[i] = (rand() % 200 - 100);
		}
	}

	return temp;
}

Chromosome* GAMain::TournamentSelection()
{
	std::vector<Chromosome*> tournamentMembers;
	int populationIndex;
	bool popMarkers[GA_POPSIZE] = { false };
	while (tournamentMembers.size() < GA_TOURNAMENT_SIZE)
	{
		populationIndex = (rand() % GA_POPSIZE);
		if (popMarkers[populationIndex] == false)
		{
			popMarkers[populationIndex] = true;
			tournamentMembers.emplace_back(_population.at(populationIndex));
		}
	}

	int bestMemberIndex = -1;
	int bestMemberFitness = -INT_MAX;
	int index = 0;
	for (int i = 0; i < GA_TOURNAMENT_SIZE; i++)
	{
		int fitness = tournamentMembers.at(i)->fitness;
		if (fitness > bestMemberFitness)
		{
			bestMemberFitness = fitness;
			bestMemberIndex = i;
		}
	}

	return tournamentMembers.at(bestMemberIndex);
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