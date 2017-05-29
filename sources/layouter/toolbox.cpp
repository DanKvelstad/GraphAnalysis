#include "toolbox.h"

bool intersection(
	const pixel_point& ts, const pixel_point& tt,
	const pixel_point& os, const pixel_point& ot
)
{

	// https://en.wikipedia.org/wiki/line%E2%80%93line_intersection

	const auto denominator = (ts.x-tt.x)*(os.y-ot.y)-(ts.y-tt.y)*(os.x-ot.x);
	
	if(0==denominator)
	{	// "When the two lines are parallel or coincident the denominator is zero"
		
		if( ((ts.x==os.x && ts.y==os.y) && (tt.x==ot.x && tt.y==ot.y)) ||
			((ts.x==ot.x && ts.y==ot.y) && (tt.x==os.x && tt.y==os.y)) )
		{	// They are the same segment, possibly in different directions
			return false;
		}
		else if(contains(ts, tt, os))
		{	
			return true;
		}
		else if (contains(ts, tt, ot))
		{
			return true;
		}
		else if (contains(os, ot, ts))
		{
			return true;
		}
		else if (contains(os, ot, tt))
		{
			return true;
		}
		else
		{
			return false;
		}

	}
	else
	{	// " Note that the intersection point is for the infinitely long lines defined by the points, 
		//   rather than the line segments between the points, 
		//   and can produce an intersection point beyond the lengths of the line segments. "

		if (ts == os || ts == ot || tt == os || tt == ot)
		{
			return false;
		}
		else
		{

			pixel_point point{
				static_cast<float>((ts.x*tt.y - ts.y*tt.x)*(os.x - ot.x) - (ts.x - tt.x)*(os.x*ot.y - os.y*ot.x)) / denominator,
				static_cast<float>((ts.x*tt.y - ts.y*tt.x)*(os.y - ot.y) - (ts.y - tt.y)*(os.x*ot.y - os.y*ot.x)) / denominator
			};

			return contains(ts, tt, point) && contains(os, ot, point);

		}
	}

}

bool contains(pixel_point s, pixel_point t, pixel_point p)
{

	float cross_product((p.y - s.y)*(t.x - s.x) - (p.x - s.x)*(t.y - s.y));

	if (s.x == p.x && s.y == p.y)
	{	// if p is source
		return true;
	}
	else if (t.x == p.x && t.y == p.y)
	{	// if p is target
		return true;
	}
	else if (!(0.1>cross_product&&-0.1<cross_product))
	{	// if p is not on the line
		return false;
	}
	else
	{	// else p is betw<een source and target

		// http://stackoverflow.com/a/328122

		auto dot_product    = (p.x - s.x)*(t.x - s.x) + (p.y - s.y)*(t.y - s.y);
		auto squared_length = (t.x - s.x)*(t.x - s.x) + (t.y - s.y)*(t.y - s.y);

		if (0.0 <= dot_product && dot_product <= squared_length)
		{
			return true;
		}
		else
		{
			return false;
		}

	}

}