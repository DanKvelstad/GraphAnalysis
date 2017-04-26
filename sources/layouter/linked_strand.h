#pragma once

#include "dllexport.h"
#include "common.h"
#include "linked_knot.h"
#include <utility>
#include <functional>

class DLLEXPORT linked_strand
{

public:

	linked_strand(unsigned& intersection_counter);
	linked_strand(const linked_strand& old);
	linked_strand& operator=(const linked_strand& old);
	~linked_strand(void);

	void emplace(const linked_knot& map, unsigned source, unsigned target);
	
	unsigned get_source(void) const;
	unsigned get_target(void) const;

	const linked_strand* next(void) const;

private:

	unsigned&						intersection_counter;
	linked_strand*					linked;
	std::pair<unsigned, unsigned>*	knots;

};