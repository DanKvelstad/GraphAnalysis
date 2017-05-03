#pragma once

#include "dllexport.h"
#include "common.h"
#include "linked_transition.h"
#include <utility>
#include <functional>

class DLLEXPORT linked_transition
{

public:

	linked_transition(void);
	linked_transition(const linked_transition& old);
	linked_transition& operator=(const linked_transition& old);
	linked_transition(linked_transition&& old) = delete;
	linked_transition& operator=(linked_transition&& old) = delete;
	~linked_transition(void);

	void emplace(unsigned source, unsigned target, const std::string& name);
	
	unsigned get_source(void) const;
	unsigned get_target(void) const;

	const linked_transition& get(unsigned i) const;
	const linked_transition* next(void) const;

private:

	linked_transition*	linked;
	unsigned			source;
	unsigned			target;
	char				name[128];
	
};