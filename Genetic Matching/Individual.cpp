#include "Individual.h"

Individual::Individual() {
	this->id = 0;
}

Individual::Individual(unsigned int id, Chromosome chromosome) {
	this->id = id;
	this->chromosome = chromosome;
	calculateFitness();
	countMatchings();
}

void Individual::calculateFitness() {

	// Fitness of a chromosome is basically the total fitness of all genes
	// Both fitness of a single gene and number of matched genes increases the total fitness
	for (Gene gene : chromosome) {
		fitness += gene.getFitness();
	}
}

void Individual::countMatchings() {

	// Match count is incremented for each matched gene
	for (Gene gene : chromosome) {
		if (gene.isMatched()) {
			matchCount++;
		}
	}
}

unsigned int Individual::getId() {
	return id;
}

Chromosome &Individual::getChromosome() {
	return chromosome;
}

double Individual::getTotalFitness() {
	return fitness;
}

double Individual::getAverageFitness() {
	return fitness / chromosome.size();
}

unsigned int Individual::getMatchCount() {
	return matchCount;
}
