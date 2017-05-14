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

point segment_shorten(point source, point target)
{

	const float radius(5);

	float dx = static_cast<float>(target.x - source.x);
	float dy = static_cast<float>(target.y - source.y);
	float length = std::sqrtf(dx * dx + dy * dy);
	if (length > 0)
	{
		dx /= length;
		dy /= length;
	}
	dx *= length - radius;
	dy *= length - radius;

	return point{
		static_cast<scalar>(source.x + dx),
		static_cast<scalar>(source.y + dy)
	};

}

point intersecting_lines(point ts, point tt, point os, point ot)
{

	const float denominator = static_cast<float>(
		(ts.x - tt.x)*(os.y - ot.y) - (ts.y - tt.y)*(os.x - ot.x)
	);

	float intersection_x(
		((ts.x*tt.y - ts.y*tt.x)*(os.x - ot.x) -
		(ts.x - tt.x)*(os.x*ot.y - os.y*ot.x))
		/ denominator
	);
	float intersection_y(
		((ts.x*tt.y - ts.y*tt.x)*(os.y - ot.y) -
		(ts.y - tt.y)*(os.x*ot.y - os.y*ot.x))
		/ denominator
	);

	return point{
		static_cast<int>(std::round(intersection_x)),
		static_cast<int>(std::round(intersection_y))
	};

}

