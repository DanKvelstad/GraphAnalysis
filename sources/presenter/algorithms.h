#pragma once

#include <utility>
#include "common.h"

std::pair<bool, point> intersecting_line_segments(point as, point at, point bs, point bt);
std::pair<point, point> intersecting_line_segment_between_states(const SkIRect& source, const SkIRect& target);