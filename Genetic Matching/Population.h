#ifndef POPULATION
#define POPULATION

#include "Gene.h"
#include "TriangularMesh.h"

// There is exactly one gene for each face of the mesh in a chromosome
// Each gene stores the information whether the face is matched or not, and if so with which face it is matched
// Since a match requires two faces, there are two genes for each matching that stores the same information
typedef std::vector<Gene> Chromosome;

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

	// Debug function that outputs how many faces successfully matched in each individual
	void outputIndividualMatchings();

private:

	// Size of the population is the same at the end of each generation
	unsigned int size;

	// Input triangular mesh instance
	const TriangularMesh* mesh;

	// Individuals in the population
	std::vector<Chromosome> chromosomeList;

	// Generates random individuals until it reaches the initial size
	void initialize();

	// Generates a new random individual (matching) for the population
	void generateIndividual();

};

#endif
