#ifndef GENE
#define GENE

#include "FitnessPreprocessor.h"

class Gene {
public:

	// Matches the gene with the given neighbor
	void match(unsigned int neighborId);

	// Clears the matching
	void unmatch();

	// Sets the matched flag
	void setMatched(bool matched);

	// Sets the face id
	void setFaceId(unsigned int faceId);

	// Sets the neighbor id
	void setNeighborId(unsigned int neighborId);

	// Sets the fitness
	void setFitness(double fitness);

	// Returns the match flag
	bool isMatched();

	// Returns the face id
	unsigned int getFaceId();

	// Returns the face id of its match
	unsigned int getNeighborId();

	// Returns the fitness score 
	double getFitness();


private:

	// Indicates whether the current gene stores a matched or unmatched face
	// Genes with unmatched faces will not be contributing to the fitness score of the chromosome
	bool matchFlag = false;

	// Id (or index) of the face that we are trying to match
	unsigned int faceId;

	// If the match flag is set, this id represents the face which the face with faceId is matched with
	unsigned int neighborId;

	// Contribution of the gene to the fitness score - 0 if matchFlag is false (Scaled Jacobian)
	double fitness = 0;

	// Fitness preprocessor stores the pre-calculated fitness scores
	static FitnessPreprocessor* preprocessor;

};

#endif
