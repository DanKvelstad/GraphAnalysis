#include "linked_state_name.h"

linked_state_name::linked_state_name(void)
: name("")
, linked(nullptr)
{
	name[0] = '\0';
}

linked_state_name::linked_state_name(const linked_state_name& old)
{
	auto length_of_name(strlen(old.name));
	if (0 < length_of_name)
	{
		std::copy(old.name, old.name+length_of_name+1, name);
	}
	if (nullptr == old.linked)
	{
		linked = nullptr;
	}
	else
	{
		linked = new linked_state_name(*old.linked);
	}
}

linked_state_name & linked_state_name::operator=(const linked_state_name& old)
{
	auto length_of_name(strlen(old.name));
	if (0 < length_of_name)
	{
		std::copy(old.name, old.name+length_of_name+1, name);
	}
	if (nullptr == old.linked)
	{
		linked = nullptr;
	}
	else
	{
		linked = new linked_state_name(*old.linked);
	}
	return *this;
}

linked_state_name::~linked_state_name(void)
{
	if (nullptr != linked)
	{
		delete linked;
	}
}

void linked_state_name::emplace(const std::string& new_name)
{
	if ('\0'==name[0])
	{
		*std::copy(new_name.begin(), new_name.end(), name) = '\0';
	}
	else if (nullptr == linked)
	{
		linked = new linked_state_name();
		linked->emplace(name);
	}
	else
	{
		linked->emplace(name);
	}
}

const char * linked_state_name::get(void) const
{
	return name;
}

const linked_state_name& linked_state_name::get_name(unsigned i) const
{
	if (0 == i)
	{
		return *this;
	}
	else
	{
		return linked->get_name(i - 1);
	}
}

const linked_state_name* linked_state_name::next(void) const
{
	return linked;
}