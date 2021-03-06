#include "state.h"
#include "SkCanvas.h"
#include "SkTextBox.h"
#include "SkRegion.h"

state::state(const std::string& name, unsigned x, unsigned y)
: name(name)
, x(x)
, y(y)
{

	paint_text.setAntiAlias(true);
	paint_text.setStrokeWidth(4);
	paint_text.setColor(SK_ColorBLACK);
	paint_text.setTextSize(22.f);
	paint_text.setTextAlign(SkPaint::Align::kCenter_Align);

	paint_shape.setStyle(SkPaint::kStroke_Style);
	paint_shape.setAntiAlias(true);
	paint_shape.setStrokeWidth(4);
	paint_shape.setColor(SK_ColorBLACK);

}

state::state(const state & other)
: name(other.name)
, x(other.x)
, y(other.y)
, paint_shape(other.paint_shape)
, paint_text(other.paint_text)
{
}

state::~state(void)
{
}

unsigned state::get_x(void) const
{
	return x;
}

unsigned state::get_y(void) const
{
	return y;
}

std::pair<unsigned, unsigned> state::get_text_dimensions(void)
{
	SkRect bounds;
	paint_text.measureText(name.c_str(), name.length(), &bounds);
	return std::make_pair(
		static_cast<unsigned>(bounds.width()),
		static_cast<unsigned>(bounds.height())
	);
}


point get_single_endpoints(const SkRegion& region, shape::endpoint_select select)
{

	auto margin(region.getBounds());

	const float pi(3.141592653589793238462643383279502884f);

	switch (select)
	{
	case shape::endpoint_select::left:
		return point{ margin.left(), margin.centerY() };
	case shape::endpoint_select::top_left:
		return point{ 
			static_cast<scalar>((margin.left()+(10+5)) + (10+5)*std::cos(pi+pi/4)), 
			static_cast<scalar>((margin.top() +(10+5)) + (10+5)*std::sin(pi+pi/4))
		};
	case shape::endpoint_select::top:
		return point{ margin.centerX(), margin.top() };
	case shape::endpoint_select::top_right:
		return point{ 
			static_cast<scalar>((margin.right()-(10+5)) + (10+5)*std::cos(pi+3*pi/4)), 
			static_cast<scalar>((margin.top()  +(10+5)) + (10+5)*std::sin(pi+3*pi/4))
		};
	case shape::endpoint_select::right:
		return point{ margin.right(), margin.centerY() };
	case shape::endpoint_select::bottom_right:
		return point{ 
			static_cast<scalar>((margin.right() -(10+5)) + (10+5)*std::cos(pi/4)), 
			static_cast<scalar>((margin.bottom()-(10+5)) + (10+5)*std::sin(pi/4))
		};
	case shape::endpoint_select::bottom:
		return point{ margin.centerX(), margin.bottom() };
	case shape::endpoint_select::bottom_left:
		return point{ 
			static_cast<scalar>((margin.left()  +(10+5)) + (10+5)*std::cos(3*pi/4)), 
			static_cast<scalar>((margin.bottom()-(10+5)) + (10+5)*std::sin(3*pi/4))
		};
	default:
		throw std::exception();
	}

}

point get_double_endpoints_source(const SkRegion& region, shape::endpoint_select select)
{

	auto margin(region.getBounds());

	switch (select)
	{
	case shape::endpoint_select::left:
		return point{ margin.left(), margin.centerY() + 10 };
	case shape::endpoint_select::top_left:
		return point{ margin.left(), margin.top() + 10 };
	case shape::endpoint_select::top:
		return point{ margin.centerX() - 10, margin.top() };
	case shape::endpoint_select::top_right:
		return point{ margin.right() - 10, margin.top() };
	case shape::endpoint_select::right:
		return point{ margin.right(), margin.centerY() - 10 };
	case shape::endpoint_select::bottom_right:
		return point{ margin.right(), margin.bottom() - 10 };
	case shape::endpoint_select::bottom:
		return point{ margin.centerX() + 10  , margin.bottom() };
	case shape::endpoint_select::bottom_left:
		return point{ margin.left() + 10, margin.bottom() };
	default:
		throw std::exception();
	}

}

point get_double_endpoints_target(const SkRegion& region, shape::endpoint_select select)
{

	auto margin(region.getBounds());
	
	switch (select)
	{
	case shape::endpoint_select::left:
		return point{ margin.left(), margin.centerY() - 10 };
	case shape::endpoint_select::top_left:
		return point{ margin.left() + 10, margin.top() };
	case shape::endpoint_select::top:
		return point{ margin.centerX() + 10, margin.top() };
	case shape::endpoint_select::top_right:
		return point{ margin.right(), margin.top() + 10 };
	case shape::endpoint_select::right:
		return point{ margin.right(), margin.centerY() + 10 };
	case shape::endpoint_select::bottom_right:
		return point{ margin.right() - 10, margin.bottom() };
	case shape::endpoint_select::bottom:
		return point{ margin.centerX() - 10, margin.bottom() };
	case shape::endpoint_select::bottom_left:
		return point{ margin.left(), margin.bottom() - 10 };
	default:
		throw std::exception();
	}

}

point state::get_endpoint(const SkRegion & region, endpoint_select select, endpoint_duplex endpoint) const
{

	switch (endpoint)
	{
	case shape::endpoint_duplex::single:
		return get_single_endpoints(region, select);
	case shape::endpoint_duplex::source:
		return get_double_endpoints_source(region, select);
	case shape::endpoint_duplex::target:
		return get_double_endpoints_target(region, select);
	default:
		throw std::exception();
	}

}

void state::draw(SkCanvas& canvas, const SkRegion& region)
{

	auto margin(region.getBounds());
	auto border(
		SkRect::MakeLTRB(
			static_cast<SkScalar>(margin.left()   + 5),
			static_cast<SkScalar>(margin.top()    + 5),
			static_cast<SkScalar>(margin.right()  - 5),
			static_cast<SkScalar>(margin.bottom() - 5)
		)
	);

	canvas.drawRoundRect(
		border,
		10,
		10,
		paint_shape
	);

	auto padding(
		SkRect::MakeLTRB(
			static_cast<SkScalar>(border.left()   + 5),
			static_cast<SkScalar>(border.top()    + 5),
			static_cast<SkScalar>(border.right()  - 5),
			static_cast<SkScalar>(border.bottom() - 5)
		)
	);
	
	SkTextBox content;
	content.setBox(padding);
	content.setText(name.c_str(), name.length(), paint_text);
	content.setSpacingAlign(SkTextBox::SpacingAlign::kCenter_SpacingAlign);
	content.draw(&canvas);

}