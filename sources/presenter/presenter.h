#pragma once

#include "dllexport.h"
#include "states.h"
#include "edges.h"
#include <experimental/filesystem>

using namespace std::experimental::filesystem::v1;

DLLEXPORT void draw(
	const path&		output,
	states			the_states, 
	edges			the_edges
);