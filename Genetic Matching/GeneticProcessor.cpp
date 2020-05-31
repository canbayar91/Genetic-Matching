#include "GeneticProcessor.h"
#include <iostream>
#include <ctime>

GeneticProcessor::GeneticProcessor(const TriangularMesh* mesh) {
	this->mesh = mesh;
}

GeneticProcessor::~GeneticProcessor() {
	delete population;
	population = 0;
}

void GeneticProcessor::process() {

	// Calculate all possible matching scores for triangles
	FaceList faceList = mesh->getFaces();
	FitnessPreprocessor::getInstance()->precalculateMatchingScores(faceList);
	
	// Generate the initial population of size n
	generateInitialPopulation();

	// Start time of generation
	const clock_t beginTime = clock();

	// Genetically processes the population until a constraint is satisfied
	while (!stopConditionSatisfied()) {

		// Each iteration consists of crossover, mutation and selection phases
		nextGeneration();

		// Increment the iteration counter
		generationCounter++;
	}

	// End time of generation
	const clock_t endTime = clock();

	// Calculate the average time
	float timeDifference = float(endTime - beginTime);
	float averageTime = timeDifference / generationCounter;

	// Output the iteration count and average generation time of the algorithm
	std::cout << "Generation Count: " << generationCounter << std::endl;
	std::cout << "Average Generation Time: " << averageTime / CLOCKS_PER_SEC << " seconds" << std::endl;
}

void GeneticProcessor::generateInitialPopulation() {

	// Output the population size
	std::cout << "Population Size: " << POPULATION_SIZE << std::endl;

	// Start time of generation
	const clock_t beginTime = clock();

	// Initialize the population
	population = new Population(POPULATION_SIZE, mesh);

	// End time of generation
	const clock_t endTime = clock();

	// Output the time difference
	float timeDifference = float(endTime - beginTime);
	std::cout << "Initial Population Creation Time: " << timeDifference / CLOCKS_PER_SEC << " seconds" << std::endl;
}

void GeneticProcessor::nextGeneration() {

	// Each generation doubles the size of the population by a factor of 2
	for (unsigned int i = 0; i < POPULATION_SIZE / 2; i++) {
		population->crossover();
		population->mutation();
	}
	
	// Half of the individuals eliminated at the end of the generation
	population->selection();
}

bool GeneticProcessor::stopConditionSatisfied() {

	// Best individuals
	Individual fittestIndividual = population->getFittestIndividual();
	Individual mostMatchedIndividual = population->getMostMatchedIndividual();

	// The defined conditions
	bool fitnessConditionSatisfied = fittestIndividual.getAverageFitness() >= FITNESS_OBJECTIVE;
	bool matchingConditionSatisfied = mostMatchedIndividual.getMatchCount() >= MATCH_PERCENTAGE_OBJECTIVE;
	bool generationConditionSatisfied = generationCounter >= MAX_ITERATION_COUNT;

	// Satisfying a single condition is enough to stop iterating
	return fitnessConditionSatisfied || matchingConditionSatisfied || generationConditionSatisfied;
}
