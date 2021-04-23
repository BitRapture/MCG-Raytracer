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

	void Camera::TraceRay(const int& _x, const int& _y, Ray& _ray, const float& _sampleX, const float& _sampleY)
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

		// Set origin to transformed rO (excluding w)
		_ray.origin = glm::fvec3(rO.x, rO.y, rO.z);
		// Subtract rays origin from the world space pixel coord
		_ray.direction = glm::fvec3(wP.x, wP.y, wP.z) - _ray.origin;
		// Convert ray direction into unit vector
		_ray.direction = glm::normalize(_ray.direction);

		//DrawToPlane(_x, _y, { (float)_y / imageHeight, (float)_x / imageWidth, 1 - ((float)_y / imageHeight) });

		//std::cout << _ray.direction.x << " " << _ray.direction.y << " " << _ray.direction.z << std::endl;
	}

	void Camera::SetFOV(float _angleDeg)
	{
		// Sets the FOV, uses trig to scale 
		// the plane in screen/camera space
		fov = glm::tan(glm::radians(_angleDeg) / 2);
	}

	bool Camera::IsInit()
	{
		// Return init flag
		return fInitialised;
	}

	Camera::Camera(const int& _pixelWidth, const int& _pixelHeight)
		:
		// Set cam matrix to an identity
		camToWorld(1.0f),
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
	Primitive::Primitive(glm::fvec3& _position)
		:
		position(_position)
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
		float sHL = glm::sqrt(mL - radiusSqr);

		// Calculate the possible starting and ending intersects
		float iStart = lPD - sHL,
			iEnd = lPD + sHL;

		if (iStart < 0)
		{
			if (iEnd < 0) return false;
		}

		return true;
	}

	Sphere::Sphere(glm::fvec3 _position, float _radius)
		:
		Primitive(_position),
		radius{ _radius }
	{
		radiusSqr = _radius * _radius;
	}
}