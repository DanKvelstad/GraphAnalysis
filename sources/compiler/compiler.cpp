#include "compiler.h"
#include "support.h"
#include <map>

DLLEXPORT parsed parse(const path& working_folder)
{

	auto files(
		search_for_files_with_extension(
			working_folder,
			std::vector<std::string>{
				".c", ".cc", ".cpp", ".c++", ".cp", ".cxx",
				".h", ".hpp" 
			}
		)
	);

	std::vector<std::pair<path, definition>> definitions; 
	for (auto& file : files)
	{
		for (auto& definition : find_variable_definition(file_to_string(file)))
		{
			definitions.emplace_back(file, definition);
		}
	}

	
	parsed to_return;

	for (auto& result : definitions)
	{

		std::string content(file_to_string(result.first));

		auto states(find_enum_class(content, result.second.states));
		std::map<std::string, unsigned> state_string_to_index;
		for (unsigned i(0); i<states.size(); i++)
		{
			state_string_to_index.emplace(states.at(i), i);
			to_return.emplace_state(states.at(i));
		}
		
		for (auto& transition : find_transitions(content, result.second))
		{
			to_return.emplace_edge(
				state_string_to_index.find(transition.from_state)->second,
				state_string_to_index.find(transition.to_state)->second,
				transition.event_name
			);
		}
		
	}

	return const_cast<parsed&>(to_return);

}
