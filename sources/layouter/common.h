#pragma once

#include "dllexport.h"

typedef unsigned grid_scalar;
struct DLLEXPORT grid_point
{
	grid_scalar x;
	grid_scalar y;
};

typedef float pixel_scalar;
struct DLLEXPORT pixel_point
{
	pixel_point(pixel_scalar x=0, pixel_scalar y=0)
	: x(x)
	, y(y)
	{
	}
	pixel_scalar x;
	pixel_scalar y;
};