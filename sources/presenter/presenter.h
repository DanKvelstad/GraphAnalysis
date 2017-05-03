#pragma once

#include "dllexport.h"
#include "linked_state.h"
#include "linked_edge.h"
#include <experimental/filesystem>

using namespace std::experimental::filesystem::v1;

DLLEXPORT void draw(
	const path&		output,
	linked_state	states, 
	linked_edge		edges
);