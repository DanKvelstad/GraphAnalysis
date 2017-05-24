#include "edge.h"
#include "states.h"
#include "common.h"
#include "algorithms.h"
#include "SkCanvas.h"
#include "SkTextBox.h"
#include "SkRegion.h"


edge::edge(unsigned source, unsigned target, const std::string & name)
: source(source)
, target(target)
, name(name)
{

	paint_line.setAntiAlias(true);
	paint_line.setStyle(SkPaint::kStroke_Style);
	paint_line.setStrokeWidth(4);
	paint_line.setStrokeCap(SkPaint::Cap::kRound_Cap);
	paint_line.setColor(SK_ColorBLACK);

	paint_text.setAntiAlias(true);
	paint_text.setStrokeWidth(4);
	paint_text.setColor(SK_ColorBLACK);
	paint_text.setTextSize(22.f);

}

edge::edge(const edge & other)
: source(other.source)
, target(other.target)
, name(other.name)
, paint_line(other.paint_line)
, paint_text(other.paint_text)
{
}

edge::~edge(void)
{
}

void edge::add(const std::string & another_name)
{
	name += ", " + another_name;
}

unsigned edge::get_spacing(const states& the_states) const
{

	SkRect bounds;
	paint_text.measureText(name.c_str(), name.length(), &bounds);
	float hypotenuse = bounds.width() + 50;

	auto source_point(the_states.get_endpoint(source, target, states::states_duplex::unidirectional_source));
	auto target_point(the_states.get_endpoint(source, target, states::states_duplex::unidirectional_target));
	float angle_in_radians(
		atan2f(
			static_cast<float>(target_point.y - source_point.y),
			static_cast<float>(target_point.x - source_point.x)
		)
	);

	float opposite = std::abs(std::cosf(angle_in_radians)*hypotenuse);
	float adjacent = std::abs(std::sinf(angle_in_radians)*hypotenuse);

	return static_cast<unsigned>(std::fmaxf(opposite, adjacent));

}

