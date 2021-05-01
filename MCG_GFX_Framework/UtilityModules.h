#ifndef _UTILITY_MODULES_H_
#define _UTILITY_MODULES_H_

namespace MRT
{
	// ColorPixel
	// - A simple structure containing 3 floats for RGB color
	// - Each color is normalised (0 to 1.f) 
	struct ColorPixel
	{
		float r{ 0 }, g{ 0 }, b{ 0 };
	};

}

#endif // !_UTILITY_MODULES_H_