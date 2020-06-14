#include "Gene.h"

FitnessPreprocessor* Gene::preprocessor = FitnessPreprocessor::getInstance();

void Gene::match(unsigned int neighborId) {
	this->matchFlag = true;
	this->neighborId = neighborId;
	fitness = preprocessor->getFitness(faceId, neighborId);
}

void Gene::unmatch() {
	matchFlag = false;
	fitness = 0;
}

void Gene::setMatched(bool matched) {
	this->matchFlag = matched;
}

void Gene::setFaceId(unsigned int faceId) {
	this->faceId = faceId;
}

void Gene::setNeighborId(unsigned int neighborId) {
	this->neighborId = neighborId;
}

void Gene::setFitness(double fitness) {
	this->fitness = fitness;
}

bool Gene::isMatched() {
	return matchFlag;
}

double Gene::getFitness() {
	return fitness;
}

unsigned int Gene::getFaceId() {
	return faceId;
}

unsigned int Gene::getNeighborId() {
	return neighborId;
}
