#pragma once

#include <vector>
#include <string>

#ifdef _WIN32 
#	if PROJECT_SUPPORT
#		define DLLEXPORT __declspec(dllexport)
#	else
#		define DLLEXPORT __declspec(dllimport)
#	endif
#else
#	define DLLEXPORT	
#endif

typedef struct 
{
	std::string variable;
	std::string states;
	std::string edges;
} definition;
DLLEXPORT std::vector<definition>  find_variable_definition(std::string content);

DLLEXPORT std::vector<std::string> find_enum_class(const std::string& content, const std::string& type_name);

typedef struct
{
	std::string from;
	std::string ev;
	std::string to;
} transition;
DLLEXPORT std::vector<transition> find_transitions(std::string content, const definition& state_machine);