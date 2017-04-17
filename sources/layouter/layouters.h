#pragma once

#include "dllexport.h"
#include "common.h"
#include "area_rectangle.h"
#include "line.h"
#include <vector>
#include <utility>

#pragma warning(disable:4251)  
struct DLLEXPORT layout
{

	std::vector<area_rectangle> states;
	std::vector<line>			edges;

	unsigned intersections;
	unsigned workspace_size;

};
#pragma warning(default:4251)

DLLEXPORT layout layouter_brute(
	unsigned									state_count, 
	std::vector<std::pair<unsigned, unsigned>>	edges,
	grid_point									grid_dimensions
);