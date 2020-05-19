#include "GeometricFunctions.h"
#include <cmath>

double GeometricFunctions::dotProduct(const Vector &a, const Vector &b) {

	// Calculate and return the dot product value
	return a.getProductX() * b.getProductX() + a.getProductY() * b.getProductY() + a.getProductZ() * b.getProductZ();
}

double GeometricFunctions::crossProduct(const Vector &a, const Vector &b) {

	// Calculate and return the cross product value
	double positivePart = a.getProductY() * b.getProductZ() + a.getProductZ() * b.getProductX() + a.getProductX() * b.getProductY();
	double negativePart = a.getProductZ() * b.getProductY() + a.getProductX() * b.getProductZ() + a.getProductY() * b.getProductX();
	return positivePart - negativePart;
}

bool GeometricFunctions::checkConcavityByAngle(const Quadrilateral* quadrilateral) {

	// Traverse through angles and see if any of them is bigger than 180
	// This function assumes that input quadrilaterals are planar

	// Get the edge vectors for the first vertex
	const Vector AB(quadrilateral->a, quadrilateral->b);
	const Vector AD(quadrilateral->a, quadrilateral->d);

	// Calculate the angle between vectors and return true if the angle > 180
	if (GeometricFunctions::calculateAngle(AB, AD) > 180.0) {
		return true;
	}

	// Get the edge vectors for the second vertex
	const Vector BC(quadrilateral->b, quadrilateral->c);
	const Vector BA(quadrilateral->b, quadrilateral->a);

	// Calculate the angle between vectors and return true if the angle > 180
	if (GeometricFunctions::calculateAngle(BC, BA) > 180.0) {
		return true;
	}

	// Get the edge vectors for the third vertex
	const Vector CD(quadrilateral->c, quadrilateral->d);
	const Vector CB(quadrilateral->c, quadrilateral->b);

	// Calculate the angle between vectors and return true if the angle > 180
	if (GeometricFunctions::calculateAngle(CD, CB) > 180.0) {
		return true;
	}

	// Get the edge vectors for the fourth vertex
	const Vector DA(quadrilateral->d, quadrilateral->a);
	const Vector DC(quadrilateral->d, quadrilateral->c);

	// Calculate the angle between vectors and return true if the angle > 180
	if (GeometricFunctions::calculateAngle(DA, DC) > 180.0) {
		return true;
	}

	// Return false, if the quadrilateral is convex
	return false;
}

bool GeometricFunctions::checkConcavityByIntersection(const Quadrilateral* quadrilateral) {

	// Get the diagonals and see if they cross each other
	// This function assumes that input quadrilaterals are planar

	// Get the diagonals on the quadrilateral
	const Edge diagonalAC(quadrilateral->a, quadrilateral->c);
	const Edge diagonalBD(quadrilateral->b, quadrilateral->d);

	// If diagonals do not intersect, the quadrilateral is concave
	return !checkLineIntersection(diagonalAC, diagonalBD);
}

bool GeometricFunctions::checkPlanarity(const Quadrilateral* quadrilateral) {
	// TODO - Implement planarity function
	return true;
}

bool GeometricFunctions::checkLineIntersection(const Edge &a, const Edge &b) {

	// Vertices of the first line segment
	const Vertex p1 = a.start;
	const Vertex p2 = a.end;

	// Vertices of the second line segment
	const Vertex q1 = b.start;
	const Vertex q2 = b.end;

	// Calculate the normal endpoints for the line segments
	const Vertex na(p2.y - p1.y, p2.x - p1.x);
	const Vertex nb(q2.y - q1.y, q2.x - q1.x);

	// Check if both endpoints of line segments are on the same side
	bool firstCond = ((q1.x - p1.x) * na.x - (q1.y - p1.y) * na.y) * ((q2.x - p1.x) * na.x - (q2.y - p1.y) * na.y) < 0;
	bool secondCond = ((p1.x - q1.x) * nb.x - (p1.y - q1.y) * nb.y) * ((p2.x - q1.x) * nb.x - (p2.y - q1.y) * nb.y) < 0;

	// Result depends on both conditions
	return firstCond && secondCond;
}

const Vertex GeometricFunctions::findLineIntersection(const Edge &a, const Edge &b) {

	// Edge A is represented as a1x + b1y = c1
	double a1 = a.end.y - a.start.y;
	double b1 = a.start.x - a.end.x;
	double c1 = a1 * a.start.x + b1 * a.start.y;

	// Edge B is represented as a2x + b2y = c2
	double a2 = b.end.y - b.start.y;
	double b2 = b.start.x - b.end.x;
	double c2 = a2 * b.start.x + b2 * b.start.y;

	// Find the intersection point
	double determinant = a1 * b2 - a2 * b1;
	double x = (b2 * c1 - b1 * c2) / determinant;
	double y = (a1 * c2 - a2 * c1) / determinant;

	// Return the intersection point
	return Vertex(x, y);
}

const Normal GeometricFunctions::findNormal(const Vector &a, const Vector &b) {

	// Find the length of the normal vector in xyz-coordinates
	// TODO - Cross product can return 0
	double x = a.getProductY() * b.getProductZ() - a.getProductZ() * b.getProductY();
	double y = a.getProductZ() * b.getProductX() - a.getProductX() * b.getProductZ();
	double z = a.getProductX() * b.getProductY() - a.getProductY() * b.getProductX();

	// Add the vector lengths into the starting point
	Vertex endpoint(a.start.x + x, a.start.y + y, a.start.z + z);

	// Create the normal vector
	Normal normal(a.start, endpoint);

	// Return the normal vector
	return normal;
}

Angle GeometricFunctions::calculateAngle(const Vector &a, const Vector &b) {

	// Calculate the dot product
	double dotProduct = GeometricFunctions::dotProduct(a, b);

	// Get the lengths of the edges
	double lengthA = a.getLength();
	double lengthB = b.getLength();

	// Calculate the angle between the edges in degrees
	Angle angle = acos(dotProduct / (lengthA * lengthB)) * 180.0 / PI;

	// Return the angle
	return angle;
}

Angle GeometricFunctions::degreesToRadians(const Angle angle) {

	// Convert degrees to radians
	return angle * PI / 180.0;
}

Angle GeometricFunctions::radiansToDegrees(const Angle angle) {

	// Convert radians to degrees
	return angle * 180.0 / PI;
}

const Vector GeometricFunctions::normalizeVector(const Vector &vector) {

	// Get the length of the vector
	double length = vector.getLength();

	// Calculate the normalized vector endpoints
	double x = vector.start.x + vector.getLengthX() / length;
	double y = vector.start.y + vector.getLengthY() / length;
	double z = vector.start.z + vector.getLengthZ() / length;
	
	// Create and return the normalized vector
	return Vector(vector.start, Vertex(x, y, z));
}
