#ifndef _PLANE_H_
#define _PLANE_H_

// Included libraries
#include "MCG_GFX_Lib.h"

// Core modules
#include "UtilityModules.h"
#include "Ray.h"
#include "Primitive.h"

namespace MRT
{
	// Plane
	// - Extends Primitive
	// - Used for creating planes, can be extended further
	// - Parent class for primitive shapes e.g triangles, circles
	// - Can not be instantiated (Intersect isnt overridden)
	class Plane : public Primitive
	{
	protected:
		glm::fvec3 direction;

	protected:
		// Check if ray intersects plane
		// Uses the algebraic form of a plane to calculate intersection
		// @param _ray : The ray to check for an intersection
		// @returns float : The ray length, if (> 0) intersection occurred
		float IntersectPlane(Ray& _ray);

	public:
		Plane(glm::fvec3& _position, glm::fvec3& _direction, ColorPixel& _color);
	};
}

#endif // !_PLANE_H_
