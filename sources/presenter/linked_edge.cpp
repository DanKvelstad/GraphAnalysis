#include "linked_edge.h"
#include "linked_state.h"
#include "SkCanvas.h"
#include "SkTextBox.h"

linked_edge::linked_edge(void)
: name("")
, source(static_cast<unsigned>(-1))
, target(static_cast<unsigned>(-1))
, linked(nullptr)
{
}

linked_edge::linked_edge(const linked_edge & other)
: name(other.name)
, source(other.source)
, target(other.target)
{
	if (nullptr != other.linked)
	{
		linked = new linked_edge(*other.linked);
	}
	else
	{
		linked = nullptr;
	}
}

linked_edge::~linked_edge()
{
}

void linked_edge::emplace(const char* new_name, unsigned new_source, unsigned new_target)
{

	if (static_cast<unsigned>(-1)==source || static_cast<unsigned>(-1)==target)
	{
		name	= new_name;
		source	= new_source;
		target	= new_target;
	}
	else if (nullptr == linked)
	{
		linked = new linked_edge();
		linked->emplace(new_name, new_source, new_target);
	}
	else
	{
		linked->emplace(new_name, new_source, new_target);
	}

}

linked_edge* linked_edge::next(void) 
{
	return linked;
}

//auto get_endcap(void) const -> std::vector<pixel_point>
//{
//
//	const float pi(3.141592653589793238462643383279502884f);
//	const float offset_to_line_angle(pi / 4);
//	const float length(10);
//
//	float angle(atan2(target.y - source.y, target.x - source.x));
//
//	std::vector<pixel_point> endcap;
//
//	endcap.emplace_back(
//		target.x - cosf(angle - offset_to_line_angle)*length,
//		target.y - sinf(angle - offset_to_line_angle)*length
//	);
//
//	endcap.emplace_back(target.x, target.y);
//
//	endcap.emplace_back(
//		target.x - cosf(angle + offset_to_line_angle)*length,
//		target.y - sinf(angle + offset_to_line_angle)*length
//	);
//
//	return endcap;
//
//}

void linked_edge::draw(SkCanvas* canvas, const linked_state& states) const
{

	SkPaint paint_line;
	paint_line.setAntiAlias(true);
	paint_line.setStrokeWidth(4);
	paint_line.setColor(SK_ColorRED);

	SkPaint paint_text;
	paint_text.setAntiAlias(true);
	paint_text.setStrokeWidth(4);
	paint_text.setColor(SK_ColorWHITE);
	paint_text.setTextSize(22.f);
	paint_text.setTextAlign(SkPaint::Align::kCenter_Align);


	auto source_point(states.at(source).intersection(states.at(target)));
	auto target_point(states.at(target).intersection(states.at(source)));

	canvas->drawLine(
		static_cast<SkScalar>(source_point.x), static_cast<SkScalar>(source_point.y),
		static_cast<SkScalar>(target_point.x), static_cast<SkScalar>(target_point.y),
		paint_line
	);

	if (linked)
	{
		linked->draw(canvas, states);
	}

}