#ifndef DEFINITIONS
#define DEFINITIONS

#include "Face.h"
#include <map>

typedef std::pair<unsigned int, unsigned int> VertexPair;

struct HalfEdgeData;

struct VertexData {
	unsigned int id;
	Vertex coordinate;
	HalfEdgeData* half;
};

struct FaceData {
	bool matched = false;
	bool processed = false;
	unsigned int id;
	double priority = 0;
	HalfEdgeData* half;
};

struct HalfEdgeData {
	bool matched = false;
	VertexData* origin;
	HalfEdgeData* pair;
	HalfEdgeData* next;
	FaceData* face;
};

struct MaxPriorityOrder {
	bool operator()(FaceData* &lhs, FaceData* &rhs) {
		return lhs->priority < rhs->priority;
	}
};

#endif
