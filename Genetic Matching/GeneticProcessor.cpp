#include "GeneticProcessor.h"
#include <iostream>
#include <ctime>

GeneticProcessor* GeneticProcessor::instance;

GeneticProcessor::GeneticProcessor() {
	instance = new GeneticProcessor();
}

GeneticProcessor::~GeneticProcessor() {
	delete instance;
	instance = 0;
}

GeneticProcessor* GeneticProcessor::getInstance() {
	return instance;
}

void GeneticProcessor::initialize(const TriangularMesh* mesh) {
	this->mesh = mesh;
}

void GeneticProcessor::process() {
	
	// Generate the initial population of size n
	generateInitialPopulation();

	// Output the population size
	std::cout << "Population size: " << POPULATION_SIZE << std::endl;

	// Genetically processes the population until a constraint is satisfied
	unsigned int currentIteration = 0;
	while (currentIteration < MAX_ITERATION_COUNT) {

		// Applies crossovers between randomly selected individuals in order to generate new individuals
		crossover();

		// Applies mutations on randomly selected individuals in order to generate new individuals
		mutate();

		// Calculates the fitness score of each individual
		calculateFitness();

		// Select the fittest individuals for the next generation
		selectFittest();
	}

	// Output the iteration count of the algorithm
	std::cout << "Iteration Count: " << currentIteration << std::endl;
}

void GeneticProcessor::generateInitialPopulation() {

}

void GeneticProcessor::selectFittest() {

}

void GeneticProcessor::crossover() {

}

void GeneticProcessor::mutate() {

}

void GeneticProcessor::calculateFitness() {

}
