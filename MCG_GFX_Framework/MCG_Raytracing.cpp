#include "MCG_Raytracing.h"

#include <iostream>

// Ray
namespace MRT
{
	Ray::Ray(glm::fvec3 _origin, glm::fvec3 _dir)
		:
		origin(_origin), direction(_dir)
	{

	}
	Ray::~Ray()
	{

	}
}

// Raytracing camera
namespace MRT
{
	void Camera::ConstructCamMatrix()
	{
		camToWorld = glm::translate(glm::fmat4(1.0f), position) * camRotation * glm::fmat4(1.0f);
	}

	bool Camera::DrawToPlane(const int& _x, const int& _y, ColorPixel _color)
	{
		// Check pixel coord is in bounds on plane
		if (_x < 0 || _x >= imageWidth || _y < 0 || _y >= imageHeight) return false;

		// Set the selected pixel on the plane to the new color
		imagePlane[(_y * imageWidth) + _x] = _color;

		return true;
	}

	void Camera::DisplayPlane()
	{
		// Loop through entire array
		for (int y = 0; y < imageHeight; ++y)
		{
			int row = y * imageWidth;
			for (int x = 0; x < imageWidth; ++x)
			{
				// Get the correct pixel to display
				int index = row + x;
				// Draw the color to the window (origin 0,0)
				MCG::DrawPixel({ x, y }, {
					imagePlane[index].r,
					imagePlane[index].g,
					imagePlane[index].b });
			}
		}

	}

	void Camera::DisplayPlanePixel(const int& _x, const int& _y)
	{
		// Check pixel coord is in bounds on plane
		if (_x < 0 || _x >= imageWidth || _y < 0 || _y >= imageHeight) return;

		int index = (_y * imageWidth) + _x;

		MCG::DrawPixel({ _x, _y }, {
			imagePlane[index].r,
			imagePlane[index].g,
			imagePlane[index].b });
	}

	void Camera::CastRay(const int& _x, const int& _y, Ray& _ray, const float& _sampleX, const float& _sampleY)
	{
		// Check pixel coord is in bounds on plane
		if (_x < 0 || _x >= imageWidth || _y < 0 || _y >= imageHeight) return;

		// Convert pixel coord to NDC space (0,1)
		float wX = (_x + _sampleX) / imageWidth, wY = (_y + _sampleY) / imageHeight;
		// Convert NDC to screen space (-1,1)
		wX = ((wX * 2) - 1) * imageAspectX * fov; wY = (1 - (wY * 2)) * imageAspectY * fov;

		// Create a 4 column vector to multiply with camToWorld
		glm::fvec4 wP(wX, wY, -1, 1);
		// Transform pixel from screen/camera space to world space
		wP = camToWorld * wP;

		// Create a 4 column vector of the ray origin
		glm::fvec4 rO(_ray.origin.x, _ray.origin.y, _ray.origin.z, 1);
		// Multiply with camToWorld, transforms origin
		rO = camToWorld * rO;

		// Set ray length to the max viewing distance
		_ray.hitInfo.length = maxViewingDistance;

		// Set origin to transformed rO (excluding w)
		_ray.origin = glm::fvec3(rO.x, rO.y, rO.z);
		// Subtract rays origin from the world space pixel coord
		_ray.direction = glm::fvec3(wP.x, wP.y, wP.z) - _ray.origin;
		// Convert ray direction into unit vector
		_ray.direction = glm::normalize(_ray.direction);

	}

	void Camera::SetFOV(float _angleDeg)
	{
		// Sets the FOV, uses trig to scale 
		// the plane in screen/camera space
		fov = glm::tan(glm::radians(_angleDeg) / 2);
	}

	void Camera::SetPosition(glm::fvec3 _position)
	{
		// Set the new position and then construct the new matrix
		position = _position;
		ConstructCamMatrix();
	}

	void Camera::SetRotation(glm::fvec3 _axis, float _angle)
	{
		// Set the new rotation and then construct the new matrix
		camRotation = glm::rotate(glm::fmat4(1.0f), glm::radians(_angle), _axis);
		ConstructCamMatrix();
	}

	void Camera::LookAt(glm::fvec3 _point)
	{
		// Get the inverse of the view matrix which gives the rotation
		camRotation = glm::inverse(glm::lookAt(position, _point, glm::fvec3(0, 1, 0)));
		ConstructCamMatrix();
	}


	Camera::Camera(const int& _pixelWidth, const int& _pixelHeight)
		:
		// Set cam matrix to an identity
		camToWorld(1.0f), camRotation(1.0f),
		position(0, 0, 0),
		imageWidth{ _pixelWidth }, imageHeight{ _pixelHeight }
	{
		// Create an array of color pixels for the image plane
		imagePlane = new ColorPixel[_pixelWidth * _pixelHeight];
		// Check that the image plane has been allocated
		fInitialised = (imagePlane != nullptr);

		if (fInitialised)
		{
			// Calculate aspect ratios
			imageAspectX = (_pixelWidth > _pixelHeight ? (float)_pixelWidth / (float)_pixelHeight : 1.f);
			imageAspectY = (_pixelHeight > _pixelWidth ? (float)_pixelHeight / (float)_pixelWidth : 1.f);
		}
	}
	Camera::~Camera()
	{
		delete[] imagePlane;
	}
}

