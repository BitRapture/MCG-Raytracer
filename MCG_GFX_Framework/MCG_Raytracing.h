#ifndef _MCG_RAYTRACING_H_
#define _MCG_RAYTRACING_H_

#include "MCG_GFX_Lib.h"

// Included libraries
#include <iostream>
#include <string>

// Core modules
#include "UtilityModules.h"
#include "Ray.h"
#include "Camera.h"
#include "Primitive.h"
#include "Sphere.h"
#include "Plane.h"
#include "Circle.h"
#include "RayTracer.h"

// This header file groups together all usable modules into a scene manager

// MRT namespace (MCG. Ray. Tracer.)
namespace MRT
{
	// Scene Manager
	// - Allows for custom scenes to be created
	// - Lets users change parameters and add/remove objects
	// - Contains pointer to RayTracer system
	// - Uses <iostream> to query user input
	// (TODO): Add filesystem to read scene instructions from file
	class SceneManager
	{
	private:
		// Pointing to an already instantiated raytracer system
		// allows for modifying before instantiation of scene system
		RayTracer* raytracer{ nullptr };

		// Instruction list
		// Things the user can do to the scene/raytracer
		std::string* instructions;
		// Instructions set (the size of the instructions array)
		int instructionSet{ 0 };

		// SceneManager flags
		// Check system is completely initialised
		bool fInitialised{ false };
		// Check that user input is enabled
		bool fUserEnabled{ true };

	// Internal methods
	private:
		// Add all instructions to the instruction list
		void FillInstructionList();

		// Instructions
		// These all have the same parameters unless specified
		// @param _argv : The argument array parsed down
		// @param _argc : The amount of arguments inside the array
		// @returns bool : If the instruction successfully run
		// -------------------------------------------------------
		// Display the help page
		void InstHelp();
		// Turn user input off
		void InstUioff();
		// Renders the scene
		void InstRender();
		// Clear the scene
		void InstClear();
		// Set the scene background color
		bool InstColor(std::string* _argv, int& _argc);
		// Move the raytracing camera
		bool InstMove(std::string* _argv, int& _argc);
		// Rotate the raytracing camera around axis
		bool InstRotate(std::string* _argv, int& _argc);
		// Set raytracing camera to look at target
		bool InstLookat(std::string* _argv, int& _argc);
		// Change field of view of raytracing camera
		bool InstFOV(std::string* _argv, int& _argc);
		// Add a circle to the scene
		bool InstCircle(std::string* _argv, int& _argc);
		// Add a sphere to the scene
		bool InstSphere(std::string* _argv, int& _argc);

	public:

		// Run the scene manager and poll user input through iostream
		void Run();

		// Instantiation
		// @param _raytracer : Pointer to raytracing system
		SceneManager(RayTracer* _raytracer);
	};
};

#endif // !_MCG_RAYTRACING_H_


// All sources used in this project:
/*
	Jeremiah (2011). Understanding the View Matrix - 3D Game Engine Programming. [online] 3D Game Engine Programming. Available at: https://www.3dgep.com/understanding-the-view-matrix/ [Accessed 29 Apr. 2021].

	Lu, C., Roetter, A. and Schultz, A. (1997). Ray Tracing Implementation. [online] cs.stanford.edu. Available at: https://cs.stanford.edu/people/eroberts/courses/soco/projects/1997-98/ray-tracing/implementation.html#intro [Accessed 28 Apr. 2021].

	Scratchapixel (n.d.). A Minimal Ray-Tracer: Rendering Simple Shapes (Sphere, Cube, Disk, Plane, etc.) (Ray-Plane and Ray-Disk Intersection). [online] www.scratchapixel.com. Available at: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection [Accessed 28 Apr. 2021].

	Scratchapixel (n.d.). Computing the Pixel Coordinates of a 3D Point (Mathematics of Computing the 2D Coordinates of a 3D Point). [online] www.scratchapixel.com. Available at: https://www.scratchapixel.com/lessons/3d-basic-rendering/computing-pixel-coordinates-of-3d-point/mathematics-computing-2d-coordinates-of-3d-points [Accessed 28 Apr. 2021].

	Scratchapixel (n.d.). Ray-Tracing: Generating Camera Rays (Generating Camera Rays). [online] www.scratchapixel.com. Available at: https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays [Accessed 28 Apr. 2021].

	Shene, C.-K. (2011). Geometric Transformations. [online] pages.mtu.edu. Available at: https://pages.mtu.edu/~shene/COURSES/cs3621/NOTES/geometry/geo-tran.html [Accessed 29 Apr. 2021].

	Wikipedia Contributors (2018). Line (geometry). [online] Wikipedia. Available at: https://en.wikipedia.org/wiki/Line_(geometry) [Accessed 28 Apr. 2021].

	Wikipedia Contributors (2021). Line–plane intersection. [online] Wikipedia. Available at: https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection [Accessed 28 Apr. 2021].
*/