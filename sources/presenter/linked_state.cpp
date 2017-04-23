#include "linked_state.h"
#include "SkCanvas.h"
#include "SkTextBox.h"
#include <algorithm>

linked_state::linked_state(std::string name, int x, int y)
: width(100)
, height(50)
, spacing(std::min(width, height))
, name(name)
, left(x*(width +spacing)+spacing)
, top(y*(height+spacing)+spacing)
, right(left+width)
, bottom(top+height)
, center(point{ left+width/2, top+height/2 })
{
}

linked_state::~linked_state()
{
}

void linked_state::emplace(std::string name, int x, int y)
{

	if (linked)
	{
		linked->emplace(name, x, y);
	}
	else
	{
		linked = std::make_unique<linked_state>(name, x, y);
	}

}

linked_state* linked_state::next(void) 
{
	
	return linked.get();

}

bool contains(point s, point t, point p)
{

	scalar cross_product((p.y - s.y)*(t.x - s.x) - (p.x - s.x)*(t.y - s.y));

	if (s.x == p.x && s.y == p.y)
	{	// Charlie is Alpha
		return true;
	}
	else if (t.x == p.x && t.y == p.y)
	{	// Charlie is Bravo
		return true;
	}
	else if (!(0.1>cross_product&&-0.1<cross_product))
	{	// Charlie is not on the line
		return false;
	}
	else
	{	// Evaluate if Charlie is between Alpha and Bravo

		// http://stackoverflow.com/a/328122

		auto dot_product = (p.x - s.x)*(t.x - s.x) + (p.y - s.y)*(t.y - s.y);
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

std::pair<bool, point> intersecting_lines(point ts, point tt, point os, point ot)
{

	std::pair<bool, point> result;

	// https://en.wikipedia.org/wiki/line%E2%80%93line_intersection

	const float denominator = static_cast<float>(
		(ts.x-tt.x)*(os.y-ot.y)-(ts.y-tt.y)*(os.x-ot.x)
	);
	
	if(0==denominator)
	{	// "When the two lines are parallel or coincident the denominator is zero"
		// this is not what we are searching for here, so just sendback a gravestone
		result = std::make_pair(
			false,
			point{static_cast<int>(-1), static_cast<int>(-1)}
		);
	}
	else
	{	// They are not parallel

		point intersection_point{
			static_cast<scalar>(
				std::round(
					( (ts.x*tt.y - ts.y*tt.x)*(os.x-ot.x) - 
					  (ts.x-tt.x)*(os.x*ot.y - os.y*ot.x) )
					/ denominator
				)
			),
			static_cast<scalar>(
				std::round(
					( (ts.x*tt.y - ts.y*tt.x)*(os.y-ot.y) - 
					  (ts.y-tt.y)*(os.x*ot.y - os.y*ot.x) )
					/ denominator
				)
			)
		};

		// " Note that the intersection point is for the infinitely long lines defined 
		//   by the points, rather than the line segments between the points, and can 
		//   produce an intersection point beyond the lengths of the line segments. "

		if (contains(ts, tt, intersection_point) &&
			contains(os, ot, intersection_point))
		{
			result = std::make_pair(
				true,
				intersection_point
			);
		}
		else
		{
			result = std::make_pair(
				false,
				point{ static_cast<int>(-1), static_cast<int>(-1) }
			);
		}

	}

	return result;

}

point linked_state::intersection(const linked_state& target) const
{

	point os(center);
	point ot(target.center);

	{	// left
		auto result = intersecting_lines(
			point{ left,	bottom	},
			point{ left,	top		},
			os,
			ot
		);
		if (result.first)
		{
			return result.second;
		}
	}
	{	// top
		auto result = intersecting_lines(
			point{ left,	top		},
			point{ right,	top		},
			os,
			ot
		);
		if (result.first)
		{
			return result.second;
		}
	}
	{	// right
		auto result = intersecting_lines(
			point{ right,	top		},
			point{ right,	bottom	},
			os,
			ot
		);
		if (result.first)
		{
			return result.second;
		}
	}
	{	// bottom
		auto result = intersecting_lines(
			point{ right,	bottom	},
			point{ left,	bottom	},
			os,
			ot
		);
		if (result.first)
		{
			return result.second;
		}
	}

	throw std::exception();

}

void linked_state::draw(SkCanvas* canvas) const
{

	SkPaint paint_shape;
	paint_shape.setStyle(SkPaint::kStroke_Style);
	paint_shape.setAntiAlias(true);
	paint_shape.setStrokeWidth(4);
	paint_shape.setColor(SK_ColorBLUE);

	canvas->drawRect(
		SkRect::MakeLTRB(
			static_cast<SkScalar>(left), 
			static_cast<SkScalar>(top),
			static_cast<SkScalar>(right), 
			static_cast<SkScalar>(bottom)
		),
		paint_shape
	);
	

	SkPaint paint_text;
	paint_text.setAntiAlias(true);
	paint_text.setStrokeWidth(4);
	paint_text.setColor(SK_ColorMAGENTA);
	paint_text.setTextSize(22.f);
	paint_text.setTextAlign(SkPaint::Align::kCenter_Align);

	SkTextBox text_box;
	text_box.setBox(
		static_cast<SkScalar>(left),
		static_cast<SkScalar>(top),
		static_cast<SkScalar>(right),
		static_cast<SkScalar>(bottom)
	); 
	text_box.setText(name.c_str(), name.length(), paint_text);
	text_box.setSpacingAlign(SkTextBox::SpacingAlign::kCenter_SpacingAlign);
	text_box.draw(canvas);

	if (linked)
	{
		linked->draw(canvas);
	}

}