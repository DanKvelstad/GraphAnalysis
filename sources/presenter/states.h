#pragma once

#include "common.h"
#include <string>
#include <vector>
#include "shape.h"

class state;
class SkCanvas;
class SkRegion;

class states
{

public:

	states(void);
	states(const states& other);
	~states(void);
	
	void emplace(const std::string& name, unsigned x, unsigned y);

	unsigned get_workspace_width(void);
	unsigned get_workspace_height(void);

	unsigned get_cell_distance(unsigned a, unsigned b) const;
	void set_spacing(unsigned new_spacing_width, unsigned new_spacing_height);

	SkRegion region_of_state(const state& s) const;
	SkRegion region_of_state(unsigned i) const;

	enum class states_duplex{ unidirectional_source, unidirectional_target, bidirectional_source, bidirectional_target };
	point get_endpoint(unsigned source, unsigned target, states_duplex duplex) const;

	void draw(SkCanvas& canvas);

private:

	const unsigned		frame_thickness;

	unsigned			rightmost_state;
	unsigned			lowest_state;

	unsigned			state_width;
	unsigned			state_height;

	unsigned			spacing_width;
	unsigned			spacing_height;

	std::vector<state>	my_states;

};