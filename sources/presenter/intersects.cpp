#include <utility>
#include "common.h"
#include "states.h"
#include "SkRect.h"

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

std::pair<point, point> intersecting_line_segment_between_states(const SkIRect& source, const SkIRect& target)
{

	// todo: i dont really want the intersection, i want the two points 
	//       where the distance between them is the smallest, that would 
	//       improve the visual impression.

	auto helper(
		[](const SkIRect& source, point center_of_source, point center_of_target)
		{
		
			{	// does the line intersect with the left line segment of source
				auto result = intersecting_line_segments(
					point{ source.left(), source.bottom()	},
					point{ source.left(), source.top()		},
					center_of_source,
					center_of_target
				);
				if (result.first)
				{
					return result.second;
				}
			}
			{	// does the line intersect with the top line segment of source
				auto result = intersecting_line_segments(
					point{ source.left(),	source.top()		},
					point{ source.right(),	source.top()		},
					center_of_source,
					center_of_target
				);
				if (result.first)
				{
					return result.second;
				}
			}
			{	// does the line intersect with the right line segment of source
				auto result = intersecting_line_segments(
					point{ source.right(),	source.top()	},
					point{ source.right(),	source.bottom() },
					center_of_source,
					center_of_target
				);
				if (result.first)
				{
					return result.second;
				}
			}
			{	// // does the line intersect with the bottom line segment of source
				auto result = intersecting_line_segments(
					point{ source.right(),	source.bottom()	},
					point{ source.left(),	source.bottom()	},
					center_of_source,
					center_of_target
				);
				if (result.first)
				{
					return result.second;
				}
			}
		
			throw std::logic_error("Line between states does not intersect with source state");

		}
	);

	auto center_of_source(point{ source.centerX(), source.centerY() });
	auto center_of_target(point{ target.centerX(), target.centerY() });

	return std::make_pair(
		helper(source, center_of_source, center_of_target),
		helper(target, center_of_target, center_of_source)
	);
	
}