#include "line.h"
#include "common.h"

line::line(void)
{
}

line::line(pixel_point source, pixel_point target)
: source(source)
, target(target)
{
}

line::~line()
{
}

void line::update(pixel_point new_source, pixel_point new_target)
{
	source = new_source;
	target = new_target;
}

pixel_point line::get_source(void) const
{
	return source;
}

pixel_point line::get_target(void) const
{
	return target;
}

auto line::get_endcap(void) const -> std::vector<pixel_point>
{

	const float pi(3.141592653589793238462643383279502884f);
	const float offset_to_line_angle(pi / 4);
	const float length(10);

	float angle(atan2(target.y - source.y, target.x - source.x));

	std::vector<pixel_point> endcap;

	endcap.emplace_back(
		target.x - cosf(angle - offset_to_line_angle)*length,
		target.y - sinf(angle - offset_to_line_angle)*length
	);

	endcap.emplace_back(target.x, target.y);

	endcap.emplace_back(
		target.x - cosf(angle + offset_to_line_angle)*length,
		target.y - sinf(angle + offset_to_line_angle)*length
	);

	return endcap;

}

void line::cut_source(const shape& other) 
{

	auto intersection(other.intersect(*this));
	if (intersection_status::does_not_intersect != intersection.status)
	{
		source = intersection.point;
	}

}

void line::cut_target(const shape& other)
{

	auto intersection(other.intersect(*this));
	if (intersection_status::does_not_intersect != intersection.status)
	{
		target = intersection.point;
	}

}

intersection_results line::intersect(const shape& s) const
{

	// This Source / Target
	auto ts(get_target());
	auto tt(get_source());

	// todo will crasch if s is a rectangle
	auto l(dynamic_cast<const line&>(s));

	// Other Source / Target
	auto os(l.get_source());
	auto ot(l.get_target());

	intersection_results results;

	// https://en.wikipedia.org/wiki/line%E2%80%93line_intersection

	const auto denominator = (ts.x-tt.x)*(os.y-ot.y)-(ts.y-tt.y)*(os.x-ot.x);
	
	if(0==denominator)
	{	// "When the two lines are parallel or coincident the denominator is zero"

		if( ((ts.x==os.x && ts.y==os.y) && (tt.x==ot.x && tt.y==ot.y)) ||
			((ts.x==ot.x && ts.y==ot.y) && (tt.x==os.x && tt.y==os.y)) )
		{	// They are the same segment, possibly in different directions
			
			results.status = intersection_status::intersects_between_endpoints;

		}
		else
		{	// Evaluate if the segments overlapp

			results.status =   contains(os) ||   contains(ot) ||
							 s.contains(ts) || s.contains(tt) ;

		}

	}
	else
	{	// " Note that the intersection point is for the infinitely long lines defined by the points, 
		//   rather than the line segments between the points, 
		//   and can produce an intersection point beyond the lengths of the line segments. "

		results.point = pixel_point{
			static_cast<float>((ts.x*tt.y - ts.y*tt.x)*(os.x-ot.x) - (ts.x-tt.x)*(os.x*ot.y - os.y*ot.x)) / denominator,
			static_cast<float>((ts.x*tt.y - ts.y*tt.x)*(os.y-ot.y) - (ts.y-tt.y)*(os.x*ot.y - os.y*ot.x)) / denominator
		};

		results.status = contains(results.point) && s.contains(results.point) ;

	}

	return results;

}

auto line::contains(pixel_point p) const -> intersection_status
{

	auto s(get_target());
	auto t(get_source());

	float cross_product((p.y - s.y)*(t.x - s.x) - (p.x - s.x)*(t.y - s.y));

	if (s.x == p.x && s.y == p.y)
	{	// Charlie is Alpha
		return intersection_status::intersects_at_endpoint;
	}
	else if (t.x == p.x && t.y == p.y)
	{	// Charlie is Bravo
		return intersection_status::intersects_at_endpoint;
	}
	else if (!(0.1>cross_product&&-0.1<cross_product))
	{	// Charlie is not on the line
		return intersection_status::does_not_intersect;
	}
	else
	{	// Evaluate if Charlie is between Alpha and Bravo

		// http://stackoverflow.com/a/328122

		auto dot_product    = (p.x - s.x)*(t.x - s.x) + (p.y - s.y)*(t.y - s.y);
		auto squared_length = (t.x - s.x)*(t.x - s.x) + (t.y - s.y)*(t.y - s.y);

		if (0.0 <= dot_product && dot_product <= squared_length)
		{
			return intersection_status::intersects_between_endpoints;
		}
		else
		{
			return intersection_status::does_not_intersect;
		}

	}

}

bool line::operator==(const line& other) const
{
	return	source.x == other.source.x	&&
			source.y == other.source.y	&&
			target.x == other.target.x	&&
			target.y == other.target.y	;
}