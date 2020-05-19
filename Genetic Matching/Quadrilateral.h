#ifndef QUADRILATERAL
#define QUADRILATERAL

#include "Primitive.h"

class Quadrilateral : public Primitive {
public:

	const static int POINT_COUNT = 4;

	Vertex a;
	Vertex b;
	Vertex c;
	Vertex d;

	Quadrilateral();
	Quadrilateral(const Vertex &a, const Vertex &b, const Vertex &c, const Vertex &d);
	Quadrilateral(const Quadrilateral& quadrilateral);

	void output(std::string filename) const;

};

#endif
