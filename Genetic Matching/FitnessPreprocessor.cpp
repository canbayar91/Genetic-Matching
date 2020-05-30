#include "FitnessPreprocessor.h"
#include "EvaluationFunctions.h"
#include <iostream>
#include <ctime>

FitnessPreprocessor* FitnessPreprocessor::instance = new FitnessPreprocessor();

FitnessPreprocessor::FitnessPreprocessor() {
	// Empty Constructor
}

FitnessPreprocessor::~FitnessPreprocessor() {
	delete instance;
	instance = 0;
}

FitnessPreprocessor* FitnessPreprocessor::getInstance() {
	return instance;
}

void FitnessPreprocessor::precalculateMatchingScores(FaceList faceList) {

	// Start time of calculation
	const clock_t beginTime = clock();

	// Iterate through each face and store pre-calculated scores for possible matchings
	for (FaceData* face : faceList) {

		// Run through the half-edges (there are exacly 3 neighbors for each mesh)
		unsigned int faceId = face->id;
		HalfEdgeData* runner = face->half;
		do {

			// Get the neighbor corresponding to the runner
			FaceData* pair = runner->pair->face;
			unsigned int neighborId = pair->id;

			// Calculate the fitness score by forming a rectangle with two faces
			if (fitnessMap.count(faceId) == 0 || fitnessMap[faceId].count(neighborId) == 0) {
				double fitness = calculateScaledJacobian(face, pair, runner);
				storeFitness(faceId, neighborId, fitness);
			}

			// Update the runner pointer
			runner = runner->next;

		} while (runner != face->half);
	}

	// End time of calculation
	const clock_t endTime = clock();

	// Output the total calculation time
	float timeDifference = float(endTime - beginTime);
	std::cout << "Fitness Pre-calculation: " << timeDifference / CLOCKS_PER_SEC << " seconds" << std::endl;
}

double FitnessPreprocessor::calculateScaledJacobian(FaceData* face, FaceData* pair, HalfEdgeData* runner) {

	// Find the vertices of the matching
	const Vertex a = runner->origin->coordinate;
	const Vertex b = runner->pair->next->next->origin->coordinate;
	const Vertex c = runner->pair->origin->coordinate;
	const Vertex d = runner->next->next->origin->coordinate;

	// Construct the quadrilateral
	const Quadrilateral* quadrilateral = new Quadrilateral(a, b, c, d);

	// Return the minimum scaled jacobian value
	return EvaluationFunctions::calculateMinimumScaledJacobian(quadrilateral);
}

void FitnessPreprocessor::storeFitness(unsigned int faceId, unsigned int neighborId, double score) {

	// If the current face does not exist in the map, add it
	if (fitnessMap.count(faceId) == 0) {
		std::unordered_map<unsigned int, double> neighborMap;
		fitnessMap[faceId] = neighborMap;
	}

	// Do the same operation for its neighbor
	if (fitnessMap.count(neighborId) == 0) {
		std::unordered_map<unsigned int, double> neighborMap;
		fitnessMap[neighborId] = neighborMap;
	}

	// Add the score with the neighbor
	fitnessMap[faceId][neighborId] = score;
	fitnessMap[neighborId][faceId] = score;
}

double FitnessPreprocessor::getFitness(unsigned int faceId, unsigned int neighborId) {
	return fitnessMap[faceId][neighborId];
}
