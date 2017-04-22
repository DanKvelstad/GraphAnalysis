#include "gtest/gtest.h"
#include "presenter/presenter.h"
#include "presenter/linked_state.h"
#include "presenter/linked_edge.h"

TEST(skia, hello_world)
{

	linked_state states("s0", 0, 0);
	states.emplace("s1", 1, 0);
	
	linked_edge edges("e0", states, *states.next());

	draw(states, edges);

}