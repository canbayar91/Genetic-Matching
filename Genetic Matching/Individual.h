#ifndef INDIVIDUAL
#define INDIVIDUAL

#include "Gene.h"
#include <vector>

// There is exactly one gene for each face of the mesh in a chromosome
// Each gene stores the information whether the face is matched or not, and if so with which face it is matched
// Since a match requires two faces, there are two genes for each matching that stores the same information
typedef std::vector<Gene> Chromosome;

class Individual {
public:

	// Default constructor (don't use)
	Individual();

	// Constructor
	Individual(unsigned int id, Chromosome chromosome);

	// Get the identifier
	unsigned int getId();

	// Get the chromosome
	Chromosome &getChromosome();

	// Get the fitness score
	double getFitness();

private:

	// Unique identifier
	unsigned int id;

	// Chromosome of the individual
	Chromosome chromosome;

	// Fitness score (Scaled Jacobian)
	double fitness = 0;

	// Calculates the fitness of the individual
	void calculateFitness();

};

// Callback used in sorting individuals according to fitness score in descending order
struct FitnessOrder {
	bool operator()(Individual &lhs, Individual &rhs) {
		return lhs.getFitness() > rhs.getFitness();
	}
};

#endif
