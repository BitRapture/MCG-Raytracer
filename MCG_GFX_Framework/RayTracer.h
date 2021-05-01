#ifndef _RAYTRACER_H_
#define _RAYTRACER_H_

// Included libraries
#include "MCG_GFX_Lib.h"

// Core modules
#include "UtilityModules.h"
#include "Ray.h"
#include "Camera.h"
#include "Primitive.h"

namespace MRT
{
	// The Raytracer
	// - An all encompassing class that simplifies the raytracing process
	// - Contains Primitive managing system
	// - Contains Camera
	// - Contains adjustable parameters to tweak simulation
	class RayTracer
	{
		// Adjustable parameters
	private:
		// Default background color
		ColorPixel backgroundDefault;

		// Internal variables
	private:
		// Primitives manager (hard-capped to 100 objects)
		Primitive** primiManager{ nullptr };
		int primiMax{ 100 }, primiAmount{ 0 };
		// An array to order objects closest to the camera
		float* primiMap{ nullptr };

		// The raytracing camera
		Camera camera;

		// Screen dimensions
		int screenW{ 0 }, screenH{ 0 };

		// RayTracer flags
		// Is RayTracer initialised
		bool fInitialised{ false };

		// Private functions
	private:

		// Internal shading sub system
		// Creates shading after rays have intersected objects
		// @param _ray : The ray to be used to shade an object
		// @returns ColorPixel : The color of the object
		ColorPixel Shade(Ray& _ray);

		// Public functions
	public:

		// Is the system initialised
		// @returns bool : true if initialised
		bool IsInit() { return fInitialised; };

		// Set the default background color
		// @param _color : RGB normalised color value
		void SetBackgroundColor(ColorPixel _color);

		// Set camera position in world using its method
		// @param _position : The new position of the camera
		void SetCameraPosition(glm::fvec3 _position) { camera.SetPosition(_position); }

		// Rotate camera using its method
		// @param _axis : The axis to apply the rotation around
		// @param _angle : The angle to be applied (degrees)
		void SetCameraRotation(glm::fvec3 _axis, float _angle) { camera.SetRotation(_axis, _angle); }

		// Set camera to look at a point using its method
		// @param _point : The position vector to look at
		void SetCameraTarget(glm::fvec3 _point) { camera.LookAt(_point); }

		// Set camera FOV
		// @param _fov : The new fov (degrees)
		void SetCameraFOV(float _fov) { camera.SetFOV(_fov); }

		// Set the max viewing render distance of the camera using its method
		// @param _distance : The new distance
		void SetCameraRenderDistance(float _distance) { camera.SetRenderDistance(_distance); }

		// Add a Primitive object to the scene
		// @param _object : The object to add to the scene (needs to be created from new)
		void AddPrimitive(Primitive* _object);

		// Delete all primitives from the manager
		void ClearPrimitives();

		// Raytrace the entire scene
		void RenderScene();

		// Instantiation
		// @param _screenWidth : The window screen width
		// @param _screenHeight : The window screen height
		RayTracer(int _screenWidth, int _screenHeight);
		~RayTracer();
	};
}

#endif // !_RAYTRACER_H_
