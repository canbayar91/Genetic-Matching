#include "GeneticProcessor.h"
#include "MeshReader.h"
#include "MeshWriter.h"
#include <iostream>
#include <ctime>

int main(int argc, char **argv) {

	// Read the file name from command line
	std::string filename = argv[1];

	// Read the triangular mesh from the given file
	const TriangularMesh* mesh = MeshReader::getInstance()->readMesh(filename);

	// Output the total face count of the mesh
	std::cout << "Face Count: " << mesh->getFaceCount() << std::endl;

	// Create a genetic processor instance and initialize it with the mesh instance
	GeneticProcessor* processor = GeneticProcessor::getInstance();
	processor->initialize(mesh);

	// Start time of matching
	const clock_t beginTime = clock();

	// Genetically process the mesh to optimize matching
	processor->process();

	// End time of matching
	const clock_t endTime = clock();

	// Output the time difference
	float timeDifference = float(endTime - beginTime);
	std::cout << "Total Running Time: " << timeDifference / CLOCKS_PER_SEC;

	// Write the quadrilateral mesh into an off file
	// std::map<unsigned int, unsigned int> matching = builder->getMatching();
	// MeshWriter::getInstance()->writeMesh("output.off", mesh, matching);

	// Close the program on key press
	getchar();
	return 0;
}