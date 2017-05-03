#include "linked_transition.h"
#include <algorithm>

linked_transition::linked_transition(void)
: linked(nullptr)
, source(static_cast<unsigned>(-1))
, target(static_cast<unsigned>(-1))
{
	name[0] = '\0';
}

linked_transition::linked_transition(const linked_transition& old)
: source(old.source)
, target(old.target)
{
	auto length_of_name(strlen(old.name));
	if (0 < length_of_name)
	{
		std::copy(old.name, old.name+length_of_name+1, name);
	}
	if (nullptr != old.linked)
	{
		linked = new linked_transition(*old.linked);
	}
	else
	{
		linked = nullptr;
	}
}

linked_transition& linked_transition::operator=(const linked_transition& old)
{
	source = old.source;
	target = old.target;
	auto length_of_name(strlen(old.name));
	if (0 < length_of_name)
	{
		std::copy(old.name, old.name+length_of_name+1, name);
	}
	if (nullptr != old.linked)
	{
		linked = new linked_transition(*old.linked);
	}
	else
	{
		linked = nullptr;
	}
	return *this;
}

linked_transition::~linked_transition(void)
{
	if (nullptr != linked)
	{
		delete linked;
	}
}

void linked_transition::emplace(unsigned new_source, unsigned new_target, const std::string& new_name)
{
	
	if (static_cast<unsigned>(-1)==source)
	{
		source = new_source;
		target = new_target;
		if (!new_name.empty())
		{
			*std::copy(new_name.begin(), new_name.end(), name) = '\0';
		}
	}
	else
	{
		if (nullptr == linked)
		{
			linked = new linked_transition();
		}
		linked->emplace(new_source, new_target, new_name);
	}

}

unsigned linked_transition::get_source(void) const
{
	return source;
}

unsigned linked_transition::get_target(void) const
{
	return target;
}

const linked_transition& linked_transition::get(unsigned i) const
{
	if (0 == i)
	{
		return *this;
	}
	else
	{
		return linked->get(i-1);
	}
}

const linked_transition* linked_transition::next(void) const
{
	return linked;
}
