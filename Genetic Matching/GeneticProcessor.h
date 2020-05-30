#ifndef GENETIC_PROCESSOR
#define GENETIC_PROCESSOR

#include "Population.h"
#include <unordered_map>

#define POPULATION_SIZE 10
#define MAX_ITERATION_COUNT 50

class GeneticProcessor {
public:

	// Constuctor
	GeneticProcessor(const TriangularMesh* mesh);

	// Destructor
	~GeneticProcessor();

	// Genetically processes the input mesh
	void process();

private:

	// Initial triangular mesh instance
	const TriangularMesh* mesh;

	// Each individual in the population represents a different matching
	Population* population;

	// Generates the initial population of size n
	void generateInitialPopulation();

	// On each generation, population size is doubled and fittest n individuals are selected for the next generation
	void nextGeneration();

};

#endif
