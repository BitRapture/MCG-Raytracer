#include "Plane.h"

// Plane
namespace MRT
{
	float Plane::IntersectPlane(Ray& _ray)
	{
		// Get ray origin and direction
		glm::fvec3 rO = _ray.GetOrigin(), rD = _ray.GetDirection();

		// Set the ray length to 0 by default; no intersection
		float mL = 0;
		// Calculate the dot product from the ray direction and plane direction
		// Used as the denominator for solving mL
		float d = glm::dot(rD, direction);

		// If ray and plane are parallel it will most likely not be 0, a tiny value is compared against
		// to check for an intersection
		if (d > (float)1e-6)
		{
			// Subtract the ray position from the plane position
			glm::fvec3 rOpos = position - rO;
			// The direction (which is a normal) is then dot multiplied with the result
			// and divided by the denominator value to solve the ray length
			mL = glm::dot(rOpos, direction) / d;
		}

		// If an intersection occurred, the ray length will be greater than 0
		return mL;
	}

	Plane::Plane(glm::fvec3& _position, glm::fvec3& _direction, ColorPixel& _color)
		:
		Primitive(_position, _color),
		direction(_direction)
	{
		// Normalise the direction vector just in case
		glm::normalize(direction);
	}
}