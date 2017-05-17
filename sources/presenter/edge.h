#pragma once

#include <string>
#include "SkPaint.h"

class states;
class SkCanvas;

class edge
{

public:

	edge(unsigned source, unsigned target, const std::string& name);
	edge(const edge& other);
	~edge(void);

	std::pair<unsigned, unsigned> get_text_dimensions(void);

	void draw(SkCanvas& canvas, const states& the_states);

private:

	const std::string name;
	unsigned source;
	unsigned target;

	SkPaint paint_line;
	SkPaint paint_text;

};