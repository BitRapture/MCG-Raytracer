#include "Primitive.h"

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