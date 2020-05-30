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

	// Initialize the random seed
	srand(time(NULL));

	// Generate a random individual by random matches
	for (unsigned int i = 0; i < size; i++) {
		generateIndividual();
	}

	// outputPopulationStatistics();
}

void Population::generateIndividual() {

	// Create a vector that stores an index value for each corresponding face id
	size_t faceCount = mesh->getFaceCount();
	std::vector<size_t> indexVector(faceCount);
	for (size_t i = 0; i < faceCount; i++) {
		indexVector[i] = i;
	}

	// Apply Fisher-Yates shuffling algorithm to create a random order to consume the faces
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
		chromosome[faceId].setFaceId(faceId);
		if (!chromosome[faceId].isMatched()) {

			// Run through the half-edges (there are exacly 3 neighbors for each mesh)
			FaceData* face = faceList[faceId];
			HalfEdgeData* runner = face->half;
			do {

				// Match the face with the first unmatched neighbor
				FaceData* pair = runner->pair->face;
				unsigned int neighborId = pair->id;
				if (!chromosome[neighborId].isMatched()) {
					chromosome[neighborId].setFaceId(neighborId);
					chromosome[faceId].match(neighborId);
					chromosome[neighborId].match(faceId);
					break;
				}

				// Update the runner pointer
				runner = runner->next;

			} while (runner != face->half);
		}
	}

	// Add the new individual into the population
	Individual individual(populationCounter, chromosome);
	population.push(individual);

	// Add reference to the map
	individualMapping[populationCounter] = individual;

	// Increment the counter
	populationCounter++;
}

void Population::crossover() {

	// Pick a random crossover point
	size_t faceCount = mesh->getFaceCount();
	int randomIndex = rand() % faceCount;

	// Increment the counter
	populationCounter++;
}

void Population::mutation() {

	// Pick a random mutation point
	size_t faceCount = mesh->getFaceCount();
	int randomIndex = rand() % faceCount;

	// Increment the counter
	populationCounter++;
}

void Population::selection() {

	// Remove the individuals with the lowest fitness score
	while (population.size() > size) {

		// Remove the individual from queue
		Individual individual = population.top();
		population.pop();

		// Erase its reference from map
		unsigned int individualId = individual.getId();
		individualMapping.erase(individualId);
	}
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
		std::cout << "Fitness Score: " << individual.getFitness() << std::endl;
	}
}
