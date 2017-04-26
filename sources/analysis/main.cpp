#include "support/support.h"
#include "presenter/presenter.h"
#include "presenter/linked_state.h"
#include "presenter/linked_edge.h"
#include <experimental/filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <layouter\layouters.h>
#include <map>

using namespace std::experimental::filesystem::v1;

int main(int argc, char* argv[])
{

	path working_folder;
	if (1 == argc)
	{
		working_folder = path(canonical(argv[0]).parent_path());
	}
	else if (2 == argc)
	{
		working_folder = path(canonical(argv[1]));
	}
	else
	{
		return EXIT_FAILURE;
	}
	if (!exists(working_folder))
	{
		std::cout << "Could not find " << working_folder << std::endl;
		return EXIT_FAILURE;
	}
	
	std::cout << "Searching for files in " << working_folder << "... ";
	auto search_start(std::chrono::steady_clock::now());
	auto files(
		search_for_files_with_extension(
			working_folder,
			std::vector<std::string>{
				".c", ".cc", ".cpp", ".c++", ".cp", ".cxx",
				".h", ".hpp" 
			}
		)
	);
	std::cout << "done" << std::endl;
	auto search_duration(
		std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::steady_clock::now()-search_start
		)
	);
	std::cout	<< "Found " << files.size() << " files in " 
				<< search_duration.count() << "ms" << std::endl;

	std::cout << "Searching through the files for state machines...";
	auto search_sm_start(std::chrono::steady_clock::now());
	std::vector<std::pair<path, definition>> definitions; 
	for (auto& file : files)
	{
		for (auto& definition : find_variable_definition(file_to_string(file)))
		{
			definitions.emplace_back(file, definition);
		}
	}
	std::cout << "done" << std::endl;
	auto search_sm_duration(
		std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::steady_clock::now()-search_sm_start
		)
	);
	std::cout	<< "Found " << definitions.size() << " state machines in "
				<< search_sm_duration.count() << "ms" << std::endl;

	for (auto& result : definitions)
	{
	
		std::string content(file_to_string(result.first));

		std::cout << std::endl;
		std::cout << "Name   " << result.second.variable	<< std::endl;
		std::cout << "File   " << result.first				<< std::endl;
		std::cout << "States " << result.second.states		<< std::endl;
		auto states(find_enum_class(content, result.second.states));
		std::map<std::string, unsigned> state_string_to_index;
		for (unsigned i(0); i<states.size(); i++)
		{
			state_string_to_index.emplace(states.at(i), i);
			std::cout << "       " << states.at(i) << std::endl;
		}
		std::cout << "Edges  " << result.second.edges << std::endl;
		auto edges(find_enum_class(content, result.second.edges));
		for (auto& edge : edges)
		{
			std::cout << "       " << edge << std::endl;
		}
		std::cout << "Transitions" << std::endl;
		auto transitions(find_transitions(content, result.second));
		for (auto& transition : transitions)
		{
			std::cout << "       " << transition.from << ":" << transition.ev << ":" << transition.to << std::endl;
		}

		std::cout << "Now layouting the statemachine...";
		auto layouting_start(std::chrono::steady_clock::now());
		std::vector<std::pair<unsigned, unsigned>> transition_edges(transitions.size());
		std::transform(
			transitions.begin(), transitions.end(),
			transition_edges.begin(),
			[&state_string_to_index](const transition& t)
			{
				return std::make_pair(
					state_string_to_index.find(t.from)->second, 
					state_string_to_index.find(t.to)->second
				);
			}
		);
		auto layout(layouter_brute(transition_edges));
		auto layouting_duration(
			std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::steady_clock::now()-layouting_start
			)
		);
		std::cout << "done" << std::endl;
		std::cout	<< "Layouting finished in "
					<< layouting_duration.count() << "ms" << std::endl;
		std::cout	<< "       " << layout.get_intersection_count() << " intersections" << std::endl;

		linked_state l_states;
		for (auto it(layout.get_knots()); nullptr != it; it = it->next())
		{
			l_states.emplace(
				states.at(it->get_index()).c_str(), 
				it->get().x, 
				it->get().y
			);
		}

		linked_edge l_edges;
		for (auto it(layout.get_strands()); nullptr!=it; it = it->next())
		{
			l_edges.emplace(
				"...", 
				it->get_source(),
				it->get_target()
			);
		}

		draw(l_states, l_edges);

	}

	return EXIT_SUCCESS;

}