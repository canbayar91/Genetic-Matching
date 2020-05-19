#ifndef PRIMITIVE
#define PRIMITIVE

#include "Vector.h"
#include <string>

class Primitive {
public:

	virtual void output(std::string filename) const = 0;

};

#endif
