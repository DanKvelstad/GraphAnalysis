#pragma once

#include <vector>

class SkCanvas;
class state;
class states;
class edge;

class edges
{

public:

	edges(void);
	edges(const edges& other);
	~edges(void);

	void emplace(unsigned source, unsigned target, const std::string& name="");
	
	void update_spacing(states& the_states);
	
	void draw(SkCanvas& canvas, const states& the_states);

private:

	std::vector<edge> the_edges;

};