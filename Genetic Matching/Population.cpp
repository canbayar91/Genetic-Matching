#include "Population.h"
#include <iostream>
#include <random>
#include <time.h>

Population::Population(unsigned int size, const TriangularMesh* mesh) {
	this->size = size;
	this->mesh = mesh;
	initialize();
}

void Population::initialize() {

	// Create an initial vector that stores an index value for each corresponding face id
	size_t faceCount = mesh->getFaceCount();
	std::vector<size_t> indexVector(faceCount);
	for (size_t i = 0; i < faceCount; i++) {
		indexVector[i] = i;
	}

	// Initialize the random seed
	srand(time(NULL));

	// Generate a random individual by random matches
	for (unsigned int i = 0; i < size; i++) {
		generateIndividual(indexVector);
	}

	// outputPopulationStatistics();
}

void Population::generateIndividual(std::vector<size_t> &indexVector) {

	// Apply Fisher-Yates shuffling algorithm to create a random order to consume the faces
	size_t faceCount = mesh->getFaceCount();
	for (size_t i = 0; i < faceCount; i++) {
		int randomIndex = rand() % faceCount;
		size_t temp = indexVector[i];
		indexVector[i] = indexVector[randomIndex];
		indexVector[randomIndex] = temp;
	}

	// Create a new individual to store a randomly-ordered greedy matching instance
	Chromosome chromosome(faceCount);

	// Run through each face in the created random order and try to match each face with first non-matched neighbor
	FaceList faceList = mesh->getFaces();
	for (size_t i = 0; i < faceCount; i++) {

		// Select a random face and if it is not matched yet, try to match
		int faceId = indexVector[i];
		Gene &gene = chromosome[faceId];
		gene.setFaceId(faceId);
		if (!gene.isMatched()) {

			// Run through the half-edges (there are exacly 3 neighbors for each mesh)
			FaceData* face = faceList[faceId];
			HalfEdgeData* runner = face->half;
			do {

				// Match the face with the first unmatched neighbor
				FaceData* pair = runner->pair->face;
				unsigned int neighborId = pair->id;
				Gene &neighbor = chromosome[neighborId];
				if (!neighbor.isMatched()) {
					neighbor.setFaceId(neighborId);
					gene.match(neighborId);
					neighbor.match(faceId);
					break;
				}

				// Update the runner pointer
				runner = runner->next;

			} while (runner != face->half);
		}
	}

	// Add the new individual into the population
	Individual individual(populationCounter, chromosome);
	storeIndividual(individual);
}

void Population::storeIndividual(Individual &individual) {

	// Add reference to the map
	individualMapping[populationCounter] = individual;

	// Increment the counter
	populationCounter++;

	// Get the individual id
	unsigned int individualId = individual.getId();

	// Update the fittest individual
	double averageFitness = individual.getAverageFitness();
	if (averageFitness > highestAverageFitness) {
		fittestIndividualId = individualId;
		highestAverageFitness = averageFitness;
	}

	// Update the most matched individual
	unsigned int matchCount = individual.getMatchCount();
	if (matchCount > highestMatchCount) {
		mostMatchedIndividualId = individualId;
		highestMatchCount = matchCount;
	}
}

void Population::crossover(unsigned int indexSmall, unsigned int indexBig) {

	// Iterate until the small index
	auto it = individualMapping.begin();
	for (unsigned int i = 0; i < indexSmall; i++) {
		it++;
	}

	// Get the first individual
	Individual parent1 = it->second;

	// Iterate until the big index
	for (unsigned int i = indexSmall; i < indexBig; i++) {
		it++;
	}

	// Get the second individual
	Individual parent2 = it->second;

	// Create a new chromosome by copying the parent
	Individual individual(parent1);
	individual.setId(populationCounter);

	// Pick a random crossover point
	size_t faceCount = mesh->getFaceCount();
	int rootId = rand() % faceCount;

	// Store the individual
	storeIndividual(individual);
}

void Population::mutation(unsigned int index) {

	// Iterate until the index
	auto it = individualMapping.begin();
	for (unsigned int i = 0; i < index; i++) {
		it++;
	}

	// Get the first individual
	Individual parent = it->second;

	// Create a new chromosome by copying the parent
	Individual individual(parent);
	individual.setId(populationCounter);

	// Pick a random mutation point
	size_t faceCount = mesh->getFaceCount();
	int rootId = rand() % faceCount;

	// Store the individual
	storeIndividual(individual);
}

void Population::selection() {

	// Remove the individuals with the lowest fitness score
	while (individualMapping.size() > size) {

		// Find the individual with the worst score
		unsigned int worstId = 0;
		double worstFitnessScore = DBL_MAX;
		for (auto it = individualMapping.begin(); it != individualMapping.end(); it++) {
			double current = it->second.getTotalFitness();
			if (current < worstFitnessScore) {
				worstFitnessScore = current;
				worstId = it->first;
			}
		}

		// Erase its reference from map
		individualMapping.erase(worstId);
	}
}

Individual &Population::getFittestIndividual() {

	// Returns the fittest individual
	return individualMapping[fittestIndividualId];
}

Individual &Population::getMostMatchedIndividual() {

	// Returns the individual with most matched genes
	return individualMapping[mostMatchedIndividualId];
}

void Population::outputPopulationStatistics() {

	// Iterate through chromosomes
	for (auto it = individualMapping.begin(); it != individualMapping.end(); it++) {

		// Get the chromosome
		Individual individual = it->second;
		Chromosome chromosome = individual.getChromosome();

		// Iterate through genes and increment if face is matched
		unsigned int count = 0;
		for (Gene gene : chromosome) {
			if (gene.isMatched()) {
				count++;
			}
		}

		// Output the total matched face count
		double percentage = (double) count / (double) mesh->getFaceCount() * 100;
		std::cout << "Matched Genes: " << count << " Percentage: " << percentage << std::endl;
		std::cout << "Fitness Score: " << individual.getAverageFitness() << std::endl;
		std::cout << "Matched Face Count: " << individual.getMatchCount() << std::endl;
	}
}
