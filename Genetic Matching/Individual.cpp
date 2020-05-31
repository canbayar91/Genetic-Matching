#include "Individual.h"

Individual::Individual() {
	this->id = 0;
}

Individual::Individual(unsigned int id, Chromosome &chromosome) {
	this->id = id;
	this->chromosome = chromosome;
	calculateFitness();
	countMatchings();
}

Individual::Individual(const Individual &original) {
	this->id = original.getId();
	this->fitness = original.getTotalFitness();
	this->matchCount = original.getMatchCount();
	Chromosome chromosome = original.getChromosome();
	copy(chromosome.begin(), chromosome.end(), back_inserter(this->chromosome));
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

void Individual::setId(unsigned int id) {
	this->id = id;
}

unsigned int Individual::getId() const {
	return id;
}

const Chromosome &Individual::getChromosome() const {
	return chromosome;
}

double Individual::getTotalFitness() const {
	return fitness;
}

double Individual::getAverageFitness() const {
	return fitness / chromosome.size();
}

unsigned int Individual::getMatchCount() const {
	return matchCount;
}

double Individual::getMatchingPercentage() const {
	return (double) matchCount / (double) chromosome.size() * 100;
}
