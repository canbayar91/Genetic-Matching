#include "MeshReader.h"
#include <iostream>
#include <fstream>
#include <ctime>

MeshReader* MeshReader::instance = new MeshReader();

MeshReader::MeshReader() {
	// Empty Constructor
}

MeshReader::~MeshReader() {
	delete instance;
	instance = 0;
}

MeshReader* MeshReader::getInstance() {
	return instance;
}

const TriangularMesh* MeshReader::readMesh(std::string &filepath) {

	// Output the start of reading
	std::cout << "Reading mesh from " << filepath << std::endl;

	// Start time
	const clock_t beginTime = clock();

	// Open the file in given location
	std::ifstream infile(filepath);

	// Read the file format
	std::string format;
	infile >> format;

	// Read the mesh information
	unsigned int numVertices, numFaces, numEdges;
	infile >> numVertices >> numFaces >> numEdges;

	// Read the vertices from the file
	std::vector<Vertex> vertexList(numVertices);
	for (unsigned int i = 0; i < numVertices; i++) {

		// Read vertex coordinates
		float x, y, z;
		infile >> x >> y >> z;

		// Add it to the vertex list
		vertexList[i] = Vertex(x, y, z);
	}

	// Read the triangular faces from the file and creates a triangular mesh
	std::vector<Face> faceList(numFaces);
	for (unsigned int i = 0; i < numFaces; i++) {

		// Read the number of vertices (always 4 for quadrilateral)
		unsigned int vertexCount;
		infile >> vertexCount;

		// Read the quadrilateral vertices
		unsigned int a, b, c;
		infile >> a >> b >> c;

		// Construct the triangle and add it to the list
		faceList[i] = Face(a, b, c);
	}

	// Close the input file
	infile.close();

	// Create the mesh using vertex and face list
	TriangularMesh* mesh = new TriangularMesh(vertexList, faceList);

	// End time
	const clock_t endTime = clock();

	// Output the time difference
	float timeDifference = float(endTime - beginTime);
	std::cout << "Reading Time: " << timeDifference / CLOCKS_PER_SEC << " seconds" << std::endl;

	// Return the resulting list
	return mesh;
}
