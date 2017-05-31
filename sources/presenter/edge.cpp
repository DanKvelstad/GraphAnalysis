#include "edge.h"
#include "states.h"
#include "common.h"
#include <algorithm>
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
	paint_text.setTextAlign(SkPaint::Align::kCenter_Align);

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

	return static_cast<unsigned>(std::fmaxf(opposite, adjacent))/the_states.get_cell_distance(source, target);

}

void edge::draw(SkCanvas& canvas, const states& the_states, bool single)
{
	
	const auto text_offset(paint_text.getTextSize());

	const float pi(3.141592653589793238462643383279502884f);
	const float offset_to_line_angle(pi / 4);
	const float arrowhead_length(10);

	if (source == target)
	{

		// do something fancy

	}
	else
	{

		point source_point;
		point target_point;

		{	// draw the line

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

		}

		float line_angle;
		float line_length;
		{

			line_angle = atan2f(
				static_cast<float>(target_point.y - source_point.y),
				static_cast<float>(target_point.x - source_point.x)
			);

			auto diff_x(static_cast<float>(source_point.x - target_point.x));
			auto diff_y(static_cast<float>(source_point.y - target_point.y));
			line_length = std::sqrtf(diff_x*diff_x + diff_y*diff_y);

		}

		{	// draw the arrow

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

		}

		// the angle is in the range [0:pi] and [0:-pi],
		// we now need it to be in the range [0:2pi]
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

		const float EPSILON(0.001f);
		const float NEPSILON(-1 * EPSILON);

		point text_center{
			std::min(source_point.x, target_point.x) + std::abs(source_point.x - target_point.x) / 2,
			std::min(source_point.y, target_point.y) + std::abs(source_point.y - target_point.y) / 2
		};

		if (single)
		{

			if ((pi / 2.0f - EPSILON) < line_angle && (3.0f*pi / 2 - EPSILON) > line_angle)
			{

				// flip the line angle so that the text is written on the other side
				line_angle += pi;

				auto text_angle(line_angle + 3*pi/2);
				float text_hypotenuse = 5.0f;
				float text_opposite = std::cosf(text_angle)*text_hypotenuse;
				float text_adjacent = std::sinf(text_angle)*text_hypotenuse;

				text_center.x = static_cast<scalar>(text_center.x + text_opposite);
				text_center.y = static_cast<scalar>(text_center.y + text_adjacent);

			}
			else
			{

				auto text_angle(line_angle - pi / 2);
				float text_hypotenuse = 5;
				float text_opposite = std::cosf(text_angle)*text_hypotenuse;
				float text_adjacent = std::sinf(text_angle)*text_hypotenuse;

				text_center.x = static_cast<scalar>(text_center.x + text_opposite);
				text_center.y = static_cast<scalar>(text_center.y + text_adjacent);

			}

		}
		else
		{

			if ((pi / 2.0f - EPSILON) < line_angle && (3.0f*pi / 2 - EPSILON) > line_angle)
			{

				// flip the line angle so that the text is written on the other side
				line_angle += pi;

				auto text_angle(line_angle + pi / 2);
				float text_hypotenuse = static_cast<float>(paint_text.getTextSize());
				float text_opposite = std::cosf(text_angle)*text_hypotenuse;
				float text_adjacent = std::sinf(text_angle)*text_hypotenuse;

				text_center.x = static_cast<scalar>(text_center.x + text_opposite);
				text_center.y = static_cast<scalar>(text_center.y + text_adjacent);

			}
			else
			{

				auto text_angle(line_angle - pi / 2);
				float text_hypotenuse = 5;
				float text_opposite = std::cosf(text_angle)*text_hypotenuse;
				float text_adjacent = std::sinf(text_angle)*text_hypotenuse;

				text_center.x = static_cast<scalar>(text_center.x + text_opposite);
				text_center.y = static_cast<scalar>(text_center.y + text_adjacent);

			}

		}

		canvas.rotate(
			static_cast<SkScalar>(line_angle*180/pi),
			static_cast<SkScalar>(text_center.x),
			static_cast<SkScalar>(text_center.y)
		);

		canvas.drawText(
			name.c_str(),
			name.length(),
			static_cast<SkScalar>(text_center.x),
			static_cast<SkScalar>(text_center.y),
			paint_text
		);

		canvas.resetMatrix();

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
