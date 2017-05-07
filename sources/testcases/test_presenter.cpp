#include "gtest/gtest.h"
#include "presenter/presenter.h"
#include "presenter/linked_state.h"
#include "presenter/linked_edge.h"

TEST(test_presenter, simple)
{

	linked_state states;
	states.emplace("s0", 0, 0);
	states.emplace("s1", 1, 0);
	states.emplace("s2", 1, 1);
	states.emplace("s3", 0, 1);
	
	linked_edge edges;
	edges.emplace("e0", 0, 1);
	edges.emplace("e1", 1, 2);
	edges.emplace("e2", 2, 3);
	edges.emplace("e3", 3, 0);
	edges.emplace("e4", 0, 2);

	draw(
		temp_directory_path()/"simple.png",
		states, 
		edges
	);

}