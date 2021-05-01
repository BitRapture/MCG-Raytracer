#ifndef _CAMERA_H_
#define _CAMERA_H_

// Included libraries
#include "MCG_GFX_Lib.h"

// Core modules
#include "UtilityModules.h"
#include "Ray.h"

namespace MRT
{
	// Raytracing camera
	// - Contains an image plane context for rendering
	// - Sets rays to allow for tracing from the camera origin
	class Camera
	{
		// Let MRT::RayTracer access private members of Camera
		friend class RayTracer;

	private:
		// The image plane
		ColorPixel* imagePlane{ nullptr };

		// Image aspect ratios
		float imageAspectX{ 1.0f }, imageAspectY{ 1.0f };
		// Image dimensions
		int imageWidth{ 0 }, imageHeight{ 0 };

		// Field of view (default ~60 degrees)
		float fov{ 0.57f };

		// Max viewing render distance (default 10000)
		float maxViewingDistance{ 10000.0f };

		// Camera matrix (used for rotation & translation)
		// Converts camera to world space
		glm::fmat4 camToWorld;
		// Rotation matrix 
		glm::fmat4 camRotation;

		// Construct camToWorld matrix
		void ConstructCamMatrix();

		// Camera position
		glm::fvec3 position;

		// Camera flags
		// Check camera has been instantiated 
		bool fInitialised{ false };

	public:

		// Draw to image plane
		// @param _x : The X coordinate of the plane (0 to imageWidth-1)
		// @param _y : The Y coordinate of the plane (0 to imageHeight-1)
		// @param _color : The color of the pixel to be drawn
		// @returns bool : true on success
		bool DrawToPlane(const int& _x, const int& _y, ColorPixel _color);

		// Display the whole image plane
		void DisplayPlane();

		// Display a specific pixel of the image plane
		// @param _x : The X coordinate of the plane (0 to imageWidth-1)
		// @param _y : The Y coordinate of the plane (0 to imageHeight-1)
		void DisplayPlanePixel(const int& _x, const int& _y);

		// Cast a ray
		// @param _x : The current pixel X coordinate on the image plane (0 to imageWidth-1)
		// @param _y : The current pixel Y coordinate on the image plane (0 to imageHeight-1)
		// @param _ray : The ray that will be setup for tracing
		// @param _sampleX : The sample X coordinate on a pixel (0 to 1.f)
		// @param _sampleY : The sample Y coordinate on a pixel (0 to 1.f)
		void CastRay(const int& _x, const int& _y, Ray& _ray, const float& _sampleX, const float& _sampleY);

		// Set FOV
		// @param _angleDeg : An angle in degrees (0 to PI)
		void SetFOV(float _angleDeg);

		// Set camera position in world
		// @param _position : The new position of the camera
		void SetPosition(glm::fvec3 _position);

		// Set max render distance
		// @param _distance : The new distance
		void SetRenderDistance(float _distance) { maxViewingDistance = _distance; }

		// Rotate camera
		// @param _axis : The axis to apply the rotation around
		// @param _angle : The angle to be applied (degrees)
		void SetRotation(glm::fvec3 _axis, float _angle);

		// Look at point
		// @param _point : The position vector to look at
		void LookAt(glm::fvec3 _point);

		// Check camera is initialised
		// @returns bool : true when successfully initialised
		bool IsInit() { return fInitialised; };

		// Instantiation
		// @param _pixelWidth : The width of the image plane (screenWidth)
		// @param _pixelHeight : The height of the image plane (screenHeight)
		Camera(const int& _pixelWidth, const int& _pixelHeight);
		~Camera();
	};
}

#endif // !_CAMERA_H_