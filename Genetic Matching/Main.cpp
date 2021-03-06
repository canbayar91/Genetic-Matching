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
	std::cout << "Triangle Count: " << mesh->getFaceCount() << std::endl;

	// Create a genetic processor instance and initialize it with the mesh instance
	GeneticProcessor* processor = new GeneticProcessor(mesh);

	// Start time of matching
	const clock_t beginTime = clock();

	// Genetically process the mesh to optimize matching
	Individual fittest = processor->process();

	// End time of matching
	const clock_t endTime = clock();

	// Output the time difference and fittest individual statistics
	float timeDifference = float(endTime - beginTime);
	std::cout << "Algorithm Running Time: " << timeDifference / CLOCKS_PER_SEC << " seconds" << std::endl;
	std::cout << "Fittest Individual Average Score: " << fittest.getAverageFitness() << std::endl;
	std::cout << "Fittest Individual Match Count: " << fittest.getMatchCount() << std::endl;

	// Write the quadrilateral mesh into an off file
	// std::map<unsigned int, unsigned int> matching = builder->getMatching();
	// MeshWriter::getInstance()->writeMesh("output.off", mesh, matching);

	// Close the program on key press
	std::cout << "Press any key to exit..." << std::endl;
	getchar();
	return 0;
}