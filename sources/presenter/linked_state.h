#pragma once

#include "dllexport.h"
#include "common.h"
#include <string>

class SkCanvas;

class DLLEXPORT linked_state
{

	struct state_data
	{

		state_data(const char* name, int left, int top, int width, int height)
		: name(name)
		, left(left)
		, top(top)
		, right(left + width)
		, bottom(top + height)
		, center(point{ left + width / 2, top + height / 2 })
		{
		}

		const char*	name;
		const int	left;
		const int	top;
		const int	right;
		const int	bottom;
		const point	center;

	};

public:

	linked_state(unsigned index=0);
	linked_state(const linked_state& other);
	~linked_state(void);

	void emplace(const char* name, int x, int y);
	linked_state* next(void);
	const linked_state& at(unsigned i) const;

	point intersection(const linked_state& target) const;

	const state_data& get() const;

	void draw(SkCanvas* canvas) const;

private:

	const unsigned	index;
	const int		width;
	const int		height;
	const int		spacing;

	state_data*		data;
	linked_state*	linked;

};