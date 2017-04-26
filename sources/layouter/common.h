#pragma once

#include "dllexport.h"

typedef float pixel_scalar;
struct DLLEXPORT pixel_point
{
	pixel_scalar x;
	pixel_scalar y;
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