#pragma once

#include "dllexport.h"
#include "shape.h"
#include "intersection.h"
#include "line.h"

class DLLEXPORT area_rectangle : public shape
{

public:

	area_rectangle(void);
	area_rectangle(grid_point grid);
	~area_rectangle(void);

	pixel_point get_center(void) const;

	intersection_results intersect(const shape& s) const override;
	intersection_status  contains(pixel_point point) const;

private:

	pixel_point center;

	line top;
	line right;
	line bottom;
	line left;

};

