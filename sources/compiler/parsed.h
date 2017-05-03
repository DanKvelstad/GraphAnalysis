#pragma once

#include "dllexport.h"
#include "common.h"
#include "linked_state_name.h"
#include "linked_transition.h"
#include <vector>
#include <string>

class DLLEXPORT parsed
{

public:

	parsed(void);
	~parsed(void);

	void emplace_state(const std::string& name);
	void emplace_edge(unsigned source, unsigned target, const std::string& name);

	const linked_state_name& get_state(unsigned index)		const;
	const linked_transition& get_transition(unsigned index)	const;

	const linked_transition& get_transitions(void)			const;

private:

	linked_state_name	states;
	linked_transition	edges;
	//const parsed*		linked;

};