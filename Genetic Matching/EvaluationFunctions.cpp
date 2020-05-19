#include "EvaluationFunctions.h"
#include <functional>
#include <algorithm>
#include <cmath>

const double EvaluationFunctions::calculateMinimumScaledJacobian(const Quadrilateral* quadrilateral) {

	// Create triangles for each corner
	Triangle* triangleABD = new Triangle(quadrilateral->a, quadrilateral->b, quadrilateral->d);
	Triangle* triangleBCA = new Triangle(quadrilateral->b, quadrilateral->c, quadrilateral->a);
	Triangle* triangleCDB = new Triangle(quadrilateral->c, quadrilateral->d, quadrilateral->b);
	Triangle* triangleDAC = new Triangle(quadrilateral->d, quadrilateral->a, quadrilateral->c);

	// Project the triangle areas created by the edge vectors to the z=0 plane
	projectTriangle(triangleABD);
	projectTriangle(triangleBCA);
	projectTriangle(triangleCDB);
	projectTriangle(triangleDAC);

	// Calculate the scaled jacobian values for each triangle
	double scaledJacobianABD = calculateScaledJacobian(triangleABD);
	double scaledJacobianBCA = calculateScaledJacobian(triangleBCA);
	double scaledJacobianCDB = calculateScaledJacobian(triangleCDB);
	double scaledJacobianDAC = calculateScaledJacobian(triangleDAC);

	// Delete the created triangles to free memory
	delete triangleABD;
	delete triangleBCA;
	delete triangleCDB;
	delete triangleDAC;

	// Return the minimum scaled jacobian value
	return std::min(std::min(scaledJacobianABD, scaledJacobianBCA), std::min(scaledJacobianCDB, scaledJacobianDAC));
}

const double EvaluationFunctions::calculateDistortion(const Quadrilateral* quadrilateral) {

	// Get the diagonals on the quadrilateral
	const Vector diagonalAC(quadrilateral->a, quadrilateral->c);
	const Vector diagonalBD(quadrilateral->b, quadrilateral->d);

	// Find the intersection point of the diagonals
	// TODO - Diagonals of the concave quadrilaterals do not intersect
	const Vertex intersection = GeometricFunctions::findLineIntersection(diagonalAC, diagonalBD);

	// Create four triangles created by the diagonals
	Triangle* triangleAB = new Triangle(quadrilateral->a, quadrilateral->b, intersection);
	Triangle* triangleBC = new Triangle(quadrilateral->b, quadrilateral->c, intersection);
	Triangle* triangleCD = new Triangle(quadrilateral->c, quadrilateral->d, intersection);
	Triangle* triangleDA = new Triangle(quadrilateral->d, quadrilateral->a, intersection);

	// Calculate the triangle distortion amounts
	double alphaValues[Quadrilateral::POINT_COUNT];
	alphaValues[0] = calculateTriangleDistortion(triangleAB);
	alphaValues[1] = calculateTriangleDistortion(triangleBC);
	alphaValues[2] = calculateTriangleDistortion(triangleCD);
	alphaValues[3] = calculateTriangleDistortion(triangleDA);

	// Sort the triangle distortion amounts
	std::sort(std::begin(alphaValues), std::end(alphaValues), std::greater<double>());

	// Delete the created triangles to free memory
	delete triangleAB;
	delete triangleBC;
	delete triangleCD;
	delete triangleDA;

	// Calculate and return the quadrilateral distortion
	return (alphaValues[2] * alphaValues[3]) / (alphaValues[0] * alphaValues[1]);
}

