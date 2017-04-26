#pragma once

#include "dllexport.h"
#include "common.h"

class DLLEXPORT linked_knot
{

public:

	linked_knot(unsigned index=0);
	linked_knot(const linked_knot& old);
	linked_knot& operator=(const linked_knot& old);
	~linked_knot(void);

	void emplace(const grid_point& new_point);

	const grid_point&  get(void)		const;
	const linked_knot& get(unsigned i)	const;
	unsigned get_index(void)			const;

	const linked_knot* next(void) const;

private:

	unsigned index;

	grid_point*		grid;
	linked_knot*	linked;

};