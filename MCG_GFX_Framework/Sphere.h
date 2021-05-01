#ifndef _SPHERE_H_
#define _SPHERE_H_

// Included libraries
#include "MCG_GFX_Lib.h"

// Core modules
#include "UtilityModules.h"
#include "Ray.h"
#include "Primitive.h"

namespace MRT
{
	// Sphere
	// - Extends Primitive
	// - Used for creating sphere shapes
	class Sphere : public Primitive
	{
	private:
		float radius{ 0 }, radiusSqr{ 0 };

	public:
		// Check if ray intersects sphere
		// Uses the geometric solution to calculate intersection
		// @param _ray : The ray to check for an intersection
		// @returns bool : true if intersecting
		bool Intersect(Ray& _ray) override;

		Sphere(glm::fvec3 _position, float _radius, ColorPixel _color = { 1, 0, 0 });
	};
}

#endif // !_SPHERE_H_
