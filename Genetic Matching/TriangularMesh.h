#ifndef TRIANGULAR_MESH
#define TRIANGULAR_MESH

#include "Definitions.h"
#include <vector>
#include <map>

typedef std::vector<VertexData*> VertexList;
typedef std::vector<FaceData*> FaceList;

class TriangularMesh {
public:

	// Constructor
	TriangularMesh(std::vector<Vertex> vertices, std::vector<Face> faces);

	// Destructor
	~TriangularMesh();

	// Get vertex list
	const VertexList &getVertices() const;

	// Get face list
	const FaceList &getFaces() const;

	// Get vertex count
	size_t getVertexCount() const;

	// Get face count
	size_t getFaceCount() const;

private:

	// Vertex data
	VertexList vertices;
	
	// Face data
	FaceList faces;

	// Vertex count
	size_t vertexCount;
	
	// Face count
	size_t faceCount;

	// Lookup map to is used to determine the neighborhoods
	std::map<VertexPair, HalfEdgeData*> lookupMap;

	// Pairs the half-edges that are sharing the same vertices but pointing to different directions
	void pairHalfEdges(unsigned int origin, unsigned int destination, HalfEdgeData* halfEdge);

};

#endif
