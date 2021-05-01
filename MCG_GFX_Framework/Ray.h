#ifndef _RAY_H_
#define _RAY_H_

// Included libraries
#include "MCG_GFX_Lib.h"

// Core modules
#include "UtilityModules.h"

namespace MRT
{
	// HitInformation
	// - Ray hit information for raytracing
	// - Saves the most recent intersection data from an object
	struct HitInformation
	{
		// The current length of the ray to the object
		float length{ 0 };

		// The normal of the object that has been hit
		glm::fvec3 hitNormal;

		// The color of the objects the ray has hit (Additive blending)
		ColorPixel hitColor;
	};

	// Ray
	// - A single ray for tracing and intersecting
	// - All members defaulted to 0, needs to be setup through CTOR
	// - Can be setup via an MRT::Camera
	class Ray
	{
		// Let MRT::Camera access private members of Ray
		friend class Camera;

	private:
		// Origin is in world space 
		// Direction that the ray is pointing, needs to be normalised
		glm::fvec3 origin, direction;

		// The rays hit information
		// used to work out object distances, shading, lighting, etc
		HitInformation hitInfo;

	public:
		// Get the ray origin
		// @returns glm::fvec3 : The ray origin
		glm::fvec3 GetOrigin() { return origin; }

		// Get the ray direction
		// @returns glm::fvec3 : The ray direction
		glm::fvec3 GetDirection() { return direction; }

		// Set ray hit information
		// @param _info : The information to be saved to the ray
		void SetHitInfo(HitInformation& _info) { hitInfo = _info; }

		// Get ray hit information
		// @returns HitInformation : The current rays hit information
		HitInformation GetHitInfo() { return hitInfo; }

		// Get ray length
		// Very useful to get this variable alone (instead of returning full hitInfo)
		// @returns float : The current length of the ray
		float GetLength() { return hitInfo.length; }

		// Instantiation
		// @param _origin : Optional, sets the ray origin
		// @param _dir : Optional, sets the ray direction, needs to be normalised
		Ray(glm::fvec3 _origin = { 0, 0, 0 }, glm::fvec3 _dir = { 0, 0, 0 });
		~Ray();
	};
}

#endif // !_RAY_H_
