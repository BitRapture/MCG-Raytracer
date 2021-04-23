#ifndef _MCG_RAYTRACING_H_
#define _MCG_RAYTRACING_H_

#include "MCG_GFX_Lib.h"

namespace MRT
{
	// ColorPixel
	// - A simple structure containing 3 floats for RGB color
	// - Each color is normalised (0 to 1.f) 
	struct ColorPixel
	{
		float r{ 0 }, g{ 0 }, b{ 0 };
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

	public:

		glm::fvec3 GetOrigin() { return origin;  }

		glm::fvec3 GetDirection() { return direction; }

		// Instantiation
		// @param _origin : Optional, sets the ray origin
		// @param _dir : Optional, sets the ray direction, needs to be normalised
		Ray(glm::fvec3 _origin = { 0, 0, 0 }, glm::fvec3 _dir = { 0, 0, 0 });
		~Ray();
	};

	// Raytracing camera
	// - Contains an image plane context for rendering
	// - Sets rays to allow for tracing from the camera origin
	class Camera
	{
	private:
		// The image plane
		ColorPixel* imagePlane{ nullptr };

		// Image aspect ratios
		float imageAspectX{ 1.f }, imageAspectY{ 1.f };
		// Image dimensions
		int imageWidth{ 0 }, imageHeight{ 0 };

		// Field of view (default ~60 degrees)
		float fov{ 0.57f };

		// Camera matrix (used for rotation & translation)
		// Converts camera to world space
		glm::fmat4 camToWorld;

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

		// Trace a ray
		// @param _x : The current pixel X coordinate on the image plane (0 to imageWidth-1)
		// @param _y : The current pixel Y coordinate on the image plane (0 to imageHeight-1)
		// @param _ray : The ray that will be setup for tracing
		// @param _sampleX : The sample X coordinate on a pixel (0 to 1.f)
		// @param _sampleY : The sample Y coordinate on a pixel (0 to 1.f)
		void TraceRay(const int& _x, const int& _y, Ray& _ray, const float& _sampleX, const float& _sampleY);

		// Set FOV
		// @param _angleDeg : An angle in degrees (0 to PI)
		void SetFOV(float _angleDeg);

		// Check camera is initialised
		// @returns bool : true when successfully initialised
		bool IsInit();

		// Instantiation
		// @param _pixelWidth : The width of the image plane (screenWidth)
		// @param _pixelHeight : The height of the image plane (screenHeight)
		Camera(const int& _pixelWidth, const int& _pixelHeight);
		~Camera();
	};

	// Primitive
	// - Base class for all primitive geometry
	// - Can not be instantiated (pure virtual)
	class Primitive
	{
	protected:
		glm::fvec3 position;

	public:
		virtual bool Intersect(Ray& _ray) = 0;

		Primitive(glm::fvec3& _position);
	};

	// Sphere
	// - Extends Primitive
	// - Used for creating sphere shapes
	class Sphere : public Primitive
	{
	private:
		float radius{ 0 }, radiusSqr{ 0 };

	public:
		bool Intersect(Ray& _ray) override;

		Sphere(glm::fvec3 _position, float _radius);
	};
};

#endif


// Sources:
// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays
// https://www.scratchapixel.com/lessons/3d-basic-rendering/computing-pixel-coordinates-of-3d-point/mathematics-computing-2d-coordinates-of-3d-points
// https://github.com/g-truc/glm/blob/master/manual.md#section1
// https://cs.stanford.edu/people/eroberts/courses/soco/projects/1997-98/ray-tracing/implementation.html#intro