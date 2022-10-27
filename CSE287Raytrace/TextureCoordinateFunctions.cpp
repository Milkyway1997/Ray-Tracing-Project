#include "TextureCoordinateFunctions.h"

dvec2 calcSphericalTextCoord(const dvec3& objectCoord, const double& width, const double& height, AXIS axis)
{
	double s, t;

	dvec3 normObjCoord = glm::normalize(objectCoord);

	if (axis == X_AXIS) {
		s = 0.5 - atan2(normObjCoord.z, normObjCoord.y) / TWO_PI;
		t = 0.5 - (asin(normObjCoord.x) / PI);
	}
	else if (axis == Y_AXIS) {
		s = 0.5 - atan2(normObjCoord.z, normObjCoord.x) / TWO_PI;
		t = 0.5 - (asin(normObjCoord.y) / PI);
	}
	else { // Z_AXIS
		s = 0.5 - atan2(normObjCoord.y, normObjCoord.x) / TWO_PI;
		t = 0.5 - (asin(normObjCoord.z) / PI);
	}

	return glm::dvec2(s, t);

}

dvec2 calcPlanarTextCoord(const dvec3& objectCoord, const double& width, const double& height, AXIS axis)
{
	double s, t;
	if (axis == X_AXIS) {
		s = objectCoord.z / width + width / 2;
		t = objectCoord.y / height + height / 2;
	}
	else if (axis == Y_AXIS) {
		s = objectCoord.x / width + width / 2;
		t = objectCoord.z / height + height / 2;
	}
	else { // Z_AXIS	
		s = objectCoord.x / width + width / 2;
		t = objectCoord.y / height + height / 2;
	}

	return dvec2(s, t);
}

dvec2 calcCylindricalTextCoord(const dvec3& localCoordinates, const double& height, AXIS axis)
{
	double s, t;

	if (axis == X_AXIS) {
		s = 0.5 - atan2(localCoordinates.y, localCoordinates.z) / TWO_PI;
		t = 0.5 - localCoordinates.x / height;
	}
	else if (axis == Y_AXIS) {
		s = 0.5 - atan2(localCoordinates.x, localCoordinates.z) / TWO_PI;
		t = 0.5 - localCoordinates.y / height;
	}
	else { // Z_AXIS
		s = 0.5 - atan2(localCoordinates.x, localCoordinates.y) / TWO_PI;
		t = 0.5 - localCoordinates.z / height;
	}

	return glm::dvec2(s, t);
}

dvec2 calcSineTextCoord(const dvec3& localCoordinates, const double& height, const dvec3& axis)
{
	//float scaleS = 5; // scale of the pattern 
	//float pattern = (sin(hitTexCoordinates.x * 2 * M_PI * scaleS) + 1) * 0.5; // compute sine wave pattern 
	//hitColor += vis * pattern * lightIntensity * std::max(0.f, hitNormal.dotProduct(-lightDir));

	//float scaleS = 5, scaleT = 5; // scale of the pattern 
	//float pattern = (cos(hitTexCoordinates.y * 2 * M_PI * scaleT) * sin(hitTexCoordinates.x * 2 * M_PI * scaleS) + 1) * 0.5; // compute sine wave pattern 
	//hitColor += vis * pattern * lightIntensity * std::max(0.f, hitNormal.dotProduct(-lightDir));

	double s = 1, t = 1;

	return glm::dvec2(s, t);
}



//glm::vec2 Surface::calculateSphericalTextureCoordinates(glm::vec3 localCoordinates)
//{
//	glm::vec3 vp = glm::normalize(localCoordinates);
//	glm::vec3 vn(0.0f, 1.0f, 0.0f);
//	glm::vec3 ve(1.0f, 0.0f, 0.0f);
//
//	float phi = glm::acos(-glm::dot(vn, vp));
//
//	float v = phi / PI;
//
//	float temp = 2.0f * PI;
//
//	float theta = glm::acos(glm::dot(vp, ve) / glm::sin(phi)) / temp;
//
//	float u = 1 - theta;
//	if (glm::dot(glm::cross(vn, ve), vp) > 0) {
//		u = theta;
//	}
//
//	return glm::vec2(u, v);
//
//}
//
//glm::vec2 Surface::calculatePlanarTextureCoordinates(glm::vec3 localCoordinates, float height, float width, glm::vec3 normal)
//{
//	glm::vec3 n = glm::normalize(normal);
//	float x, y;
//	if ((n.z > n.y) && (n.z > n.x)) {
//		x = localCoordinates.x + width;
//		y = localCoordinates.y + height;
//	}
//	else if ((n.y > n.z) && (n.y > n.x)) {
//		x = localCoordinates.x + width;
//		y = localCoordinates.z + height;
//	}
//	else {
//		x = localCoordinates.z + width;
//		y = localCoordinates.y + height;
//	}
//
//	float u = x / width;
//	float v = y / height;
//
//	return glm::vec2(u, v);
//}
//
//dvec2 ImplicitSurface::calculateCylindricalTextureCoordinates(dvec3 localCoordinates, double height, dvec3 axis)
//{
//	double theta, y;
//
//	double repeat = 1.0;
//
//	if ((axis.z > axis.y) && (axis.z > axis.x)) {
//		y = localCoordinates.z + height / 2.0;
//		theta = repeat * atan2(localCoordinates.x, localCoordinates.y);
//	}
//	else if ((axis.y > axis.z) && (axis.y > axis.x)) {
//		y = localCoordinates.y + height / 2.0;
//		theta = repeat * atan2(localCoordinates.z, localCoordinates.x) ;
//	}
//	else {
//		y = 0.5 - localCoordinates.x + height / 2.0;
//		theta = repeat * atan2(localCoordinates.z, localCoordinates.y);
//	}
//
//	// Normalize theta
//	theta = fmod(theta, M_PI);
//
//	if (theta < 0) {
//
//		theta += M_PI;
//	}
//
//	double u = theta / (2.0f * M_PI);
//
//	double v = y / height;
//
//	return glm::vec2(u, v);
//}