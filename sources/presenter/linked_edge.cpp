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

const char * linked_edge::get_name()
{
	return name;
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

void linked_edge::draw(SkCanvas* canvas, const linked_state& states) const
{

	SkPaint paint_line;
	paint_line.setAntiAlias(true);
	paint_line.setStrokeWidth(4);
	paint_line.setColor(SK_ColorBLACK);

	auto source_point(states.at(source).intersection(states.at(target)));
	auto target_point(states.at(target).intersection(states.at(source)));

	canvas->drawLine(
		static_cast<SkScalar>(source_point.x), 
		static_cast<SkScalar>(source_point.y),
		static_cast<SkScalar>(target_point.x), 
		static_cast<SkScalar>(target_point.y),
		paint_line
	);

	const float pi(3.141592653589793238462643383279502884f);
	const float offset_to_line_angle(pi / 4);
	const float arrowhead_length(10);

	float line_angle(
		atan2f(
			static_cast<float>(target_point.y-source_point.y), 
			static_cast<float>(target_point.x-source_point.x)
		)
	);

	canvas->drawLine(
		static_cast<SkScalar>(target_point.x - cosf(line_angle - offset_to_line_angle)*arrowhead_length),
		static_cast<SkScalar>(target_point.y - sinf(line_angle - offset_to_line_angle)*arrowhead_length),
		static_cast<SkScalar>(target_point.x), 
		static_cast<SkScalar>(target_point.y),
		paint_line
	);

	canvas->drawLine(
		static_cast<SkScalar>(target_point.x - cosf(line_angle + offset_to_line_angle)*arrowhead_length),
		static_cast<SkScalar>(target_point.y - sinf(line_angle + offset_to_line_angle)*arrowhead_length),
		static_cast<SkScalar>(target_point.x),
		static_cast<SkScalar>(target_point.y),
		paint_line
	);

	auto diff_x(source_point.x - target_point.x);
	auto diff_y(source_point.y - target_point.y);
	auto line_length(std::sqrt(diff_x*diff_x + diff_y*diff_y));

	SkPaint paint_text;
	paint_text.setAntiAlias(true);
	paint_text.setStrokeWidth(4);
	paint_text.setColor(SK_ColorBLACK);
	paint_text.setTextSize(22.f);
	
	const float EPSILON(0.0001f);

	if (std::fabs(0-line_angle) < EPSILON)
	{
		paint_text.setTextAlign(SkPaint::Align::kCenter_Align);
		SkTextBox text_box;
		text_box.setText(name, strlen(name), paint_text);
		text_box.setSpacingAlign(SkTextBox::SpacingAlign::kCenter_SpacingAlign);
		text_box.setBox(
			static_cast<SkScalar>(source_point.x),
			static_cast<SkScalar>(source_point.y - paint_text.getTextSize() - 5),
			static_cast<SkScalar>(target_point.x),
			static_cast<SkScalar>(target_point.y)
		);
		text_box.draw(canvas);
	}
	else if (std::fabs(pi/2 - line_angle) < EPSILON)
	{
		paint_text.setTextAlign(SkPaint::Align::kLeft_Align);
		canvas->drawText(
			name,
			strlen(name),
			static_cast<SkScalar>(source_point.x+5),
			static_cast<SkScalar>(source_point.y + (target_point.y - source_point.y)/2) + paint_text.getTextSize()/4,
			paint_text
		);
	}
	else if (std::fabs(pi - line_angle) < EPSILON)
	{
		paint_text.setTextAlign(SkPaint::Align::kCenter_Align);
		SkTextBox text_box;
		text_box.setText(name, strlen(name), paint_text);
		text_box.setSpacingAlign(SkTextBox::SpacingAlign::kCenter_SpacingAlign);
		text_box.setBox(
			static_cast<SkScalar>(target_point.x),
			static_cast<SkScalar>(target_point.y - paint_text.getTextSize() - 5),
			static_cast<SkScalar>(source_point.x),
			static_cast<SkScalar>(source_point.y)
		);
		text_box.draw(canvas);
	}
	else if (std::fabs(-1*pi/2 - line_angle) < EPSILON)
	{
		paint_text.setTextAlign(SkPaint::Align::kLeft_Align);
		canvas->drawText(
			name,
			strlen(name),
			static_cast<SkScalar>(source_point.x + 5),
			static_cast<SkScalar>(target_point.y + (source_point.y - target_point.y)/2) + paint_text.getTextSize() / 4,
			paint_text
		);
	}
	else
	{
		paint_text.setTextAlign(SkPaint::Align::kCenter_Align);
		SkTextBox text_box;
		text_box.setText(name, strlen(name), paint_text);
		text_box.setSpacingAlign(SkTextBox::SpacingAlign::kCenter_SpacingAlign);
		text_box.setBox(
			static_cast<SkScalar>(source_point.x),
			static_cast<SkScalar>(source_point.y),
			static_cast<SkScalar>(target_point.x),
			static_cast<SkScalar>(target_point.y)
		);
		text_box.draw(canvas);
	}

	if (nullptr!=linked)
	{
		linked->draw(canvas, states);
	}

}