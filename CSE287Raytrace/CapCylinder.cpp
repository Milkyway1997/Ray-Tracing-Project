#include "CapCylinder.h"

CapCylinder::CapCylinder(const dvec3& position, const color& material, double radius, double length)
	:Cylinder(position, material, radius, length)
{
	Cylinder::setYCylinder();
}

HitRecord CapCylinder::findIntersect(const Ray& ray)
{
	HitRecord hitRecord;
	HitRecord topRecord;
	HitRecord cylinderRecord;

	/*cylinderRecord = Cylinder::findIntersect(ray);
	if (cylinderRecord.t < INFINITY) {
		if (hitRecord.t > cylinderRecord.t) {
			hitRecord = cylinderRecord;
		}
		return hitRecord;
	}*/

	shared_ptr<Plane> topDisk = make_shared<Plane>(Cylinder::topCenter, n, this->material.getDiffuse());
	topRecord = topDisk->findIntersect(ray);
	if (glm::distance(Cylinder::topCenter, topRecord.interceptPoint) < Cylinder::radius) {
		if (hitRecord.t > topRecord.t) {
			hitRecord = topRecord;
		}
		return hitRecord;
	}

	cylinderRecord = Cylinder::findIntersect(ray);
	if (cylinderRecord.t < INFINITY) {
		if (hitRecord.t > cylinderRecord.t) {
			hitRecord = cylinderRecord;
		}
		return hitRecord;
	}

	HitRecord botRecord;
	shared_ptr<Plane> botDisk = make_shared<Plane>(Cylinder::botCenter, n, this->material.getDiffuse());
	botRecord = botDisk->findIntersect(ray);
	if (glm::distance(Cylinder::botCenter, botRecord.interceptPoint) < Cylinder::radius) {
		if (hitRecord.t > botRecord.t) {
			hitRecord = botRecord;
		}
		return hitRecord;
	}
	
	return hitRecord;
}
