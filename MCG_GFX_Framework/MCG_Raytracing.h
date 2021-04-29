#ifndef _MCG_RAYTRACING_H_
#define _MCG_RAYTRACING_H_

#include "MCG_GFX_Lib.h"

namespace MRT
{
	class Primitive;

	// ColorPixel
	// - A simple structure containing 3 floats for RGB color
	// - Each color is normalised (0 to 1.f) 
	struct ColorPixel
	{
		float r{ 0 }, g{ 0 }, b{ 0 };
	};

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
		glm::fvec3 GetOrigin() { return origin;  }

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
		float GetLength() { return hitInfo.length;  }

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
	// Let MRT::RayTracer access private members of Camera
	friend class RayTracer;

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

		// Check camera is initialised
		// @returns bool : true when successfully initialised
		bool IsInit() { return fInitialised; };

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

		// Add a Primitive object to the scene
		// @param _object : The object to add to the scene (needs to be created from new)
		void AddPrimitive(Primitive* _object);

		// Raytrace the entire scene
		void RenderScene();

		RayTracer(int _screenWidth, int _screenHeight);
		~RayTracer();
	};
};

#endif


// Sources:
// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays
// https://www.scratchapixel.com/lessons/3d-basic-rendering/computing-pixel-coordinates-of-3d-point/mathematics-computing-2d-coordinates-of-3d-points
// https://github.com/g-truc/glm/blob/master/manual.md#section1
// https://cs.stanford.edu/people/eroberts/courses/soco/projects/1997-98/ray-tracing/implementation.html#intro
// https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection