#include "Quadrilateral.h"
#include <fstream>

Quadrilateral::Quadrilateral() {
	this->a = Vertex(0, 0, 0);
	this->b = Vertex(0, 0, 0);
	this->c = Vertex(0, 0, 0);
	this->d = Vertex(0, 0, 0);
}

Quadrilateral::Quadrilateral(const Vertex &a, const Vertex &b, const Vertex &c, const Vertex &d) {
	this->a = a;
	this->b = b;
	this->c = c;
	this->d = d;
}

Quadrilateral::Quadrilateral(const Quadrilateral& quadrilateral) {
	this->a = quadrilateral.a;
	this->b = quadrilateral.b;
	this->c = quadrilateral.c;
	this->d = quadrilateral.d;
}

void Quadrilateral::output(std::string filename) const {

	// Open the file in given location
	std::ofstream outfile(filename);

	// Write the file format
	outfile << "OFF" << std::endl;
	outfile << "4 1 4" << std::endl;	

	// Write the vertex coordinates to file
	outfile << a.x << " " << a.y << " " << a.z << std::endl;
	outfile << b.x << " " << b.y << " " << b.z << std::endl;
	outfile << c.x << " " << c.y << " " << c.z << std::endl;
	outfile << d.x << " " << d.y << " " << d.z << std::endl;

	// Write the vertex order
	outfile << "4 0 1 2 3" << std::endl;

	// Close the input file
	outfile.close();
}
