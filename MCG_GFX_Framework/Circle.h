#ifndef _CIRCLE_H_
#define _CIRCLE_H_

// Included libraries
#include "MCG_GFX_Lib.h"

// Core modules
#include "UtilityModules.h"
#include "Ray.h"
#include "Primitive.h"
#include "Plane.h"

namespace MRT
{
	// Circle
	// - Extends Plane
	// - Used for creating 2D circles in a 3D environment
	class Circle : public Plane
	{
	private:
		float radius{ 0 }, radiusSqr{ 0 };

	public:
		// Check if ray intersects circle
		// Uses the algebraic form of a plane to calculate intersection
		// @param _ray : The ray to check for an intersection
		// @returns float : The ray length, if (> 0) intersection occurred
		bool Intersect(Ray& _ray) override;

		Circle(glm::fvec3 _position, glm::fvec3 _direction, float _radius, ColorPixel _color = { 1, 0, 0 });
	};
}

#endif // !_CIRCLE_H_
