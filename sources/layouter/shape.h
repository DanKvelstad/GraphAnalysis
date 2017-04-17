#pragma once

#include "dllexport.h"
#include "common.h"
#include "intersection.h"

class DLLEXPORT shape
{

public:

	virtual ~shape(void) {};
	virtual intersection_results intersect(const shape& other) const = 0;
	virtual intersection_status  contains(pixel_point point)   const = 0;

};