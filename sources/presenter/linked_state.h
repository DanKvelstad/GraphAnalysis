#pragma once

#include "dllexport.h"
#include <memory>
#include <string>

class SkCanvas;

class linked_state
{

public:

	DLLEXPORT linked_state(std::string name, unsigned x, unsigned y);
	DLLEXPORT ~linked_state();

	DLLEXPORT void emplace(std::string name, unsigned x, unsigned y);
	DLLEXPORT linked_state* next(void);

	void draw(SkCanvas* canvas) const;

private:

	const unsigned width;
	const unsigned height;
	const unsigned spacing;

	const std::string	name;
	const unsigned		x;
	const unsigned		y;
	
	std::unique_ptr<linked_state> linked;

};