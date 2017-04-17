#include "area_rectangle.h"
#include <algorithm>

area_rectangle::area_rectangle(void)
{
	// needed to export dll
}

area_rectangle::area_rectangle(grid_point grid)
{

	auto width(100);
	auto half_width(width / 2);

	auto height(50);
	auto half_height(height / 2);
	
	auto spacer(std::min(width, height));

	if (0<grid.x)
	{
		center.x =	static_cast<pixel_scalar>(
						grid.x*width  +	// Previous Areas
						grid.x*spacer +	// Spacing between areas
						width / 2		// Offset to center
					);
	}
	else
	{
		center.x = static_cast<pixel_scalar>(width/2); // Offset to center
	}

	if (0<grid.y)
	{
		center.y =	static_cast<pixel_scalar>(
						grid.y*height +	// Previous Areas
						grid.y*spacer +	// Spacing between areas
						height / 2		// Offset to center
					);
	}
	else
	{
		center.y = static_cast<pixel_scalar>(height/2);	// Offset to center
	}

	top = line(
		pixel_point{center.x-half_width, center.y-half_height},
		pixel_point{center.x+half_width, center.y-half_height}
	);
	right = line(
		pixel_point{center.x+half_width, center.y-half_height},
		pixel_point{center.x+half_width, center.y+half_height}
	);
	bottom = line(
		pixel_point{center.x+half_width, center.y+half_height},
		pixel_point{center.x-half_width, center.y+half_height}
	);
	left = line(
		pixel_point{center.x-half_width, center.y-half_height},
		pixel_point{center.x-half_width, center.y+half_height}
	);

}

area_rectangle::~area_rectangle()
{
}

pixel_point area_rectangle::get_center(void) const
{
	return center;
}

intersection_results area_rectangle::intersect(const shape& s) const
{

	auto intersection_top = top.intersect(s);
	if(intersection_status::does_not_intersect!=intersection_top.status)
	{
		return intersection_top;
	}
	
	auto intersection_right = right.intersect(s);
	if(intersection_status::does_not_intersect!=intersection_right.status)
	{
		return intersection_right;
	}
	
	auto intersection_bottom = bottom.intersect(s);
	if(intersection_status::does_not_intersect!=intersection_bottom.status)
	{
		return intersection_bottom;
	}

	auto intersection_left = left.intersect(s);
	if (intersection_status::does_not_intersect != intersection_left.status)
	{
		return intersection_left;
	}

	return intersection_results{
		intersection_status::does_not_intersect,
		pixel_point{ -1.0f, -1.0f }
	};

}

intersection_status area_rectangle::contains(pixel_point point) const
{
	
	auto intersection_top = top.contains(point);
	if(intersection_status::does_not_intersect!=intersection_top)
	{
		return intersection_top;
	}
	
	auto intersection_right = right.contains(point);
	if(intersection_status::does_not_intersect!=intersection_right)
	{
		return intersection_right;
	}
	
	auto intersection_bottom = bottom.contains(point);
	if(intersection_status::does_not_intersect!=intersection_bottom)
	{
		return intersection_bottom;
	}

	auto intersection_left = left.contains(point);
	if (intersection_status::does_not_intersect!=intersection_left)
	{
		return intersection_left;
	}

	return intersection_status::does_not_intersect;

}
