#ifndef TRIANGLE
#define TRIANGLE

#include "Primitive.h"

class Triangle : public Primitive {
public:

	const static int POINT_COUNT = 3;

	Vertex a;
	Vertex b;
	Vertex c;

	Triangle();
	Triangle(const Vertex &a, const Vertex &b, const Vertex &c);
	Triangle(const Triangle& triangle);
	
	void output(std::string filename) const;

};

#endif