const double EvaluationFunctions::calculateDistortion2(const Quadrilateral* quadrilateral) {

	// Calculate cross product for vertex A
	Vector AB(quadrilateral->a, quadrilateral->b);
	Vector AD(quadrilateral->a, quadrilateral->d);
	double cpA = GeometricFunctions::crossProduct(AB, AD);

	// Calculate cross product for vertex B
	Vector BC(quadrilateral->b, quadrilateral->c);
	Vector BA(quadrilateral->b, quadrilateral->a);
	double cpB = GeometricFunctions::crossProduct(BC, BA);

	// Calculate cross product for vertex C
	Vector CD(quadrilateral->c, quadrilateral->d);
	Vector CB(quadrilateral->c, quadrilateral->b);
	double cpC = GeometricFunctions::crossProduct(CD, CB);

	// Calculate cross product for vertex D
	Vector DA(quadrilateral->d, quadrilateral->a);
	Vector DC(quadrilateral->d, quadrilateral->c);
	double cpD = GeometricFunctions::crossProduct(DA, DC);

	// Calculate square length total values
	double squareSumA = pow(AB.getLength(), 2) + pow(AD.getLength(), 2);
	double squareSumB = pow(BC.getLength(), 2) + pow(BA.getLength(), 2);
	double squareSumC = pow(CD.getLength(), 2) + pow(CB.getLength(), 2);
	double squareSumD = pow(DA.getLength(), 2) + pow(DC.getLength(), 2);

	// Calculate the nominator and denominator
	double nominator = abs(cpA) + abs(cpB) + abs(cpC) + abs(cpD);
	double denominator = squareSumA + squareSumB + squareSumC + squareSumD;

	// Return the distortion value
	return 2 * sqrt(sqrt(nominator / denominator));
}

const double EvaluationFunctions::calculateMaximumWarpage(const Quadrilateral* quadrilateral) {

	// Calculate warpage amount on both diagonals
	double horizontalWarpage = calculateWarpage(quadrilateral, HORIZONTAL);
	double verticalWarpage = calculateWarpage(quadrilateral, VERTICAL);

	// Return the horizontal warpage amount if it is greater
	if (horizontalWarpage > verticalWarpage) {
		return horizontalWarpage;
	}

	// Return the vertical warpage amount elsewhere
	return verticalWarpage;
}

const double EvaluationFunctions::calculateAspectRatio(const Quadrilateral* quadrilateral) {

	// Project the quadrilateral onto a plane in case it is not planar
	const Quadrilateral projectedQuadrilateral = projectQuadrilateral(quadrilateral);

	// Find the middle point of the edges
	const Vertex midAB = (projectedQuadrilateral.a + projectedQuadrilateral.b) / 2;
	const Vertex midBC = (projectedQuadrilateral.b + projectedQuadrilateral.c) / 2;
	const Vertex midCD = (projectedQuadrilateral.c + projectedQuadrilateral.d) / 2;
	const Vertex midDA = (projectedQuadrilateral.d + projectedQuadrilateral.a) / 2;

	// Create edges passing through the middle points
	const Edge verticalEdge(midAB, midCD);
	const Edge horizontalEdge(midBC, midDA);

	// Calculate the aspect-ratio by dividing the longer edge by shorter one
	double aspectRatio;
	if (verticalEdge.getLength() > horizontalEdge.getLength()) {
		aspectRatio = verticalEdge.getLength() / horizontalEdge.getLength();
	} else {
		aspectRatio = horizontalEdge.getLength() / verticalEdge.getLength();
	}

	// Return the aspect-ratio value
	return aspectRatio;
}

const double EvaluationFunctions::calculateScaledJacobian(const Triangle* triangle) {

	// Get the edge vectors on the projected triangle
	Vector left(triangle->a, triangle->c);
	Vector right(triangle->a, triangle->b);

	// Calculate the determinant value for x-y coordinates
	double determinant = left.getLengthX() * right.getLengthY() - right.getLengthX() * left.getLengthY();

	// Return the scaled Jacobian value
	return determinant / left.getLength() / right.getLength();
}

