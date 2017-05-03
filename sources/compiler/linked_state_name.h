#pragma once

#include "dllexport.h"
#include "common.h"

class DLLEXPORT linked_state_name
{

public:

	linked_state_name(void);
	linked_state_name(const linked_state_name& old);
	linked_state_name& operator=(const linked_state_name& old);
	linked_state_name(linked_state_name&& old) = delete;
	linked_state_name& operator=(linked_state_name&& old) = delete;
	~linked_state_name(void);

	void emplace(const std::string& name);

	const char* get(void) const;
	const linked_state_name& get_name(unsigned i)	const;

	const linked_state_name* next(void) const;

private:

	char				name[128];
	linked_state_name*	linked;

};