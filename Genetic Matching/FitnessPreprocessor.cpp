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

void FitnessPreprocessor::precalculateMatchingScores(FaceList &faceList) {

	// Start time of calculation
	const clock_t beginTime = clock();

	// Allocate the vector size at the beginning
	size_t size = faceList.size();
	fitnessMap.resize(size);

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
			if (getFitness(faceId, neighborId) == -1) {
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

	// Store the score with the corresponding neighbor
	auto pair = std::make_pair(neighborId, score);
	fitnessMap[faceId].push_back(pair);

	// Same operation in reverse order
	auto pair2 = std::make_pair(faceId, score);
	fitnessMap[neighborId].push_back(pair2);
}

double FitnessPreprocessor::getFitness(unsigned int faceId, unsigned int neighborId) {

	// Find the score with the corresponding neighbor
	for (auto pair : fitnessMap[faceId]) {
		if (pair.first == neighborId) {
			return pair.second;
		}
	}

	// Error case, expected to never occur
	return -1;
}
