#pragma once

#include "graphs\moore.h"

enum class state
{
	s0,
	s1,
	s2,
	s3
};

enum class event
{
	e0,
	e1,
	e2
};

class test
{

	test(void)
	: state_machine(state::s0, [](){})
	{
		state_machine.add_state(state::s1, [](){});
		state_machine.add_state(state::s2, [](){});
		state_machine.add_state(state::s3, [](){});
		state_machine.add_transition(state::s0, event::e0, state::s2);
		state_machine.add_transition(state::s0, event::e1, state::s3);
		state_machine.add_transition(state::s1, event::e2, state::s0);
		state_machine.add_transition(state::s2, state::s1);
		state_machine.add_transition(state::s3, state::s1);

	}

	moore<state, event> state_machine;

};