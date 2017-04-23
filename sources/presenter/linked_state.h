#pragma once

#include "dllexport.h"
#include "common.h"
#include <memory>
#include <string>

class SkCanvas;

class linked_state
{

public:

	DLLEXPORT linked_state(std::string name, int x, int y);
	DLLEXPORT ~linked_state();

	DLLEXPORT void emplace(std::string name, int x, int y);
	DLLEXPORT linked_state* next(void);

	point intersection(const linked_state& target) const;

	void draw(SkCanvas* canvas) const;

private:

	const int width;
	const int height;
	const int spacing;

	const std::string	name;
	const int		left;
	const int		top;
	const int		right;
	const int		bottom;
	const point			center;
	
	std::unique_ptr<linked_state> linked;

};