// Primitive
namespace MRT
{
	Primitive::Primitive(glm::fvec3& _position, ColorPixel& _color)
		:
		position(_position),
		color{ _color }
	{}
	Primitive::Primitive()
		:
		position(0),
		color{ 0, 0, 0 }
	{}
}

// Sphere
namespace MRT
{
	bool Sphere::Intersect(Ray& _ray) 
	{
		// Get ray origin and direction
		glm::fvec3 rO = _ray.GetOrigin(), rD = _ray.GetDirection();

		// Calculate vector from ray origin to sphere origin
		glm::fvec3 lRO = position - rO;
		// Project lRO length onto ray direction
		float lPD = glm::dot(lRO, rD);
		// Ray wont intersect if the projected length is behind it
		if (lPD < 0) return false;

		// Get the length of the middle point of the ray to the sphere origin
		float mL = glm::dot(lRO, lRO) - (lPD * lPD);
		// Ray wont interesect if the length is greater than the radius
		if (mL > radiusSqr) return false;

		// Calculate half the length from mL mapped to the ray direction
		float sHL = glm::sqrt(radiusSqr - mL);

		// Calculate the possible starting and ending intersects
		float iStart = lPD - sHL,
			iEnd = lPD + sHL;

		float intersect = iStart;

		// If either intersection lengths are less than 0, there is no intersect
		if (iStart < 0)
		{
			intersect = iEnd;
			if (iEnd < 0) return false;
		}

		// Check if intersect length is greater than current ray length
		if (_ray.GetLength() < intersect) return false;

		// Send hit information to the ray
		HitInformation hitInfo{ intersect, glm::normalize((rO + rD * intersect) - position), color };
		_ray.SetHitInfo(hitInfo);

		return true;
	}

	Sphere::Sphere(glm::fvec3 _position, float _radius, ColorPixel _color)
		:
		Primitive(_position, _color),
		radius{ _radius }
	{
		// Precalculate the square of the radius
		radiusSqr = _radius * _radius;
	}
}

// Plane
namespace MRT
{
	float Plane::IntersectPlane(Ray& _ray)
	{
		// Get ray origin and direction
		glm::fvec3 rO = _ray.GetOrigin(), rD = _ray.GetDirection();

		// Set the ray length to 0 by default; no intersection
		float mL = 0;
		// Calculate the dot product from the ray direction and plane direction
		// Used as the denominator for solving mL
		float d = glm::dot(rD, direction);

		// If ray and plane are parallel it will most likely not be 0, a tiny value is compared against
		// to check for an intersection
		if (d > (float)1e-6)
		{
			// Subtract the ray position from the plane position
			glm::fvec3 rOpos = position - rO;
			// The direction (which is a normal) is then dot multiplied with the result
			// and divided by the denominator value to solve the ray length
			mL = glm::dot(rOpos, direction) / d;
		}

		// If an intersection occurred, the ray length will be greater than 0
		return mL;
	}

	Plane::Plane(glm::fvec3& _position, glm::fvec3& _direction, ColorPixel& _color)
		:
		Primitive(_position, _color), 
		direction(_direction)
	{
		// Normalise the direction vector just in case
		glm::normalize(direction);
	}
}

// Circle
namespace MRT
{
	bool Circle::Intersect(Ray& _ray)
	{
		// Get the ray length by checking the entire plane for an intersection
		float mL = IntersectPlane(_ray);
		// If length is 0, theres no intersection
		if (mL == 0) return false;

		// Check if intersect length is greater than current ray length
		if (_ray.GetLength() < mL) return false;

		// Get ray hit position
		glm::fvec3 rH = _ray.GetOrigin() + (_ray.GetDirection() * mL);
		// Get the length of the hit position to the circle center
		glm::fvec3 c = rH - position;

		// Get the dot product and see if the ray is projected inside the circle radius
		// keeping it squared saves performance (dont need to sqrt the dot product)
		if (glm::dot(c, c) > radiusSqr) return false;

		// Send hit information to the ray
		HitInformation hitInfo{ mL, glm::normalize(direction * rH), color };
		_ray.SetHitInfo(hitInfo);

		return true;
	}

	Circle::Circle(glm::fvec3 _position, glm::fvec3 _direction, float _radius, ColorPixel _color)
		:
		Plane(_position, _direction, _color),
		radius{ _radius }
	{
		// Precalculate the square of the radius
		radiusSqr = radius * radius;
	}
}

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

	void RayTracer::RenderScene()
	{
		// Set the background to the background color
		MCG::SetBackground({ backgroundDefault.r, backgroundDefault.g, backgroundDefault.b });

		for (int i = 0; i < primiAmount; ++i)
			std::cout << primiMap[i] << std::endl;

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
			if (y % 10 == 0)
				if (!MCG::ProcessFrame()) return;
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
		primiManager = new Primitive*[primiMax];
		primiMap = new float[primiMax]{ 0 };
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

// Scene Manager
namespace MRT
{
	void SceneManager::Run()
	{

	}

	SceneManager::SceneManager(RayTracer* _raytracer)
		:
		raytracer{ _raytracer }
	{
		// Double check that the raytracer is already instantiated
		fInitialised = (raytracer != nullptr ? raytracer->IsInit() : false);
	}
}