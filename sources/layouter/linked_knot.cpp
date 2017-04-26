#include "linked_knot.h"

linked_knot::linked_knot(unsigned index)
: grid(nullptr)
, linked(nullptr)
, index(index)
{
}

linked_knot::linked_knot(const linked_knot& old)
: index(old.index)
{
	if (nullptr != old.grid)
	{
		grid = new grid_point(*old.grid);
	}
	else
	{
		grid = nullptr;
	}
	if (nullptr != old.linked)
	{
		linked = new linked_knot(*old.linked);
	}
	else
	{
		linked = nullptr;
	}
}

linked_knot & linked_knot::operator=(const linked_knot& old)
{
	index		= old.index;
	if (nullptr != old.grid)
	{
		grid = new grid_point(*old.grid);
	}
	else
	{
		grid = nullptr;
	}
	if (nullptr != old.linked)
	{
		linked = new linked_knot(*old.linked);
	}
	else
	{
		linked = nullptr;
	}
	return *this;
}

linked_knot::~linked_knot(void)
{
	if (nullptr != grid)
	{
		delete grid;
	}
	if (nullptr != linked)
	{
		delete linked;
	}
}

void linked_knot::emplace(const grid_point & new_point)
{
	if (nullptr == grid)
	{
		grid = new grid_point(new_point);
	}
	else if (nullptr == linked)
	{
		linked = new linked_knot(index+1);
		linked->emplace(new_point);
	}
	else
	{
		linked->emplace(new_point);
	}
}

const grid_point& linked_knot::get(void) const
{
	return *grid;
}

const linked_knot& linked_knot::get(unsigned i) const
{
	if (0 == i)
	{
		return *this;
	}
	else
	{
		return linked->get(i - 1);
	}
}

unsigned linked_knot::get_index(void) const
{
	return index;
}

const linked_knot* linked_knot::next(void) const
{
	return linked;
}
