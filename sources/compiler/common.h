#pragma once

#include <string>

typedef struct
{
	std::string variable;
	std::string states;
	std::string edges;
} definition;

typedef struct
{
	std::string from_state;
	std::string to_state;
	std::string event_name;
} transition;