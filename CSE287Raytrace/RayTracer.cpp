#include "RayTracer.h"


RayTracer::RayTracer(FrameBuffer& cBuffer, color defaultColor)
	:colorBuffer(cBuffer), defaultColor(defaultColor), recursionDepth(2)
{

}


void RayTracer::setCameraFrame(const dvec3& viewPosition, const dvec3& viewingDirection, dvec3 up)
{
	eye = viewPosition;

	w = glm::normalize(-viewingDirection);

	u = glm::normalize(glm::cross(up, w));

	v = glm::normalize(glm::cross(w, u));

} // end setCameraFrame


void RayTracer::calculatePerspectiveViewingParameters(const double& verticalFieldOfViewDegrees)
{
	double fovRad = glm::radians(verticalFieldOfViewDegrees);

	distToPlane = 1.0 / tan(fovRad / 2.0); // distance from the view plane based on fov

	topLimit = 1.0;

	rightLimit = topLimit * ((double)colorBuffer.getWindowWidth() / colorBuffer.getWindowHeight()); // Set r based on aspect ratio and height of plane

	// Make view plane symetrical about the viewing direction
	leftLimit = -rightLimit;
	bottomLimit = -topLimit;

	// Calculate the distance between pixels in the horizontal and vertical directions
	nx = (double)colorBuffer.getWindowWidth();
	ny = (double)colorBuffer.getWindowHeight();

	renderPerspectiveView = true; // generate perspective view rays

} // end calculatePerspectiveViewingParameters


void RayTracer::calculateOrthographicViewingParameters(const double& viewPlaneHeight)
{
	topLimit = fabs(viewPlaneHeight) / 2.0;

	rightLimit = topLimit * ((double)colorBuffer.getWindowWidth() / colorBuffer.getWindowHeight()); // Set r based on aspect ratio and height of plane

	// Make view plane symetrical about the viewing direction
	leftLimit = -rightLimit;
	bottomLimit = -topLimit;

	// Calculate the distance between pixels in the horizontal and vertical directions
	nx = (double)colorBuffer.getWindowWidth();
	ny = (double)colorBuffer.getWindowHeight();

	distToPlane = 0.0; // Rays start on the view plane

	renderPerspectiveView = false; // generate orthographic view rays

} // end calculateOrthographicViewingParameters


void RayTracer::raytraceScene()
{
	// Iterate through each and every pixel in the rendering window
	for (int y = 0; y < colorBuffer.getWindowHeight(); y ++) {
		for (int x = 0; x < colorBuffer.getWindowWidth(); x ++) {

			Ray viewRay;
			color colorForPixel;

			if (renderPerspectiveView == false) {

				// Set the rayOrigin and rayDirection for perspective projection
				//This is Antialiasing******************************************************************************
				if (isAntialiasing) {
					double x1 = x;
					double y1 = y;
					for (int i = 0; i < 3; i++) {
						for (int j = 0; j < 3; j++) {
							Ray newViewRay = getOrthoViewRay(x1 + i / 4.0, y1 + j / 4.0);
							colorForPixel += traceRay(newViewRay, recursionDepth);
						}
					}
					// Trace a ray for a specific pixel
					colorForPixel = colorForPixel / 9.0;
				}
				else {
					viewRay = getOrthoViewRay(x, y);
				}
			}
			else {
				// Set the rayOrigin and rayDirection for perspective projection
				//This is Antialiasing******************************************************************************
				if (isAntialiasing) { 
					double x1 = x;
					double y1 = y;
					for (int i = 0; i < 3; i++) {
						for (int j = 0; j < 3; j++) {
							Ray newViewRay = getPerspectiveViewRay(x1 + i / 4.0, y1 + j / 4.0);
							colorForPixel += traceRay(newViewRay, recursionDepth);
						}
					}
					// Trace a ray for a specific pixel
					colorForPixel = colorForPixel / 9.0;
				}
				else {
					viewRay = getPerspectiveViewRay(x, y);
				}
			}
			//This is Antialiasing******************************************************************************
			//if (isAntialiasing) {
			//	double x1 = x;
			//	double y1 = y;
			//	for (int i = 0; i < 3; i++) {
			//		for (int j = 0; j < 3; j++) {
			//			Ray newViewRay = getPerspectiveViewRay(x1 + i / 4.0, y1 + j / 4.0);
			//			colorForPixel += traceRay(newViewRay, recursionDepth);
			//		}
			//	}
			//	// Trace a ray for a specific pixel
			//	colorForPixel = colorForPixel / 9.0;
			//}

			// Trace a ray for a specific pixel
			if (!isAntialiasing)
			colorForPixel = traceRay(viewRay, recursionDepth);

			// Set the pixel color
			colorBuffer.setPixel(x, y, colorForPixel);
		}
	}

} // end raytraceScene

/**
 * @fn    static double fresnel(const dvec3& i, const dvec3& n, const double& etai, const double& etat)
 *
 * @brief    Compute Fresnel equation
 *
 * @param    i       is the incident view direction.
 * @param    n       is the normal at the intersection point.
 * @param    etai    is the refractive index of the material the light is leaving.
 * @param    etat    is the refractive index of the material the light is entering.
 *
 * @returns    kr is the percentage of light reflected As a consequence of the conservation of
 *             energy, transmittance is given by: kt = 1 - kr
 *
 */