const double EvaluationFunctions::calculateWarpage(const Quadrilateral* quadrilateral, AlignmentType alignment) {

	// Calculate the warpage of the quadrilateral on given diagonal
	Angle warpage = 0.0;
	if (alignment == HORIZONTAL) {

		// Get the edges of the quadrilateral for the horizontal diagonal
		const Vector AB(quadrilateral->a, quadrilateral->b);
		const Vector AD(quadrilateral->a, quadrilateral->d);
		const Vector CB(quadrilateral->c, quadrilateral->b);
		const Vector CD(quadrilateral->c, quadrilateral->d);

		// Calculate the normals for both triangles for the horizontal diagonal
		const Normal normalABD = GeometricFunctions::findNormal(AB, AD);
		const Normal normalCDB = GeometricFunctions::findNormal(CD, CB);

		// Calculate the angle between the normals for the horizontal diagonal
		warpage = GeometricFunctions::calculateAngle(normalABD, normalCDB);

	} else {

		// Get the edges of the quadrilateral for the vertical diagonal
		const Vector BA(quadrilateral->b, quadrilateral->a);
		const Vector BC(quadrilateral->b, quadrilateral->c);
		const Vector DA(quadrilateral->d, quadrilateral->a);
		const Vector DC(quadrilateral->d, quadrilateral->c);

		// Calculate the normals for both triangles for the vertical diagonal
		const Normal normalBCA = GeometricFunctions::findNormal(BC, BA);
		const Normal normalDAC = GeometricFunctions::findNormal(DA, DC);

		// Calculate the angle between the normals for the vertical diagonal
		warpage = GeometricFunctions::calculateAngle(normalBCA, normalDAC);
	}

	// Return the warpage
	return warpage;
}

const double EvaluationFunctions::calculateTriangleDistortion(const Triangle* triangle) {

	// Get the edges of the triangle
	const Vector edgeAB(triangle->a, triangle->b);
	const Vector edgeCA(triangle->c, triangle->a);
	const Vector edgeCB(triangle->c, triangle->b);

	// Calculate the nominator and denominator
	const double areaElement = GeometricFunctions::crossProduct(edgeCA, edgeCB);
	const double squareSum = pow(edgeCA.getLength(), 2) + pow(edgeAB.getLength(), 2) + pow(edgeCB.getLength(), 2);

	// Find the normal direction
	int normalDirection;
	if (GeometricFunctions::crossProduct(edgeCA, edgeCB) > 0) {
		normalDirection = 1;
	} else {
		normalDirection = -1;
	}

	// Return the triangle distortion value
	return normalDirection * 2 * sqrt(3) * abs(areaElement) / squareSum;
}

const Vertex EvaluationFunctions::calculateCornerAverage(const Quadrilateral* quadrilateral) {

	// Create and return the average vertex
	return (quadrilateral->a + quadrilateral->b + quadrilateral->c + quadrilateral->d) / Quadrilateral::POINT_COUNT;
}

const Normal EvaluationFunctions::calculateNormalAverage(const Quadrilateral* quadrilateral) {

	// Calculate the normals on the first vertex
	const Vector AB(quadrilateral->a, quadrilateral->b);
	const Vector AD(quadrilateral->a, quadrilateral->d);
	const Normal normalA = GeometricFunctions::findNormal(AB, AD);

	// Calculate the normals on the second vertex
	const Vector BC(quadrilateral->b, quadrilateral->c);
	const Vector BA(quadrilateral->b, quadrilateral->a);
	const Normal normalB = GeometricFunctions::findNormal(BC, BA);

	// Calculate the normals on the third vertex
	const Vector CD(quadrilateral->c, quadrilateral->d);
	const Vector CB(quadrilateral->c, quadrilateral->b);
	const Normal normalC = GeometricFunctions::findNormal(CD, CB);

	// Calculate the normals on the fourth vertex
	const Vector DA(quadrilateral->d, quadrilateral->a);
	const Vector DC(quadrilateral->d, quadrilateral->c);
	const Normal normalD = GeometricFunctions::findNormal(DA, DC);

	// Create start and end vertices for the normal
	const Vertex startVertex = (normalA.start + normalB.start + normalC.start + normalD.start) / Quadrilateral::POINT_COUNT;
	const Vertex endVertex = (normalA.end + normalB.end + normalC.end + normalD.end) / Quadrilateral::POINT_COUNT;

	// Create the average normal
	const Normal average(startVertex, endVertex);

	// Normalize the average normal into a unit vector
	const Normal normal = GeometricFunctions::normalizeVector(average);

	// Return the unit normal
	return normal;
}

