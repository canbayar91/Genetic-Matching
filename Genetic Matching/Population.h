#ifndef POPULATION
#define POPULATION

#include "Individual.h"
#include "TriangularMesh.h"
#include <unordered_map>
#include <queue>

class Population {
public:

	// Initializes the population to given size
	Population(unsigned int size, const TriangularMesh* mesh);

	// Exchanges genes between randomly selected individuals in order to introduce a new individual
	void crossover();

	// Mutates a random individual in order to introduce a new individual 
	void mutation();

	// Selects the individuals with highest fitness score and removes others until population size reaches initial size
	void selection();

	// Debug function for some statistics for the individuals in the population
	void outputPopulationStatistics();

private:

	// Size of the population is the same at the end of each generation
	unsigned int size;

	// Input triangular mesh instance
	const TriangularMesh* mesh;

	// Individuals in the population, sorted by fitness score in descending order
	std::priority_queue<Individual, std::vector<Individual>, FitnessOrder> population;

	// Used as a fast reference for individuals in population
	std::unordered_map<unsigned int, Individual> individualMapping;

	// Each time a new individual is created, this counter will be incremented
	unsigned int populationCounter = 0;

	// Generates random individuals until it reaches the initial size
	void initialize();

	// Generates a new random individual (matching) for the population
	void generateIndividual();

};

#endif
