#pragma once

#include <utility>
#include "common.h"

std::pair<bool, point>  intersecting_line_segments(point as, point at, point bs, point bt);
std::pair<point, point> shortest_line_segment(const std::vector<point>& source, const std::vector<point>& target);