#ifndef TRIANGULAR_MESH
#define TRIANGULAR_MESH

#include "Definitions.h"
#include <vector>
#include <map>

typedef std::vector<VertexData*> VertexList;
typedef std::vector<FaceData*> FaceList;

class TriangularMesh {
public:

	TriangularMesh(std::vector<Vertex> vertices, std::vector<Face> faces);
	~TriangularMesh();

	const VertexList getVertices() const;
	const FaceList getFaces() const;

	size_t getVertexCount() const;
	size_t getFaceCount() const;

private:

	VertexList vertices;
	FaceList faces;

	size_t vertexCount;
	size_t faceCount;

	std::map<VertexPair, HalfEdgeData*> lookupMap;

	void pairHalfEdges(unsigned int origin, unsigned int destination, HalfEdgeData* halfEdge);

};

#endif
