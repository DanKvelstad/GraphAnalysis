#include <gtest/gtest.h>
#include "../graphs/moore.h"
#include <queue>

enum class state
{
	s0, s1, s2, s3, s4
};

enum class edge
{
	e0, e1, e2, e3, e4
};

TEST(Moore, Simple)
{

	std::queue<state> states;
	
	moore<state, edge> mut(
		state::s0,
		[&states](void) -> void
		{
			states.push(state::s0);
		}
	);

	mut.add_state(
		state::s1,
		[&states](void) -> void
		{
			states.push(state::s1);
		}
	);

	mut.add_edge(state::s0, edge::e0, state::s1);
	mut.add_edge(state::s1,           state::s0);

	mut.handle_event(edge::e0);

	ASSERT_EQ(3, states.size());
	ASSERT_EQ(state::s0, states.front());
	states.pop();
	ASSERT_EQ(state::s1, states.front());
	states.pop();
	ASSERT_EQ(state::s0, states.front());
	states.pop();

}