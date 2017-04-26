#pragma once

#include "common.h"

bool contains(pixel_point line_source, pixel_point line_target, pixel_point point);
bool intersection(
	grid_point line1_source, grid_point line1_target,
	grid_point line2_source, grid_point line2_target
);