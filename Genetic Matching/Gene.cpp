#include "Gene.h"
#include "EvaluationFunctions.h"

void Gene::setFaceId(unsigned int faceId) {
	this->faceId = faceId;
}

void Gene::match(unsigned int neighborId) {
	matchFlag = true;
	matchId = neighborId;
	fitness = (double) rand() / RAND_MAX;
	// EvaluationFunctions::calculateMinimumScaledJacobian();
}

bool Gene::isMatched() {
	return matchFlag;
}

double Gene::getFitness() {
	return fitness;
}
