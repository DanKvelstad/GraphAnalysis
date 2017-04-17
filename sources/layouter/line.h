#pragma once

#include "dllexport.h"
#include "shape.h"
#include "common.h"
#include "intersection.h"
#include <vector>

class DLLEXPORT line : public shape
{

public:

	line(void);
	line(pixel_point source, pixel_point target);
	~line();

	void update(pixel_point new_source, pixel_point new_target);

	auto get_source(void) const -> pixel_point;
	auto get_target(void) const -> pixel_point;
	auto get_endcap(void) const -> std::vector<pixel_point>;

	void cut_source(const shape& other);
	void cut_target(const shape& other);

	intersection_results intersect(const shape& other) const override;
	intersection_status  contains(pixel_point point) const;

	bool operator==(const line& other) const;

private:

	pixel_point source;
	pixel_point target;

};

