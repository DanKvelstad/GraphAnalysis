#pragma once

#include "dllexport.h"
#include "common.h"

enum class intersection_status
{
	intersects_between_endpoints,
	intersects_at_endpoint,
	does_not_intersect
};

struct DLLEXPORT intersection_results
{
	intersection_status	status;
	pixel_point			point;
};

DLLEXPORT intersection_status operator||(intersection_status a, intersection_status b);
DLLEXPORT intersection_status operator&&(intersection_status a, intersection_status b);