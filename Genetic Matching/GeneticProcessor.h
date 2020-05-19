#ifndef GENETIC_PROCESSOR
#define GENETIC_PROCESSOR

#include "TriangularMesh.h"
#include "Chromosome.h"

#define POPULATION_SIZE 10
#define MAX_ITERATION_COUNT 50

class GeneticProcessor {
public:

	// Returns the singleton mesh reader instance
	static GeneticProcessor* getInstance();

	// Initializes the genetic processor with input mesh
	void initialize(const TriangularMesh* mesh);

	// Genetically processes the input mesh
	void process();

private:

	// Singleton genetic processor instance
	static GeneticProcessor* instance;

	// Initial triangular mesh instance
	const TriangularMesh* mesh;

	// Default constuctor
	GeneticProcessor();

	// Destructor
	~GeneticProcessor();

	// Generates the initial population of size n
	void generateInitialPopulation();

	// Selects the n fittest individuals for the next generation
	void selectFittest();

	// Exchanges genes between randomly selected chromosomes on local areas to create new individuals
	void crossover();

	// Changes genes on randomly selected chromosomes on local areas to create new individuals
	void mutate();

	// Calculates the fitness score (scaled Jacobian) of each individual
	void calculateFitness();

};

#endif