static double fresnel(const dvec3& i, const dvec3& n, const double& etai, const double& etat)
{
	// Percentage of light that is reflected
	// Percentage of light that is refracted is equal to 1-kr
	double kr;

	// Calculate the dot product of the incidence vector and the normal 
	// vector of the surface the the light is entering
	double cosi = glm::clamp(-1.0, 1.0, glm::dot(i, n));

	// Compute the sine of the angle of refraction using Snell's law
	double sint = etai / etat * sqrt(glm::max(0.0, 1.0 - cosi * cosi));

	// Check if angle of incidence exceeds critical angle 
	if (sint >= 1.0) {
		kr = 1.0; // Total internal reflection
	}
	else {
		// Calculate the percentage of light that will be reflected
		double cost = sqrt(glm::max(0.0, 1.0 - sint * sint));
		cosi = fabs(cosi);

		// S polarized light (parallel)
		double Rs = ((etat * cosi) - (etai * cost)) / ((etai * cost) + (etat * cosi));
		// P polarized light (perpendicular)
		double Rp = ((etai * cosi) - (etat * cost)) / ((etat * cost) + (etai * cosi));

		kr = (Rs * Rs + Rp * Rp) / 2.0;
	}

	return kr;

} // end fresnel

color RayTracer::traceRay(const Ray& ray, int recursionLevel)
{
	// Find surface intersection that is closest to the origin of the viewRay
	HitRecord closestHit = findClosestIntersection(ray);

	// Check if an intersection occurred
	if (closestHit.t < INFINITY) {

		color totalColor = closestHit.material.getEmisive();

		for (auto light : lights) {
			//This is Attenuation factor ********************************************
			double d = light->getLightDistance(closestHit.interceptPoint);
			double att = 1 / (CONSTANT_ATTEN + d * LINEAR_ATTEN + pow(d, 2) * QUADRATIC_ATTEN);
			//This is shadow Feeler********************************************
			dvec3 shadowFeeler = light->getLightVector(closestHit.interceptPoint);
			HitRecord shadowHit = findClosestIntersection(Ray(closestHit.interceptPoint + EPSILON * closestHit.surfaceNormal, shadowFeeler));

			if (shadowHit.t > closestHit.t) {
				totalColor += light->getLocalIllumination(-ray.direct, closestHit.interceptPoint,
					closestHit.surfaceNormal, closestHit.material,closestHit.uv);
			}
			//Attenuation
			if(light->getLightDistance(closestHit.interceptPoint)<INFINITY)
			totalColor *= att;
		}
			//This is Reflection********************************************
			dvec3 reflectionDirection = glm::reflect(ray.direct, closestHit.surfaceNormal);
			Ray reflectionRay = Ray(closestHit.interceptPoint + EPSILON * closestHit.surfaceNormal, reflectionDirection);
			
			//This is refraction********************************************
			if (closestHit.material.isDielectric) {
				double etat;
				double etai;
				if (closestHit.rayStatus == ENTERING) {
					etat = 1.1;
					etai = closestHit.material.dielecticRefractionIndex; //air
				}
				else {
					etat = closestHit.material.dielecticRefractionIndex; //air
					etai = 1.1;
				}
				dvec3 refractionDirection = glm::refract(ray.direct, closestHit.surfaceNormal, etat / etai);
				Ray refractionRay = Ray(closestHit.interceptPoint - EPSILON * closestHit.surfaceNormal, refractionDirection);
				double kr = fresnel(ray.direct, closestHit.surfaceNormal, etai, etat);

				if (kr < 1) {
					totalColor = BLACK;
					totalColor += (1.0 - kr) * traceRay(refractionRay, recursionLevel);
				}
				else {
					totalColor = closestHit.material.getDiffuse();
				}
			}	
			else {
				if (recursionLevel == 0)
					return totalColor;
				else {
					totalColor += traceRay(reflectionRay, recursionLevel - 1);
				}
			}
		return totalColor;
	}
	else {
		if (reflectSky) {
			if (recursionLevel == 0)
				return defaultColor * 0.5;
			return defaultColor;
		}
		if (recursionLevel == 0)
			return BLACK;
		return defaultColor;
	}

} // end traceRay


HitRecord RayTracer::findClosestIntersection(const Ray& ray)
{

	HitRecord closestHit;
	closestHit.t = INFINITY;

	// Check if the ray intersects any surfaces in the scene
	for (auto& surface : this->surfaces) {

		HitRecord hitRec = surface->findIntersect(ray);

		// Check if it is closer that previous intersection.
		if (hitRec.t < closestHit.t) {

			closestHit = hitRec;
		}

	}

	return closestHit;

} // end findIntersection


Ray RayTracer::getOrthoViewRay(const double& x, const double& y)
{
	Ray orthoViewRay;

	dvec2 uv = getImagePlaneCoordinates(x, y);

	orthoViewRay.origin = eye + uv.x * u + uv.y * v;
	orthoViewRay.direct = glm::normalize(-w);

	return orthoViewRay;

} // end getOrthoViewRay


Ray RayTracer::getPerspectiveViewRay(const double& x, const double& y)
{
	Ray perspectiveViewRay;

	dvec2 uv = getImagePlaneCoordinates(x, y);

	perspectiveViewRay.origin = eye;
	perspectiveViewRay.direct = glm::normalize((-distToPlane) * w + uv.x * u + uv.y * v);

	return perspectiveViewRay;

} // end getPerspectiveViewRay


dvec2 RayTracer::getImagePlaneCoordinates(const int& x, const int& y)
{
	dvec2 s;

	s.x = leftLimit + (rightLimit - leftLimit) * (x + 0.5) / nx;
	s.y = bottomLimit + (topLimit - bottomLimit) * (y + 0.5) / ny;
	return s;

} // end getImagePlaneCoordinates
