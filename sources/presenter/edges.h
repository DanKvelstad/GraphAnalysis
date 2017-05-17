#pragma once

#include "dllexport.h"
#include <vector>

class SkCanvas;
class state;
class states;
class edge;

class edges
{

public:

	DLLEXPORT edges(void);
	DLLEXPORT edges(const edges& other);
	DLLEXPORT ~edges(void);

	DLLEXPORT void emplace(unsigned source, unsigned target, const std::string& name="");
	
	std::pair<unsigned, unsigned> get_text_dimensions(void);
	
	void draw(SkCanvas& canvas, const states& the_states);

private:

	std::vector<edge> the_edges;

};