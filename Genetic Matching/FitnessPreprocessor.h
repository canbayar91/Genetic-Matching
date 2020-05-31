#ifndef FITNESS_PREPROCESSOR
#define FITNESS_PREPROCESSOR

#include "TriangularMesh.h"

typedef std::pair<unsigned int, double> ScoreData;

class FitnessPreprocessor {
public:

	// Returns the singleton instance
	static FitnessPreprocessor* getInstance();

	// Calculates all possible matching scores (3 x faceCount / 2) prior to start matching
	void precalculateMatchingScores(FaceList &faceList);

	// Returns the pre-calculated fitness score of given face pair
	double getFitness(unsigned int faceId, unsigned int neighborId);

private:

	// Singleton instance
	static FitnessPreprocessor* instance;

	// Stores the pre-calculated fitness function results for each possible matching (3 for each face)
	std::vector<std::vector<ScoreData>> fitnessMap;

	// Default constuctor
	FitnessPreprocessor();

	// Destructor
	~FitnessPreprocessor();

	// Forms a quadrilateral from the given face pair and returns Scaled Jacobian result of the pair 
	double calculateScaledJacobian(FaceData* face, FaceData* pair, HalfEdgeData* runner);

	// Stores the fitness score of the given face pair
	void storeFitness(unsigned int faceId, unsigned int neighborId, double score);

};

#endif
