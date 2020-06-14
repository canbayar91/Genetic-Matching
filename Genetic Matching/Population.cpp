#include "Population.h"
#include <iostream>
#include <random>
#include <ctime>
#include <queue>
#include <set>

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

void Population::crossover(unsigned int index1, unsigned int index2) {

	// Clone the parents to create new children
	Individual child1 = cloneIndividual(index1);
	Individual child2 = cloneIndividual(index2);

	// Get the chromosomes
	Chromosome chromosome1 = child1.getChromosome();
	Chromosome chromosome2 = child2.getChromosome();

	// Pick a random crossover point
	FaceList faceList = mesh->getFaces();
	size_t faceCount = mesh->getFaceCount();
	unsigned int rootId = (unsigned int) rand() % faceCount;

	// Create a queue for breadth-first search and a set to keep track of processed elements
	std::queue<std::pair<unsigned int, int>> faceQueue;
	std::set<unsigned int> processedFaces;

	// Add the root face into the queue
	auto rootPair = std::make_pair(rootId, 0);
	faceQueue.push(rootPair);

	// The operation will only cover the faces on the pre-defined depth from the root node
	while (!faceQueue.empty()) {

		// Get the face at the front of the queue
		auto current = faceQueue.front();
		unsigned int currentId = current.first;
		int currentDepth = current.second;

		// Swap the root node and remove it from the queue
		swapGenes(chromosome1, chromosome2, rootId);
		faceQueue.pop();

		// Operations stops when the local area limit is reached
		if (currentDepth < LOCAL_AREA_DEPTH) {

			// Run through the half-edges (there are exacly 3 neighbors for each mesh)
			FaceData* face = faceList[currentId];
			HalfEdgeData* runner = face->half;
			do {

				// Get the neighbor data
				FaceData* neighbor = runner->pair->face;
				unsigned int neighborId = neighbor->id;

				// Skip the already processed faces
				if (processedFaces.count(neighborId) == 0) {

					// Push the neighbor into the queue
					auto pair = std::make_pair(neighborId, currentDepth + 1);
					faceQueue.push(pair);
				}

				// Update the runner pointer
				runner = runner->next;

			} while (runner != face->half);
		}

		// Keep track of the processed faces, since they can be incident with more than one faces
		processedFaces.insert(currentId);
	}

	// Store the first new child
	child1.setId(populationCounter);
	storeIndividual(child1);

	// Store the second new child
	child2.setId(populationCounter);
	storeIndividual(child2);
}

void Population::swapGenes(Chromosome &chromosome1, Chromosome &chromosome2, unsigned int index) {

	// Get the genes on index
	Gene &gene1 = chromosome1[index];
	Gene &gene2 = chromosome2[index];

	// Don't waste time if matching is already the same
	if (gene1.getNeighborId() != gene2.getNeighborId()) {

		// Temporary swap variables
		bool matchFlag = gene1.isMatched();
		unsigned int neighborId = gene1.getNeighborId();
		double fitness = gene1.getFitness();

		// Update first gene
		gene1.setMatched(gene2.isMatched());
		gene1.setNeighborId(gene2.getNeighborId());
		gene1.setFitness(gene2.getFitness());

		// Update second gene
		gene2.setMatched(matchFlag);
		gene2.setNeighborId(neighborId);
		gene2.setFitness(fitness);
	}
}

void Population::mutation(unsigned int index) {

	// Clone the parent to create a new child
	Individual child = cloneIndividual(index);

	// Pick a random mutation point
	size_t faceCount = mesh->getFaceCount();
	int rootId = rand() % faceCount;

	// Store the child
	child.setId(populationCounter);
	// storeIndividual(child);
}

void Population::selection() {

	// Remove the individuals with the lowest fitness score
	while (individualMapping.size() > size) {

		// Find the individual with the worst score
		unsigned int worstId = 0;
		double worstFitnessScore = DBL_MAX;
		for (auto it = individualMapping.begin(); it != individualMapping.end(); it++) {
			double current = it->second.getTotalFitness();
			if (current <= worstFitnessScore || abs(current - worstFitnessScore) <= EPSILON) {
				worstFitnessScore = current;
				worstId = it->first;
			}
		}

		// Erase its reference from map
		individualMapping.erase(worstId);
	}
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

Individual Population::cloneIndividual(unsigned int index) {

	// Iterate until the index
	auto it = individualMapping.begin();
	for (unsigned int i = 0; i < index; i++) {
		it++;
	}

	// Get the individual
	Individual parent = it->second;

	// Create a new chromosome by copying the parent
	return Individual(parent);
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
