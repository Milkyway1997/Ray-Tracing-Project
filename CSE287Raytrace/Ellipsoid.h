#pragma once

#include "QuadricSurface.h"

/**
* Sub-class of ImplicitSurface that represents implicit description of a sphere.
*/
class Ellipsoid : public QuadricSurface
{
public:

	/**
	* Constructor for the sphere.
	* @param - point: specifies an xyz position of the center of the sphere
	* @param - radius: radius of the sphere
	* @param - material: color of the plane.
	*/
	Ellipsoid(const dvec3& position = dvec3(0.0, 0.0, -5.0),
		const color& material = color(1.0, 1.0, 1.0, 1.0), const double a = 2, const double b = 1, const double c = 1);

	/**
	* Checks a ray for intersection with the surface. Finds the closest point of intersection
	* if one exits. Returns a HitRecord with the t parameter set to INFINITY if there is no
	* intersection.
	* @param rayOrigin - Origin of the ray being check for intersection
	* @param rayDirection - Unit vector representation the direction of the ray.
	* returns HitRecord containing information about the point of intersection.
	*/
	virtual HitRecord findIntersect(const Ray& ray) override;

	//Origin is the center of the ellipsoid and the coordinate axes are axes of the ellipsoid
	//a, b, and c is coordinates axes of the ellipsoid.
	double a, b, c;
};