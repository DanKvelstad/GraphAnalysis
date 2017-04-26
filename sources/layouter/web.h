#pragma once

#include "common.h"
#include "linked_knot.h"
#include "linked_strand.h"

class DLLEXPORT web
{

public:

	web(void);
	web(const web& old);
	web& operator=(const web& old);
	~web(void);

	void emplace_knot(const grid_point& point);
	const linked_knot& get_knot(unsigned index)	const;
	const linked_knot* get_knots(void)			const;
	
	void emplace_strand(unsigned source, unsigned target);
	const linked_strand* get_strands(void);

	unsigned get_intersection_count(void) const;

private:

	unsigned		intersections;
	linked_knot		knots;
	linked_strand	strands;

};