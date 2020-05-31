#include "Gene.h"

FitnessPreprocessor* Gene::preprocessor = FitnessPreprocessor::getInstance();

void Gene::setFaceId(unsigned int faceId) {
	this->faceId = faceId;
}

void Gene::match(unsigned int neighborId) {
	matchFlag = true;
	matchId = neighborId;
	fitness = preprocessor->getFitness(faceId, matchId);
}

void Gene::unmatch() {
	matchFlag = false;
	fitness = 0;
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

unsigned int Gene::getMatchId() {
	return matchId;
}
