#include "MeshWriter.h"
#include <iostream>
#include <fstream>
#include <ctime>

MeshWriter* MeshWriter::instance = new MeshWriter();

MeshWriter::MeshWriter() {
	// Empty Constructor
}

MeshWriter::~MeshWriter() {
	delete instance;
	instance = 0;
}

MeshWriter* MeshWriter::getInstance() {
	return instance;
}

void MeshWriter::writeMesh(std::string &filepath, const TriangularMesh* mesh, std::map<unsigned int, unsigned int> &matching) {

	// Output the start of writing
	std::cout << "Writing matching to " << filepath << std::endl;

	// Start time
	const clock_t beginTime = clock();

	// Delete output file first if it already exists
	std::remove(filepath.c_str());

	// Open the file in given location
	std::ofstream outfile(filepath);

	// Write the file format
	outfile << "OFF" << std::endl;

	// Calculate the mesh information
	unsigned int numVertices = mesh->getVertexCount();
	unsigned int numFaces = mesh->getFaceCount() / 2;
	unsigned int numEdges = numFaces * 4 / 2;

	// Write the mesh information
	outfile << numVertices << " " << numFaces << " " << numEdges << std::endl;

	// Write the vertices into the file
	VertexList vertexList = mesh->getVertices();
	for (unsigned int i = 0; i < numVertices; i++) {

		// Get the vertex coordinates
		Vertex vertex = vertexList[i]->coordinate;

		// Write the vertex coordinates
		outfile << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
	}

	// Write the quadrilateral face vertices using matching map
	FaceList faceList = mesh->getFaces();
	for (auto it = matching.begin(); it != matching.end(); it++) {

		// Get the matched faces
		FaceData* first = faceList[it->first];
		FaceData* second = faceList[it->second];

		// Find the matched edge that is shared between faces
		HalfEdgeData* matchedEdge = first->half;
		while (!matchedEdge->matched) {
			matchedEdge = matchedEdge->next;
		}

		// Find the vertices of the matching
		unsigned int a = matchedEdge->origin->id;
		unsigned int b = matchedEdge->pair->next->next->origin->id;
		unsigned int c = matchedEdge->pair->origin->id;
		unsigned int d = matchedEdge->next->next->origin->id;

		// Output the vertices of the quadrilateral
		outfile << "4 " << a << " " << b << " " << c << " " << d << std::endl;
	}

	// Close the output file
	outfile.close();

	// End time
	const clock_t endTime = clock();

	// Output the time difference
	float timeDifference = float(endTime - beginTime);
	std::cout << "Writing Time: " << timeDifference / CLOCKS_PER_SEC << " seconds" << std::endl;
}
