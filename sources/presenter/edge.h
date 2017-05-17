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

	void add(const std::string& another_name);

	std::pair<unsigned, unsigned> get_text_dimensions(void);

	void draw(SkCanvas& canvas, const states& the_states);

	bool equals(const unsigned& other_source, const unsigned& other_target) const;

private:

	std::string name;
	unsigned source;
	unsigned target;

	SkPaint paint_line;
	SkPaint paint_text;

};