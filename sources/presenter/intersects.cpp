#include <utility>
#include "common.h"
#include "states.h"
#include "SkRect.h"
#include <algorithm>

bool contains(point s, point t, point p)
{

	auto equals(
		[](point a, point b) -> bool
		{
			auto equals(
				[](scalar c, scalar d) -> bool
				{
					return c < (d + 1) && (c + 1) < d;
				}
			);
			return equals(a.x, b.x) && equals(a.y, b.y);
		}
	);

	auto is_zero(
		[](scalar n) -> bool
		{
			return n < 100 && n > -100;
		}
	);

	if (equals(s, p))
	{	// p is s
		return true;
	}
	else if (equals(t, p))
	{	// t is p
		return true;
	}
	else if (is_zero((p.y-s.y)*(t.x-s.x)-(p.x-s.x)*(t.y-s.y)))
	{	// p is on the line, the cross product is zero

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
	else
	{	// else p is not on the line
		return false;
	}

}

std::pair<bool, point> intersecting_line_segments(point as, point at, point bs, point bt)
{

	auto result = std::make_pair(
		false,
		point{ static_cast<int>(-1), static_cast<int>(-1) }
	);;

	// https://en.wikipedia.org/wiki/line%E2%80%93line_intersection

	const float denominator = static_cast<float>(
		(as.x-at.x)*(bs.y-bt.y)-(as.y-at.y)*(bs.x-bt.x)
	);

	auto is_zero(
		[](float n) -> bool
		{
			const float epsilon(0.001f);
			return n < epsilon && n > -1*epsilon;
		}
	);
	
	if(!is_zero(denominator))
	{	// They are not parallel

		result.second.x = static_cast<scalar>(
			roundf(
				( (as.x*at.y - as.y*at.x)*(bs.x-bt.x) -
				  (as.x-at.x)*(bs.x*bt.y - bs.y*bt.x) )
				/ denominator
			)
		);
		result.second.y = static_cast<scalar>(
			roundf(
				( (as.x*at.y - as.y*at.x)*(bs.y-bt.y) - 
				  (as.y-at.y)*(bs.x*bt.y - bs.y*bt.x) )
				/ denominator
			)
		);

		// " Note that the intersection point is for the infinitely long lines defined 
		//   by the points, rather than the line segments between the points, and can 
		//   produce an intersection point beyond the lengths of the line segments. "

		result.first =	contains(as, at, result.second) &&
						contains(bs, bt, result.second) ;
		
	}

	return result;

}

std::pair<point, point> shortest_line_segment(const std::vector<point>& source, const std::vector<point>& target)
{

	auto shortest_length(std::numeric_limits<double>::max());
	std::vector<std::pair<point, point>> shortest;

	std::for_each(
		source.begin(), source.end(),
		[&](const point& s)
		{
			std::for_each(
				target.begin(), target.end(),
				[&](const point& t)
				{
					auto diff_x(t.x-s.x);
					auto diff_y(t.y-s.y);
					auto current_length(std::sqrt(diff_x*diff_x+diff_y*diff_y));
					if (current_length == shortest_length)
					{
						shortest.emplace_back(s, t);
					}
					else if (current_length < shortest_length)
					{
						shortest.clear();
						shortest.emplace_back(s, t);
						shortest_length = current_length;
					}
				}
			);
		}
	);

	std::pair<point, point> result;
	std::for_each(
		shortest.begin(), shortest.end(),
		[&](const std::pair<point, point>& element)
		{
			result.first.x  += element.first.x;
			result.first.y  += element.first.y;
			result.second.x += element.second.x;
			result.second.y += element.second.y;
		}
	);
	result.first.x  /= static_cast<unsigned>(shortest.size());
	result.first.y  /= static_cast<unsigned>(shortest.size());
	result.second.x /= static_cast<unsigned>(shortest.size());
	result.second.y /= static_cast<unsigned>(shortest.size());

	return result;

}