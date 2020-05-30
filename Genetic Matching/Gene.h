#ifndef GENE
#define GENE

class Gene {
public:

	// Sets the face id
	void setFaceId(unsigned int faceId);

	// Matches the gene with the given neighbor
	void match(unsigned int neighborId);

	// Returns the match flag
	bool isMatched();

	// Returns the fitness score 
	double getFitness();

private:

	// Indicates whether the current gene stores a matched or unmatched face
	// Genes with unmatched faces will not be contributing to the fitness score of the chromosome
	bool matchFlag = false;

	// Id (or index) of the face that we are trying to match
	unsigned int faceId;

	// If the match flag is set, this id represents the face which the face with faceId is matched with
	unsigned int matchId;

	// Contribution of the gene to the fitness score - 0 if matchFlag is false (Scaled Jacobian)
	double fitness = 0;

};

#endif
