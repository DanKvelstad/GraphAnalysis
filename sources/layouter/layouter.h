#pragma once

#include "dllexport.h"
#include <vector>
#include <utility>
#include "common.h"

class layouter
{

public:

	DLLEXPORT layouter(void);
	DLLEXPORT ~layouter(void);

	DLLEXPORT void emplace(unsigned from_index, unsigned to_index);

	struct layout_candidates { unsigned candidate_count; unsigned state_counts; };
	DLLEXPORT std::pair<unsigned, unsigned> layout();

	DLLEXPORT unsigned get_intersection_count(void);

	struct coordinate { unsigned x; unsigned y; };
	DLLEXPORT coordinate at(unsigned i, unsigned j);

private:

	unsigned intersection_count;
	unsigned largest_index;

	struct edge { unsigned from_index ; unsigned to_index; };
	std::vector<edge> edges;

	std::vector<std::vector<pixel_point>> candidates;

};