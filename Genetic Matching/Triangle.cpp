#include "Triangle.h"
#include <fstream>

Triangle::Triangle() {
	this->a = Vertex(0, 0, 0);
	this->b = Vertex(0, 0, 0);
	this->c = Vertex(0, 0, 0);
}

Triangle::Triangle(const Vertex &a, const Vertex &b, const Vertex &c) {
	this->a = a;
	this->b = b;
	this->c = c;
}

Triangle::Triangle(const Triangle& triangle) {
	this->a = triangle.a;
	this->b = triangle.b;
	this->c = triangle.c;
}

void Triangle::output(std::string filename) const {

	// Open the file in given location
	std::ofstream outfile(filename);

	// Write the file format
	outfile << "OFF" << std::endl;
	outfile << "3 1 3" << std::endl;

	// Write the vertex coordinates to file
	outfile << a.x << " " << a.y << " " << a.z << std::endl;
	outfile << b.x << " " << b.y << " " << b.z << std::endl;
	outfile << c.x << " " << c.y << " " << c.z << std::endl;

	// Write the vertex order
	outfile << "3 0 1 2" << std::endl;

	// Close the input file
	outfile.close();
}