void edge::draw(SkCanvas& canvas, const states& the_states, bool single)
{
	
	const auto text_offset(paint_text.getTextSize());

	const float pi(3.141592653589793238462643383279502884f);
	const float offset_to_line_angle(pi / 4);
	const float arrowhead_length(10);

	if (source == target)
	{

		auto region(the_states.region_of_state(source).getBounds());
		point source_point{ region.left() + 25, region.top() };
		point target_point{ region.left(), region.top() + 25 };

		canvas.drawArc(
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
		canvas.drawLine(
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
		canvas.drawLine(
			static_cast<SkScalar>(a2.x),
			static_cast<SkScalar>(a2.y),
			static_cast<SkScalar>(target_point.x),
			static_cast<SkScalar>(target_point.y),
			paint_line
		);

		paint_text.setTextAlign(SkPaint::Align::kCenter_Align);
		SkTextBox text_box;
		text_box.setText(name.c_str(), name.length(), paint_text);
		text_box.setSpacingAlign(SkTextBox::SpacingAlign::kCenter_SpacingAlign);
		text_box.setBox(
			static_cast<SkScalar>(target_point.x - 25),
			static_cast<SkScalar>(source_point.y - 50),
			static_cast<SkScalar>(source_point.x),
			static_cast<SkScalar>(source_point.y - 25 - 5)
		);
		text_box.draw(&canvas);

	}
	else
	{
		point source_point;
		point target_point;
		
		if (single)
		{
			source_point = the_states.get_endpoint(source, target, states::states_duplex::unidirectional_source);
			target_point = the_states.get_endpoint(source, target, states::states_duplex::unidirectional_target);
		}
		else
		{
			source_point = the_states.get_endpoint(source, target, states::states_duplex::bidirectional_source);
			target_point = the_states.get_endpoint(source, target, states::states_duplex::bidirectional_target);
		}

		canvas.drawLine(
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
		canvas.drawLine(
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
		canvas.drawLine(
			static_cast<SkScalar>(a2.x),
			static_cast<SkScalar>(a2.y),
			static_cast<SkScalar>(target_point.x),
			static_cast<SkScalar>(target_point.y),
			paint_line
		);

		target_point = intersecting_line_segments(source_point, target_point, a1, a2).second;

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
		// |        270°
		// |         |
		// | 180°--Source--0°
		// |         |
		// |        90° 
		// v y

		if ((0 + EPSILON) > line_angle || (2 * pi + NEPSILON) < line_angle)
		{	// 0 degrees, target is to the right
			paint_text.setTextAlign(SkPaint::Align::kCenter_Align);
			SkTextBox text_box;
			text_box.setText(name.c_str(), name.length(), paint_text);
			text_box.setSpacingAlign(SkTextBox::SpacingAlign::kCenter_SpacingAlign);
			text_box.setBox(
				static_cast<SkScalar>(source_point.x),
				static_cast<SkScalar>(source_point.y - text_offset),
				static_cast<SkScalar>(target_point.x),
				static_cast<SkScalar>(target_point.y - 5)
			);
			text_box.draw(&canvas);
		}
		else if ((pi / 2 + NEPSILON) > line_angle)
		{	// Between zero and 90 degrees
			canvas.rotate(
				static_cast<SkScalar>(line_angle * 180 / pi),
				static_cast<SkScalar>(source_point.x + (target_point.x - source_point.x) / 2),
				static_cast<SkScalar>(source_point.y + (target_point.y - source_point.y) / 2)
			);
			paint_text.setTextAlign(SkPaint::Align::kCenter_Align);
			SkTextBox text_box;
			text_box.setText(name.c_str(), name.length(), paint_text);
			text_box.setSpacingAlign(SkTextBox::SpacingAlign::kCenter_SpacingAlign);
			text_box.setBox(
				static_cast<SkScalar>(source_point.x),
				static_cast<SkScalar>(source_point.y - text_offset),
				static_cast<SkScalar>(target_point.x),
				static_cast<SkScalar>(target_point.y - 5)
			);
			text_box.draw(&canvas);
			canvas.resetMatrix();
		}
		else if ((pi / 2 + EPSILON) > line_angle)
		{	// 90 degreees, target is below but its y is larger
			paint_text.setTextAlign(SkPaint::Align::kLeft_Align);
			if (single)
			{
				canvas.drawText(
					name.c_str(),
					name.length(),
					static_cast<SkScalar>(source_point.x + 5),
					static_cast<SkScalar>(source_point.y - (source_point.y - target_point.y) / 2) + paint_text.getTextSize() / 4,
					paint_text
				);
			}
			else
			{
				paint_text.setTextAlign(SkPaint::Align::kRight_Align);
				canvas.drawText(
					name.c_str(),
					name.length(),
					static_cast<SkScalar>(source_point.x - 5),
					static_cast<SkScalar>(source_point.y - (source_point.y - target_point.y) / 2) + paint_text.getTextSize() / 4,
					paint_text
				);
			}
		}
		else if ((pi + NEPSILON) > line_angle)
		{	// between 90 and 180 degrees
			canvas.rotate(
				static_cast<SkScalar>(line_angle * 180 / pi - 180),
				static_cast<SkScalar>(target_point.x + (source_point.x - target_point.x) / 2),
				static_cast<SkScalar>(target_point.y + (source_point.y - target_point.y) / 2)
			);
			paint_text.setTextAlign(SkPaint::Align::kCenter_Align);
			SkTextBox text_box;
			text_box.setText(name.c_str(), name.length(), paint_text); 
			text_box.setSpacingAlign(SkTextBox::SpacingAlign::kCenter_SpacingAlign);
			if (single)
			{
				text_box.setBox(
					static_cast<SkScalar>(target_point.x),
					static_cast<SkScalar>(source_point.y - text_offset),
					static_cast<SkScalar>(source_point.x),
					static_cast<SkScalar>(target_point.y - 5)
				);
			}
			else
			{
				text_box.setBox(
					static_cast<SkScalar>(target_point.x),
					static_cast<SkScalar>(source_point.y + 5),
					static_cast<SkScalar>(source_point.x),
					static_cast<SkScalar>(target_point.y + text_offset)
				);
			}
			text_box.draw(&canvas);
			canvas.resetMatrix();
		}
		else if ((pi + EPSILON) > line_angle)
		{	// 180 degrees, target is to the left
			paint_text.setTextAlign(SkPaint::Align::kCenter_Align);
			SkTextBox text_box;
			text_box.setText(name.c_str(), name.length(), paint_text); 
			text_box.setSpacingAlign(SkTextBox::SpacingAlign::kCenter_SpacingAlign);
			if (single)
			{
				text_box.setBox(
					static_cast<SkScalar>(target_point.x),
					static_cast<SkScalar>(target_point.y - text_offset),
					static_cast<SkScalar>(source_point.x),
					static_cast<SkScalar>(source_point.y - 5)
				);
			}
			else
			{
				text_box.setBox(
					static_cast<SkScalar>(target_point.x),
					static_cast<SkScalar>(target_point.y + 5),
					static_cast<SkScalar>(source_point.x),
					static_cast<SkScalar>(source_point.y + text_offset)
				);
			}
			text_box.draw(&canvas);
		}
		else if ((3 * pi / 2 + NEPSILON) > line_angle)
		{	// between 180 and 270 degrees
			canvas.rotate(
				static_cast<SkScalar>(line_angle * 180 / pi - 180),
				static_cast<SkScalar>(target_point.x + (source_point.x - target_point.x) / 2),
				static_cast<SkScalar>(target_point.y + (source_point.y - target_point.y) / 2)
			);
			paint_text.setTextAlign(SkPaint::Align::kCenter_Align);
			SkTextBox text_box;
			text_box.setText(name.c_str(), name.length(), paint_text);
			text_box.setSpacingAlign(SkTextBox::SpacingAlign::kCenter_SpacingAlign);
			text_box.setBox(
				static_cast<SkScalar>(target_point.x),
				static_cast<SkScalar>(target_point.y + 5),
				static_cast<SkScalar>(source_point.x),
				static_cast<SkScalar>(source_point.y + text_offset)
			);
			text_box.draw(&canvas);
			canvas.resetMatrix();
		}
		else if ((3 * pi / 2 + EPSILON) > line_angle)
		{	// 270 degrees, the target is above and its y is smaller
			paint_text.setTextAlign(SkPaint::Align::kLeft_Align);
			canvas.drawText(
				name.c_str(),
				name.length(),
				static_cast<SkScalar>(source_point.x + 5),
				static_cast<SkScalar>(source_point.y + (target_point.y - source_point.y) / 2) + paint_text.getTextSize() / 4,
				paint_text
			);
		}
		else if ((2 * pi + NEPSILON) > line_angle)
		{	// between 270 and 360 degrees
			canvas.rotate(
				static_cast<SkScalar>(line_angle * 180 / pi),
				static_cast<SkScalar>(source_point.x + (target_point.x - source_point.x) / 2),
				static_cast<SkScalar>(source_point.y + (target_point.y - source_point.y) / 2)
			);
			paint_text.setTextAlign(SkPaint::Align::kCenter_Align);
			SkTextBox text_box;
			text_box.setText(name.c_str(), name.length(), paint_text);
			text_box.setSpacingAlign(SkTextBox::SpacingAlign::kCenter_SpacingAlign);
			text_box.setBox(
				static_cast<SkScalar>(source_point.x),
				static_cast<SkScalar>(source_point.y - text_offset),
				static_cast<SkScalar>(target_point.x),
				static_cast<SkScalar>(target_point.y - 5)
			);
			text_box.draw(&canvas);
			canvas.resetMatrix();
		}
		else
		{
			throw std::logic_error("not all angles are covered");
		}
	
	}

}

bool edge::equals(unsigned other_source, unsigned other_target) const
{
	return source == other_source && target == other_target;
}

bool edge::is_the_opposite_edge(const edge & other) const
{
	return source == other.target && target == other.source;
}
