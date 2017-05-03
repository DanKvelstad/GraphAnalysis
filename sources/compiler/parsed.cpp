#include "parsed.h"

parsed::parsed(void)
{
}

parsed::~parsed(void)
{
}

void parsed::emplace_state(const std::string & name)
{
	states.emplace(name);
}

void parsed::emplace_edge(unsigned source, unsigned target, const std::string & name)
{
	edges.emplace(source, target, name);
}

const linked_state_name& parsed::get_state(unsigned index) const
{
	return states.get_name(index);
}

const linked_transition& parsed::get_transition(unsigned index) const
{
	return edges.get(index);
}

const linked_transition & parsed::get_transitions(void) const
{
	return edges;
}
