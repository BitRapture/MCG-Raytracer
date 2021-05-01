#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_

// Included libraries
#include "MCG_GFX_Lib.h"

// Core modules
#include "UtilityModules.h"
#include "Ray.h"

namespace MRT
{
	// Primitive
	// - Base class for all primitive geometry
	// - Can not be instantiated (pure virtual)
	class Primitive
	{
	protected:
		// The position of the primitive
		glm::fvec3 position;
		// The color of the primitive
		ColorPixel color;

	public:
		// Pure virtual intersection function for primitives to override
		// @param _ray : The ray to check for an intersection
		// @returns bool : true if intersecting
		virtual bool Intersect(Ray& _ray) = 0;

		// Get the position vector
		// @returns glm::fvec3 : The position vector
		glm::fvec3 GetPosition() { return position; }

		// Get the color
		// @returns ColorPixel : The color
		ColorPixel GetColor() { return color; }

		Primitive(glm::fvec3& _position, ColorPixel& _color);
		Primitive();
	};
}

#endif // !_PRIMITIVE_H_
