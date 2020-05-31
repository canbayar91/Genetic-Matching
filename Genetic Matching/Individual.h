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
	Individual(unsigned int id, Chromosome &chromosome);

	// Copy constructor
	Individual(const Individual &original);

	// Set the identifier
	void setId(unsigned int id);

	// Get the identifier
	unsigned int getId() const;

	// Get the chromosome
	const Chromosome &getChromosome() const;

	// Get the total fitness score of all genes in the chromosome
	double getTotalFitness() const;

	// Get the average fitness score of a gene in the chromosome
	double getAverageFitness() const;

	// Get the matched gene count
	unsigned int getMatchCount() const;

	// Get the percentage of matched genes
	double getMatchingPercentage() const;

private:

	// Unique identifier
	unsigned int id;

	// Chromosome of the individual
	Chromosome chromosome;

	// Fitness score (Scaled Jacobian)
	double fitness = 0;

	// Total number of matched genes
	unsigned int matchCount = 0;

	// Calculates the fitness of the individual
	void calculateFitness();

	// Counts the number of genes that has the match flag set
	void countMatchings();

};

// Callback used in sorting individuals according to fitness score in descending order
struct FitnessOrder {
	bool operator()(Individual &lhs, Individual &rhs) {
		return lhs.getTotalFitness() > rhs.getTotalFitness();
	}
};

#endif
