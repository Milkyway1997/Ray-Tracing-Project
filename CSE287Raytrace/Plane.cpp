#include "Plane.h"


Plane::Plane(const dvec3 & point, const dvec3 & normal, const color & material)
	: ImplicitSurface(material), a(point), n(normalize(normal))
{
}


Plane::Plane(std::vector<dvec3> vertices, const color & material)
	: ImplicitSurface(material)
{
	a = vertices[0];

	n = glm::normalize(glm::cross(vertices[2] - vertices[1], vertices[0] - vertices[1]));
}


HitRecord Plane::findIntersect( const Ray & ray )
{
	HitRecord hitRecord;
	hitRecord.t = INFINITY;
	if (glm::dot(ray.direct, n) == 0)
		return hitRecord;
	if (glm::dot(ray.direct, n) > 0){
		n = -n;
	}
	double t = glm::dot((a - ray.origin), n) / (glm::dot(ray.direct, n));
	if (t >= 0) {
		hitRecord.t = t;
		hitRecord.material = material;
		hitRecord.interceptPoint = ray.origin + t * ray.direct;
		hitRecord.surfaceNormal = n;
		hitRecord.rayStatus = ENTERING;
	}

	return hitRecord;

} // end findClosestIntersection

