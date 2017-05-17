#pragma once

#include <string>
#include <vector>
#include "common.h"
#include "SkPaint.h"

class SkCanvas;
class SkRegion;

class state
{

public:

	state(const std::string& name, unsigned x, unsigned y);
	state(const state& other);
	~state(void);

	unsigned get_x(void) const;
	unsigned get_y(void) const;

	std::pair<unsigned, unsigned> get_text_dimensions(void);

	std::vector<point> get_single_endpoints(const SkRegion& region) const;
	std::vector<point> get_double_endpoints_source(const SkRegion& region) const;
	std::vector<point> get_double_endpoints_target(const SkRegion& region) const;

	void draw(SkCanvas& canvas, const SkRegion& region);

private:

	const std::string name;
	const unsigned x;
	const unsigned y;

	SkPaint paint_shape;
	SkPaint paint_text;

};