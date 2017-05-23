#pragma once

#include "common.h"

class SkRegion;
class SkCanvas;

class shape
{

public:

	enum class endpoint_select{ left, top_left, top, top_right, right, bottom_right, bottom, bottom_left };
	enum class endpoint_duplex{ single, source, target };
	virtual point get_endpoint(const SkRegion& region, endpoint_select select, endpoint_duplex endpoint) const = 0;

	virtual void draw(SkCanvas& canvas, const SkRegion& region) = 0;

};