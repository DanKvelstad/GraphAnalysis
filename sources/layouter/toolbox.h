#pragma once

#include "common.h"

bool contains(pixel_point line_source, pixel_point line_target, pixel_point point);
bool intersection(
	const pixel_point& line1_source, const pixel_point& line1_target,
	const pixel_point& line2_source, const pixel_point& line2_target
);