#include "Ray.h"

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