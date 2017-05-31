#pragma once

#include "dllexport.h"
#include <experimental/filesystem>
#include <memory>

using namespace std::experimental::filesystem::v1;

class states;
class edges;

class presenter 
{

public:

	DLLEXPORT presenter(void);
	DLLEXPORT ~presenter(void);

	DLLEXPORT void emplace_state(
		const std::string&	name,
		unsigned			x,
		unsigned			y
	);

	DLLEXPORT void emplace_edge(
		unsigned			source, 
		unsigned			target, 
		const std::string&	name = ""
	);

	DLLEXPORT void create_window(const path& output);
	DLLEXPORT void create_png(const path& output);

private:

	std::unique_ptr<states> the_states;
	std::unique_ptr<edges>  the_edges;

};