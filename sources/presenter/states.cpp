#include "states.h"
#include "state.h"
#include <algorithm>
#include "SkRegion.h"

states::states(void)
: frame_thickness(50)
, rightmost_state(static_cast<unsigned>(-1))
, lowest_state(static_cast<unsigned>(-1))
, state_width(0)
, state_height(50)
, spacing_width(0)
, spacing_height(0)
{
}

states::states(const states & other)
: frame_thickness(other.frame_thickness)
, rightmost_state(other.rightmost_state)
, lowest_state(other.lowest_state)
, state_width(other.state_width)
, state_height(other.state_height)
, spacing_width(other.spacing_width)
, spacing_height(other.spacing_height)
, my_states(other.my_states)
{
}

states::~states(void)
{
}

void states::emplace(const std::string& name, unsigned x, unsigned y)
{

	my_states.emplace_back(name, x, y);

	auto text_dimensions(my_states.back().get_text_dimensions());
	text_dimensions.first += 50;
	if (state_width < text_dimensions.first)
	{
		state_width  = text_dimensions.first;
	}

	auto region(region_of_state(my_states.back()).getBounds());
	if (static_cast<unsigned>(-1)==rightmost_state)
	{
		rightmost_state = static_cast<unsigned>(my_states.size() - 1);
	}
	else if (region_of_state(my_states.at(rightmost_state)).getBounds().right() < region.right())
	{
		rightmost_state = static_cast<unsigned>(my_states.size() - 1);
	}

	if (static_cast<unsigned>(-1) == lowest_state)
	{
		lowest_state = static_cast<unsigned>(my_states.size() - 1);
	}
	else if (region_of_state(my_states.at(lowest_state)).getBounds().bottom() < region.bottom())
	{
		lowest_state = static_cast<unsigned>(my_states.size() - 1);
	}

}

unsigned states::get_workspace_width(void)
{
	return region_of_state(my_states.at(rightmost_state)).getBounds().right() + frame_thickness;
}

unsigned states::get_workspace_height(void)
{
	return region_of_state(my_states.at(lowest_state)).getBounds().bottom() + frame_thickness;
}

void states::set_spacing(unsigned new_spacing_width, unsigned new_spacing_height)
{
	spacing_width  = new_spacing_width;
	spacing_height = new_spacing_height;
}

SkRegion states::region_of_state(const state& s) const
{
	
	auto x(s.get_x());
	auto y(s.get_y());

	auto left(frame_thickness + x*state_width  + x*spacing_width);
	auto top (frame_thickness + y*state_height + y*spacing_height);

	SkRegion region;
	region.setRect(left, top, left+state_width, top+state_height);
	return region;

}

SkRegion states::region_of_state(unsigned i) const
{
	return region_of_state(my_states.at(i));
}

void states::draw(SkCanvas& canvas)
{

	std::for_each(
		my_states.begin(), my_states.end(),
		[&](state& s)
		{
			s.draw(
				canvas, 
				region_of_state(s)
			);
		}
	);

}