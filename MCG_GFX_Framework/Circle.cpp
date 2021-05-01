#include "Circle.h"

// Circle
namespace MRT
{
	bool Circle::Intersect(Ray& _ray)
	{
		// Get the ray length by checking the entire plane for an intersection
		float mL = IntersectPlane(_ray);
		// If length is 0, theres no intersection
		if (mL == 0) return false;

		// Check if intersect length is greater than current ray length
		if (_ray.GetLength() < mL) return false;

		// Get ray hit position
		glm::fvec3 rH = _ray.GetOrigin() + (_ray.GetDirection() * mL);
		// Get the length of the hit position to the circle center
		glm::fvec3 c = rH - position;

		// Get the dot product and see if the ray is projected inside the circle radius
		// keeping it squared saves performance (dont need to sqrt the dot product)
		if (glm::dot(c, c) > radiusSqr) return false;

		// Send hit information to the ray
		HitInformation hitInfo{ mL, glm::normalize(direction * rH), color };
		_ray.SetHitInfo(hitInfo);

		return true;
	}

	Circle::Circle(glm::fvec3 _position, glm::fvec3 _direction, float _radius, ColorPixel _color)
		:
		Plane(_position, _direction, _color),
		radius{ _radius }
	{
		// Precalculate the square of the radius
		radiusSqr = radius * radius;
	}
}