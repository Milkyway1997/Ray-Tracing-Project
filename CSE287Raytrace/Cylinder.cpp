#include "Cylinder.h"

Cylinder::Cylinder(const dvec3& position, const color& material, double radius, double length)
	:QuadricSurface(position, material), radius(radius),length(length)
{
		isX = true;
		isY = false;
		isZ = false;
		A = 0;
		B = 1 / pow(radius, 2);
		C = 1 / pow(radius, 2);
		D = 0;
		E = 0;
		F = 0;
		G = 0;
		H = 0;
		I = 0;
		J = -1;
		topCenter = position;
		topCenter[0] = position[0] + length / 2;
		botCenter = position;
		botCenter[0] = position[0] - length / 2;
}

HitRecord Cylinder::findIntersect(const Ray& ray)
{
	HitRecord hitRecord;
	hitRecord.t = INFINITY;
	HitRecord hitCylinder = QuadricSurface::findIntersect(ray);
	
	if (isX) {
		if (hitCylinder.interceptPoint[0] > botCenter[0] && hitCylinder.interceptPoint[0] < topCenter[0]) {
			return hitCylinder;
		}
	}
	else if (isY) {
		if (hitCylinder.interceptPoint[1] > botCenter[1] && hitCylinder.interceptPoint[1] < topCenter[1]) {
			hitCylinder.uv = calcCylindricalTextCoord(hitCylinder.interceptPoint - this->center, 12, Y_AXIS);
			return hitCylinder;
		}
	}
	else if (isZ) {
		if (hitCylinder.interceptPoint[2] > botCenter[2] && hitCylinder.interceptPoint[2] < topCenter[2]) {
			return hitCylinder;
		}
	}

	if (hitCylinder.t < INFINITY) {
		Ray newRay = Ray(hitCylinder.interceptPoint - EPSILON * hitCylinder.surfaceNormal, ray.direct);
		HitRecord hitBackCylinder = QuadricSurface::findIntersect(newRay);
		if (isX) {
			if (hitBackCylinder.interceptPoint[0] > botCenter[0] && hitBackCylinder.interceptPoint[0] < topCenter[0]) {
				return hitBackCylinder;
			}
		}
		else if (isY) {
			if (hitBackCylinder.interceptPoint[1] > botCenter[1] && hitBackCylinder.interceptPoint[1] < topCenter[1]) {
				hitBackCylinder.uv = calcCylindricalTextCoord(hitBackCylinder.interceptPoint - this->center, 12, Y_AXIS);
				return hitBackCylinder;
			}
		}
		else if (isZ) {
			if (hitBackCylinder.interceptPoint[2] > botCenter[2] && hitBackCylinder.interceptPoint[2] < topCenter[2]) {
				return hitBackCylinder;
			}
		}
	}

	return hitRecord;
}

void Cylinder::setXCylinder()
{
	isX = true;
	isY = false;
	isZ = false;
	A = 0;
	B = 1 / pow(radius, 2);
	C = 1 / pow(radius, 2);
	D = 0;
	E = 0;
	F = 0;
	G = 0;
	H = 0;
	I = 0;
	J = -1;
	topCenter = center;
	topCenter[0] = center[0] + length / 2;
	botCenter = center;
	botCenter[0] = center[0] - length / 2;
}

void Cylinder::setYCylinder()
{
	isY = true;
	isX = false;
	isZ = false;
	A = 1 / pow(radius, 2);
	B = 0;
	C = 1 / pow(radius, 2);
	D = 0;
	E = 0;
	F = 0;
	G = 0;
	H = 0;
	I = 0;
	J = -1;
	topCenter = center;
	topCenter[1] = center[1] + length / 2;
	botCenter = center;
	botCenter[1] = center[1] - length / 2;
}

void Cylinder::setZCylinder()
{
	isZ = true;
	isX = false;
	isY = false;
	A = 1 / pow(radius, 2);
	B = 1 / pow(radius, 2);
	C = 0;
	D = 0;
	E = 0;
	F = 0;
	G = 0;
	H = 0;
	I = 0;
	J = -1;
	topCenter = center;
	topCenter[2] = center[2] + length / 2;
	botCenter = center;
	botCenter[2] = center[2] - length / 2;
}
