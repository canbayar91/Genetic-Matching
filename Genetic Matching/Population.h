#ifndef POPULATION
#define POPULATION

#include "Individual.h"
#include "TriangularMesh.h"
#include <queue>
#include <map>

#define LOCAL_AREA_DEPTH 3

class Population {
public:

	// Initializes the population to given size
	Population(unsigned int size, const TriangularMesh* mesh);

	// Exchanges genes between randomly selected individuals in order to introduce a new individual
	void crossover(unsigned int indexSmall, unsigned int indexBig);

	// Mutates a random individual in order to introduce a new individual 
	void mutation(unsigned int index);

	// Selects the individuals with highest fitness score and removes others until population size reaches initial size
	void selection();

	// Returns the individual with the best fitness score
	Individual &getFittestIndividual();

	// Returns the individual with the biggest amount of matched genes
	Individual &getMostMatchedIndividual();

	// Debug function for some statistics for the individuals in the population
	void outputPopulationStatistics();

private:

	// Size of the population is the same at the end of each generation
	unsigned int size;

	// Input triangular mesh instance
	const TriangularMesh* mesh;

	// Used as a fast reference for individuals in population
	std::map<unsigned int, Individual> individualMapping;

	// Each time a new individual is created, this counter will be incremented
	unsigned int populationCounter = 0;

	// The identifier of the fittest individual
	unsigned int fittestIndividualId = 0;

	// The identifier of the individual with the highest amount of matching
	unsigned int mostMatchedIndividualId = 0;

	// The highest amount of average fitness in the population
	double highestAverageFitness = 0;

	// The highest amount of matched face count in the population
	unsigned int highestMatchCount = 0;

	// Generates random individuals until it reaches the initial size
	void initialize();

	// Generates a new random individual (matching) for the population
	void generateIndividual(std::vector<size_t> &indexVector);

	// Stores the given individual 
	void storeIndividual(Individual &individual);

};

#endif
