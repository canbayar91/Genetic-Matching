#ifndef DEFINITIONS
#define DEFINITIONS

#include "Face.h"

struct HalfEdgeData;

struct VertexData {
	unsigned int id;
	Vertex coordinate;
	HalfEdgeData* half;
};

struct FaceData {
	bool matched = false;
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

#endif
