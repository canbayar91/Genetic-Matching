#ifndef GENETIC_PROCESSOR
#define GENETIC_PROCESSOR

#include "Population.h"
#include <unordered_map>

#define POPULATION_SIZE 10
#define MAX_ITERATION_COUNT 50
#define FITNESS_OBJECTIVE 0.75
#define MATCH_PERCENTAGE_OBJECTIVE 98

class GeneticProcessor {
public:

	// Constuctor
	GeneticProcessor(const TriangularMesh* mesh);

	// Destructor
	~GeneticProcessor();

	// Genetically processes the input mesh
	Individual &process();

private:

	// Initial triangular mesh instance
	const TriangularMesh* mesh;

	// Each individual in the population represents a different matching
	Population* population;

	// Number of generations
	unsigned int generationCounter = 0;

	// Generates the initial population of size n
	void generateInitialPopulation();

	// On each generation, population size is doubled and fittest n individuals are selected for the next generation
	void nextGeneration();

	// Checks whether or not any of the pre-defined constraints satistied 
	bool stopConditionSatisfied();

};

#endif
