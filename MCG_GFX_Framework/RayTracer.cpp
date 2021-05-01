#include "RayTracer.h"

// The RayTracer
namespace MRT
{
	ColorPixel RayTracer::Shade(Ray& _ray)
	{
		// Get the hit information from the ray
		HitInformation hitInfo = _ray.GetHitInfo();
		// Calculate the facing ratio by getting the dot product of the hitnormal and viewing direction (-rayDir)
		// having it clamped to 0 lets the final ratio be between 0 and 1.0f
		float facingRatio = glm::max(0.05f, glm::dot(hitInfo.hitNormal, -_ray.GetDirection()));

		return { facingRatio * hitInfo.hitColor.r, facingRatio * hitInfo.hitColor.g, facingRatio * hitInfo.hitColor.b };
	}

	void RayTracer::SetBackgroundColor(ColorPixel _color)
	{
		backgroundDefault = _color;
	}

	void RayTracer::AddPrimitive(Primitive* _object)
	{
		if (!fInitialised || primiAmount >= primiMax) return;

		// Calculate the distance from the object to the camera 
		glm::fvec3 pos = _object->GetPosition() - camera.position;
		float dist = glm::sqrt((pos.x * pos.x) + (pos.y * pos.y) + (pos.z * pos.z));

		// Compare primitive lengths to _object, insert into ordered list of lengths (primiMap)
		int insert = primiAmount;
		bool inserting = false;
		for (int i = 0; i < primiAmount; ++i)
		{
			if (primiMap[i] > dist)
			{
				insert = i;
				inserting = true;
				break;
			}
		}

		// If inserting, move all objects down in the primiManager
		if (inserting)
		{
			for (int i = primiAmount; i >= insert; --i)
			{
				primiMap[i + 1] = primiMap[i];
				primiManager[i + 1] = primiManager[i];
			}
		}

		primiManager[insert] = _object;
		primiMap[insert] = dist;
		++primiAmount;
	}

	void RayTracer::ClearPrimitives()
	{
		// Free all elements from array
		for (int i = 0; i < primiAmount; ++i)
		{
			delete primiManager[i];
			primiManager[i] = nullptr;
			primiMap[i] = 0;
		}
		// Set amount to none
		primiAmount = 0;
	}

	void RayTracer::RenderScene()
	{
		// Set the background to the background color
		MCG::SetBackground({ backgroundDefault.r, backgroundDefault.g, backgroundDefault.b });

		// Hardcoded sampling coordinates (TODO, add sampling)
		float samplingX = 0.5f, samplingY = 0.5f;
		// Loop through every pixel on the screen
		for (int y = 0; y < screenH; ++y)
		{
			for (int x = 0; x < screenW; ++x)
			{
				// Create the drawing color
				ColorPixel drawingColor = backgroundDefault;
				// Create an empty ray
				Ray ray;
				// Cast the ray in the direction of the camera from its position
				camera.CastRay(x, y, ray, samplingX, samplingY);


				for (int i = primiAmount - 1; i >= 0; --i)
				{
					if (primiManager[i]->Intersect(ray))
					{
						drawingColor = Shade(ray);
					}
				}

				camera.DrawToPlane(x, y, drawingColor);

				camera.DisplayPlanePixel(x, y);
			}
		}
	}

	RayTracer::RayTracer(int _screenWidth, int _screenHeight)
		:
		camera(_screenWidth, _screenHeight),
		screenW{ _screenWidth }, screenH{ _screenHeight },
		backgroundDefault{ 0.4f, 0.5f, 1.0f }
	{
		// Check that camera is initialised
		fInitialised = camera.IsInit();
		// Allocate memory for the primitive objects
		primiManager = new Primitive * [primiMax];
		primiMap = new float[primiMax] { 0 };
		// Check that the memory was allocated
		fInitialised &= (primiManager != nullptr);
		fInitialised &= (primiMap != nullptr);
	}

	RayTracer::~RayTracer()
	{
		// Free all elements from array
		for (int i = 0; i < primiAmount; ++i)
		{
			delete primiManager[i];
		}
		// Delete the primitive manager
		delete[] primiManager;
		delete[] primiMap;
	}
}