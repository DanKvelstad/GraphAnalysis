#pragma once

#include "dllexport.h"
#include <algorithm>

typedef float pixel_scalar;
struct DLLEXPORT pixel_point
{

	pixel_scalar x;
	pixel_scalar y;

	bool operator==(const pixel_point& other) const
	{
		auto comp(
			[](float a, float b) -> bool
			{
				const float epsilon(0.001f);
				return fabs(a - b) < epsilon;
			}
		);
		return comp(x, other.x) && comp(y, other.y);
	}

};

typedef unsigned grid_scalar;
struct DLLEXPORT grid_point
{

	grid_scalar x;
	grid_scalar y;

	operator pixel_point() const
	{
		return pixel_point{ 
			static_cast<pixel_scalar>(x), 
			static_cast<pixel_scalar>(y) 
		};
	}

};