void linked_edge::draw(SkCanvas* canvas, const linked_state& states) const
{

	SkPaint paint_line;
	paint_line.setAntiAlias(true);
	paint_line.setStyle(SkPaint::kStroke_Style);
	paint_line.setStrokeWidth(4);
	paint_line.setStrokeCap(SkPaint::Cap::kRound_Cap);
	paint_line.setColor(SK_ColorBLACK);

	SkPaint paint_text;
	paint_text.setAntiAlias(true);
	paint_text.setStrokeWidth(4);
	paint_text.setColor(SK_ColorBLACK);
	paint_text.setTextSize(22.f);

	const auto text_offset(paint_text.getTextSize() + 5);

	const float pi(3.141592653589793238462643383279502884f);
	const float offset_to_line_angle(pi / 4);
	const float arrowhead_length(10);

	if (source == target)
	{

		auto data(states.at(source).get());
		point source_point{ data.left + 25, data.top };
		point target_point{ data.left, data.top + 25 };

		canvas->drawArc(
			SkRect::MakeLTRB(
				static_cast<SkScalar>(target_point.x-25),
				static_cast<SkScalar>(source_point.y-25),
				static_cast<SkScalar>(source_point.x),
				static_cast<SkScalar>(target_point.y)
			),
			90,
			270,
			false,
			paint_line
		);

		point a1{
			static_cast<scalar>(target_point.x - cosf(0-offset_to_line_angle)*arrowhead_length),
			static_cast<scalar>(target_point.y - sinf(0-offset_to_line_angle)*arrowhead_length)
		};
		canvas->drawLine(
			static_cast<SkScalar>(a1.x),
			static_cast<SkScalar>(a1.y),
			static_cast<SkScalar>(target_point.x),
			static_cast<SkScalar>(target_point.y),
			paint_line
		);

		point a2{
			static_cast<scalar>(target_point.x - cosf(0+offset_to_line_angle)*arrowhead_length),
			static_cast<scalar>(target_point.y - sinf(0+offset_to_line_angle)*arrowhead_length)
		};
		canvas->drawLine(
			static_cast<SkScalar>(a2.x),
			static_cast<SkScalar>(a2.y),
			static_cast<SkScalar>(target_point.x),
			static_cast<SkScalar>(target_point.y),
			paint_line
		);

		paint_text.setTextAlign(SkPaint::Align::kCenter_Align);
		SkTextBox text_box;
		text_box.setText(name, strlen(name), paint_text);
		text_box.setSpacingAlign(SkTextBox::SpacingAlign::kCenter_SpacingAlign);
		text_box.setBox(
			static_cast<SkScalar>(target_point.x - 25),
			static_cast<SkScalar>(source_point.y - 50),
			static_cast<SkScalar>(source_point.x),
			static_cast<SkScalar>(source_point.y - 25 - 5)
		);
		text_box.draw(canvas);

	}
	else
	{

		// todo: i dont really want the intersection, i want the two points 
		//       where the distance between them is the smallest, that would 
		//       improve the visual impression.

		auto source_point(states.at(source).intersection(states.at(target)));
		auto target_point(states.at(target).intersection(states.at(source)));
		
		canvas->drawLine(
			static_cast<SkScalar>(source_point.x),
			static_cast<SkScalar>(source_point.y),
			static_cast<SkScalar>(target_point.x),
			static_cast<SkScalar>(target_point.y),
			paint_line
		);

		float line_angle(
			atan2f(
				static_cast<float>(target_point.y - source_point.y),
				static_cast<float>(target_point.x - source_point.x)
			)
		);

		point a1{
			static_cast<scalar>(target_point.x - cosf(line_angle - offset_to_line_angle)*arrowhead_length),
			static_cast<scalar>(target_point.y - sinf(line_angle - offset_to_line_angle)*arrowhead_length)
		};
		canvas->drawLine(
			static_cast<SkScalar>(a1.x),
			static_cast<SkScalar>(a1.y),
			static_cast<SkScalar>(target_point.x),
			static_cast<SkScalar>(target_point.y),
			paint_line
		);

		point a2{
			static_cast<scalar>(target_point.x - cosf(line_angle + offset_to_line_angle)*arrowhead_length),
			static_cast<scalar>(target_point.y - sinf(line_angle + offset_to_line_angle)*arrowhead_length)
		};
		canvas->drawLine(
			static_cast<SkScalar>(a2.x),
			static_cast<SkScalar>(a2.y),
			static_cast<SkScalar>(target_point.x),
			static_cast<SkScalar>(target_point.y),
			paint_line
		);

		target_point = intersecting_lines(source_point, target_point, a1, a2);

		auto diff_x(source_point.x - target_point.x);
		auto diff_y(source_point.y - target_point.y);
		auto line_length(std::sqrt(diff_x*diff_x + diff_y*diff_y));

		const float EPSILON(0.0001f);
		const float NEPSILON(-1 * EPSILON);

		// the angle is in the range [0:pi] and [0:-pi],
		// we need it to be in the range [0:2pi]
		if (0 > line_angle)
		{
			line_angle = line_angle + 2 * pi;
		}

		// this becomes slightly disorientating, remember that y grows downward!
		// the angles are thus as follows:
		//
		// +-----------------> x
		// |        270�
		// |         |
		// | 180�--Source--0�
		// |         |
		// |        90� 
		// v y

		if ((0 + EPSILON) > line_angle || (2 * pi + NEPSILON) < line_angle)
		{	// 0 degrees, target is to the right
			paint_text.setTextAlign(SkPaint::Align::kCenter_Align);
			SkTextBox text_box;
			text_box.setText(name, strlen(name), paint_text);
			text_box.setSpacingAlign(SkTextBox::SpacingAlign::kCenter_SpacingAlign);
			text_box.setBox(
				static_cast<SkScalar>(source_point.x),
				static_cast<SkScalar>(source_point.y - text_offset),
				static_cast<SkScalar>(target_point.x),
				static_cast<SkScalar>(target_point.y)
			);
			text_box.draw(canvas);
		}
		else if ((pi / 2 + NEPSILON) > line_angle)
		{	// Between zero and 90 degrees
			canvas->rotate(
				static_cast<SkScalar>(line_angle * 180 / pi),
				static_cast<SkScalar>(source_point.x + (target_point.x - source_point.x) / 2),
				static_cast<SkScalar>(source_point.y + (target_point.y - source_point.y) / 2)
			);
			paint_text.setTextAlign(SkPaint::Align::kCenter_Align);
			SkTextBox text_box;
			text_box.setText(name, strlen(name), paint_text);
			text_box.setSpacingAlign(SkTextBox::SpacingAlign::kCenter_SpacingAlign);
			text_box.setBox(
				static_cast<SkScalar>(source_point.x),
				static_cast<SkScalar>(source_point.y - text_offset),
				static_cast<SkScalar>(target_point.x),
				static_cast<SkScalar>(target_point.y)
			);
			text_box.draw(canvas);
			canvas->resetMatrix();
		}
		else if ((pi / 2 + EPSILON) > line_angle)
		{	// 90 degreees, target is below but its y is larger
			paint_text.setTextAlign(SkPaint::Align::kLeft_Align);
			canvas->drawText(
				name,
				strlen(name),
				static_cast<SkScalar>(source_point.x + 5),
				static_cast<SkScalar>(source_point.y - (source_point.y - target_point.y) / 2) + paint_text.getTextSize() / 4,
				paint_text
			);
		}
		else if ((pi + NEPSILON) > line_angle)
		{	// between 90 and 180 degrees
			canvas->rotate(
				static_cast<SkScalar>(line_angle * 180 / pi - 180),
				static_cast<SkScalar>(target_point.x + (source_point.x - target_point.x) / 2),
				static_cast<SkScalar>(target_point.y + (source_point.y - target_point.y) / 2)
			);
			paint_text.setTextAlign(SkPaint::Align::kCenter_Align);
			SkTextBox text_box;
			text_box.setText(name, strlen(name), paint_text);
			text_box.setSpacingAlign(SkTextBox::SpacingAlign::kCenter_SpacingAlign);
			text_box.setBox(
				static_cast<SkScalar>(target_point.x),
				static_cast<SkScalar>(source_point.y - text_offset),
				static_cast<SkScalar>(source_point.x),
				static_cast<SkScalar>(target_point.y)
			);
			text_box.draw(canvas);
			canvas->resetMatrix();
		}
		else if ((pi + EPSILON) > line_angle)
		{	// 180 degrees, target is to the left
			paint_text.setTextAlign(SkPaint::Align::kCenter_Align);
			SkTextBox text_box;
			text_box.setText(name, strlen(name), paint_text);
			text_box.setSpacingAlign(SkTextBox::SpacingAlign::kCenter_SpacingAlign);
			text_box.setBox(
				static_cast<SkScalar>(target_point.x),
				static_cast<SkScalar>(target_point.y - text_offset),
				static_cast<SkScalar>(source_point.x),
				static_cast<SkScalar>(source_point.y)
			);
			text_box.draw(canvas);
		}
		else if ((3 * pi / 2 + NEPSILON) > line_angle)
		{	// between 180 and 270 degrees
			canvas->rotate(
				static_cast<SkScalar>(line_angle * 180 / pi - 180),
				static_cast<SkScalar>(target_point.x + (source_point.x - target_point.x) / 2),
				static_cast<SkScalar>(target_point.y + (source_point.y - target_point.y) / 2)
			);
			paint_text.setTextAlign(SkPaint::Align::kCenter_Align);
			SkTextBox text_box;
			text_box.setText(name, strlen(name), paint_text);
			text_box.setSpacingAlign(SkTextBox::SpacingAlign::kCenter_SpacingAlign);
			text_box.setBox(
				static_cast<SkScalar>(target_point.x),
				static_cast<SkScalar>(target_point.y - text_offset),
				static_cast<SkScalar>(source_point.x),
				static_cast<SkScalar>(source_point.y)
			);
			text_box.draw(canvas);
			canvas->resetMatrix();
		}
		else if ((3 * pi / 2 + EPSILON) > line_angle)
		{	// 270 degrees, the target is above and its y is smaller
			paint_text.setTextAlign(SkPaint::Align::kLeft_Align);
			canvas->drawText(
				name,
				strlen(name),
				static_cast<SkScalar>(source_point.x + 5),
				static_cast<SkScalar>(source_point.y + (target_point.y - source_point.y) / 2) + paint_text.getTextSize() / 4,
				paint_text
			);
		}
		else if ((2 * pi + NEPSILON) > line_angle)
		{	// between 270 and 360 degrees
			canvas->rotate(
				static_cast<SkScalar>(line_angle * 180 / pi),
				static_cast<SkScalar>(source_point.x + (target_point.x - source_point.x) / 2),
				static_cast<SkScalar>(source_point.y + (target_point.y - source_point.y) / 2)
			);
			paint_text.setTextAlign(SkPaint::Align::kCenter_Align);
			SkTextBox text_box;
			text_box.setText(name, strlen(name), paint_text);
			text_box.setSpacingAlign(SkTextBox::SpacingAlign::kCenter_SpacingAlign);
			text_box.setBox(
				static_cast<SkScalar>(source_point.x),
				static_cast<SkScalar>(source_point.y - text_offset),
				static_cast<SkScalar>(target_point.x),
				static_cast<SkScalar>(target_point.y)
			);
			text_box.draw(canvas);
			canvas->resetMatrix();
		}
		else
		{
			throw std::logic_error("not all angles are covered");
		}
	
	}

	if (nullptr!=linked)
	{
		linked->draw(canvas, states);
	}

}