#pragma once

#include <string>
#include <vector>
#include "common.h"

#include "SkPaint.h"
#include "SkRegion.h"
#include "shape.h"
#include <map>

class SkCanvas;
class SkRegion;

class state : shape
{

public:

	state(const std::string& name, unsigned x, unsigned y);
	state(const state& other);
	~state(void);

	unsigned get_x(void) const;
	unsigned get_y(void) const;

	std::pair<unsigned, unsigned> get_text_dimensions(void);

	point get_endpoint(const SkRegion& region, endpoint_select select, endpoint_duplex endpoint) const override;

	void draw(SkCanvas& canvas, const SkRegion& region) override;

private:

	const std::string name;
	const unsigned x;
	const unsigned y;

	SkPaint paint_shape;
	SkPaint paint_text;

	SkRegion assigned_region;
	std::map<shape::endpoint_select, point> map_single_endpoint;

};