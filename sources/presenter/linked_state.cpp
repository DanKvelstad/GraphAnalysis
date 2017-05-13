#include "linked_state.h"
#include "SkCanvas.h"
#include "SkTextBox.h"
#include <algorithm>

linked_state::linked_state(unsigned index)
: index(index)
, width(100)
, height(50)
, spacing(std::max(width, height))
, data(nullptr)
, linked(nullptr)
{
}

linked_state::linked_state(const linked_state& other)
: index(other.index)
, width(other.width)
, height(other.height)
, spacing(other.spacing)
{
	if (nullptr != other.linked)
	{
		linked = new linked_state(*other.linked);
	}
	else
	{
		linked = nullptr;
	}
	if (nullptr != other.data)
	{
		data = new state_data(*other.data);
	}
	else
	{
		linked = nullptr;
	}
}

linked_state::~linked_state()
{
}

void linked_state::emplace(const char* name, int x, int y)
{

	if (nullptr == data)
	{
		data = new state_data(
			name,
			x*width  + (x+1)*spacing,
			y*height + (y+1)*spacing,
			width,
			height
		);
	}
	else if (nullptr == linked)
	{
		linked = new linked_state(index+1);
		linked->emplace(name, x, y);
	}
	else
	{
		linked->emplace(name, x, y);
	}

}

linked_state* linked_state::next(void) 
{
	
	return linked;

}

const linked_state& linked_state::at(unsigned i) const
{
	if (0 == i)
	{
		return *this;
	}
	else
	{
		return linked->at(i-1);
	}
}

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

std::pair<bool, point> intersecting_lines(point as, point at, point bs, point bt)
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

point linked_state::intersection(const linked_state& target) const
{

	auto center_of_source(data->center);
	auto center_of_target(target.data->center);

	{	// does the line intersect with the left line segment of source
		auto result = intersecting_lines(
			point{ data->left,	data->bottom	},
			point{ data->left,	data->top		},
			center_of_source,
			center_of_target
		);
		if (result.first)
		{
			return result.second;
		}
	}
	{	// does the line intersect with the top line segment of source
		auto result = intersecting_lines(
			point{ data->left,	data->top		},
			point{ data->right,	data->top		},
			center_of_source,
			center_of_target
		);
		if (result.first)
		{
			return result.second;
		}
	}
	{	// does the line intersect with the right line segment of source
		auto result = intersecting_lines(
			point{ data->right,	data->top		},
			point{ data->right,	data->bottom	},
			center_of_source,
			center_of_target
		);
		if (result.first)
		{
			return result.second;
		}
	}
	{	// // does the line intersect with the bottom line segment of source
		auto result = intersecting_lines(
			point{ data->right,	data->bottom	},
			point{ data->left,	data->bottom	},
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

const linked_state::state_data& linked_state::get() const
{
	return *data;
}

unsigned linked_state::get_spacing(void) const
{
	return spacing;
}

void linked_state::draw(SkCanvas* canvas) const
{

	SkPaint paint_shape;
	paint_shape.setStyle(SkPaint::kStroke_Style);
	paint_shape.setAntiAlias(true);
	paint_shape.setStrokeWidth(4);
	paint_shape.setColor(SK_ColorBLACK);

	canvas->drawRoundRect(
		SkRect::MakeLTRB(
			static_cast<SkScalar>(data->left+5), 
			static_cast<SkScalar>(data->top+5),
			static_cast<SkScalar>(data->right-5), 
			static_cast<SkScalar>(data->bottom-5)
		),
		10,
		10,
		paint_shape
	);
	
	SkPaint paint_text;
	paint_text.setAntiAlias(true);
	paint_text.setStrokeWidth(4);
	paint_text.setColor(SK_ColorBLACK);
	paint_text.setTextSize(22.f);
	paint_text.setTextAlign(SkPaint::Align::kCenter_Align);

	SkTextBox text_box;
	text_box.setBox(
		static_cast<SkScalar>(data->left),
		static_cast<SkScalar>(data->top),
		static_cast<SkScalar>(data->right),
		static_cast<SkScalar>(data->bottom)
	); 
	text_box.setText(data->name, strlen(data->name), paint_text);
	text_box.setSpacingAlign(SkTextBox::SpacingAlign::kCenter_SpacingAlign);
	text_box.draw(canvas);

	if (linked)
	{
		linked->draw(canvas);
	}

}