const void EvaluationFunctions::projectTriangle(Triangle* &triangle) {

	// Create a temporary edge to complete the triangle
	const Vector leftover(triangle->c, triangle->b);

	// Get the vectors on triangle adjacent to vertex A
	Vector left(triangle->a, triangle->b);
	Vector right(triangle->a, triangle->c);

	// Get the edge lengths
	double leftEdgeLength = left.getLength();
	double rightEdgeLength = right.getLength();
	double leftoverLength = leftover.getLength();

	// Calculate the x-y values for the third vertex
	double x = (pow(leftEdgeLength, 2) - pow(leftoverLength, 2) + pow(rightEdgeLength, 2)) / (2 * rightEdgeLength);
	double y = sqrt(pow(leftEdgeLength, 2) - pow(x, 2));

	// Create vertices that are aligned with z=0 plane
	const Vertex middleVertex(0, 0);
	const Vertex rightVertex(rightEdgeLength, 0);
	const Vertex leftVertex(x, y);

	// Update the edge coordinates
	triangle->a = middleVertex;
	triangle->b = leftVertex;
	triangle->c = rightVertex;
}

const Quadrilateral EvaluationFunctions::projectQuadrilateral(const Quadrilateral* quadrilateral) {

	// Calculate the average of the corner points
	const Vertex origin = calculateCornerAverage(quadrilateral);

	// Calculate the average of the normals of the corner points
	const Normal normal = calculateNormalAverage(quadrilateral);

	// Create vectors from the origin to the each vertex of the quadrilateral
	const Vector vectorA(origin, quadrilateral->a);
	const Vector vectorB(origin, quadrilateral->b);
	const Vector vectorC(origin, quadrilateral->c);
	const Vector vectorD(origin, quadrilateral->d);

	// Calculate the distance to the plane by taking dot product of each vertex with the normal
	double distanceA = GeometricFunctions::dotProduct(normal, vectorA);
	double distanceB = GeometricFunctions::dotProduct(normal, vectorB);
	double distanceC = GeometricFunctions::dotProduct(normal, vectorC);
	double distanceD = GeometricFunctions::dotProduct(normal, vectorD);

	// Calculate the projection point for the first vertex
	double x = quadrilateral->a.x - (normal.start.x + distanceA * normal.getLengthX());
	double y = quadrilateral->a.y - (normal.start.y + distanceA * normal.getLengthY());
	double z = quadrilateral->a.z - (normal.start.z + distanceA * normal.getLengthZ());
	const Vertex projectedA(x, y, z);

	// Calculate the projection point for the second vertex
	x = quadrilateral->b.x - (normal.start.x + distanceB * normal.getLengthX());
	y = quadrilateral->b.y - (normal.start.y + distanceB * normal.getLengthY());
	z = quadrilateral->b.z - (normal.start.z + distanceB * normal.getLengthZ());
	const Vertex projectedB(x, y, z);

	// Calculate the projection point for the third vertex
	x = quadrilateral->c.x - (normal.start.x + distanceC * normal.getLengthX());
	y = quadrilateral->c.y - (normal.start.y + distanceC * normal.getLengthY());
	z = quadrilateral->c.z - (normal.start.z + distanceC * normal.getLengthZ());
	const Vertex projectedC(x, y, z);

	// Calculate the projection point for the fourth vertex
	x = quadrilateral->d.x - (normal.start.x + distanceD * normal.getLengthX());
	y = quadrilateral->d.y - (normal.start.y + distanceD * normal.getLengthY());
	z = quadrilateral->d.z - (normal.start.z + distanceD * normal.getLengthZ());
	const Vertex projectedD(x, y, z);

	// Create the quadrilateral with projected vertices
	const Quadrilateral projection(projectedA, projectedB, projectedC, projectedD);

	// Return the projected quadrilateral
	return projection;
}
