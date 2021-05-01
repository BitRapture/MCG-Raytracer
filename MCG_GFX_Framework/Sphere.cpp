#include "Sphere.h"

// Sphere
namespace MRT
{
	bool Sphere::Intersect(Ray& _ray)
	{
		// Get ray origin and direction
		glm::fvec3 rO = _ray.GetOrigin(), rD = _ray.GetDirection();

		// Calculate vector from ray origin to sphere origin
		glm::fvec3 lRO = position - rO;
		// Project lRO length onto ray direction
		float lPD = glm::dot(lRO, rD);
		// Ray wont intersect if the projected length is behind it
		if (lPD < 0) return false;

		// Get the length of the middle point of the ray to the sphere origin
		float mL = glm::dot(lRO, lRO) - (lPD * lPD);
		// Ray wont interesect if the length is greater than the radius
		if (mL > radiusSqr) return false;

		// Calculate half the length from mL mapped to the ray direction
		float sHL = glm::sqrt(radiusSqr - mL);

		// Calculate the possible starting and ending intersects
		float iStart = lPD - sHL,
			iEnd = lPD + sHL;

		float intersect = iStart;

		// If either intersection lengths are less than 0, there is no intersect
		if (iStart < 0)
		{
			intersect = iEnd;
			if (iEnd < 0) return false;
		}

		// Check if intersect length is greater than current ray length
		if (_ray.GetLength() < intersect) return false;

		// Send hit information to the ray
		HitInformation hitInfo{ intersect, glm::normalize((rO + rD * intersect) - position), color };
		_ray.SetHitInfo(hitInfo);

		return true;
	}

	Sphere::Sphere(glm::fvec3 _position, float _radius, ColorPixel _color)
		:
		Primitive(_position, _color),
		radius{ _radius }
	{
		// Precalculate the square of the radius
		radiusSqr = _radius * _radius;
	}
}
