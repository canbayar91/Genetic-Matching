#ifndef FACE
#define FACE

#include "Vertex.h"

struct Face {

	unsigned int a;
	unsigned int b;
	unsigned int c;

	Face(unsigned int a = 0, unsigned int b = 0, unsigned int c = 0) {
		this->a = a;
		this->b = b;
		this->c = c;
	}

	Face(const Face& face) {
		a = face.a;
		b = face.b;
		c = face.c;
	}
};

#endif
