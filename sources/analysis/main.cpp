#include "compiler/compiler.h"
#include "presenter/presenter.h"
#include <experimental/filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <layouter\layouters.h>
#include <map>
#include <vector>
#include <utility>

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

	std::cout << "Now compiling the statemachine...";
	auto compiling_start(std::chrono::steady_clock::now());
	auto parsed(parse(working_folder));
	auto compiling_duration(
		std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::steady_clock::now()- compiling_start
		)
	);
	std::cout << "done" << std::endl;
	std::cout	<< "Compiling finished in "
				<< compiling_duration.count() << "ms" << std::endl;
	
	{	// parse should return many parsed statemachines, iterate over them

		std::vector<std::pair<unsigned, unsigned>> transition_edges;
		// transition_edges.reserve(parsed.transition_count());
		for (auto it(&parsed.get_transitions()); nullptr!=it; it = it->next())
		{
			transition_edges.emplace_back(it->get_source(), it->get_target());
		}

		std::cout << "Now layouting the statemachine...";
		auto layouting_start(std::chrono::steady_clock::now());
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

		presenter the_presenter;
		
		for (auto it(layout.get_knots()); nullptr != it; it = it->next())
		{
			the_presenter.emplace_state(
				parsed.get_state(it->get_index()).get(), 
				it->get().x, 
				it->get().y
			);
		}

		for (auto it(layout.get_strands()); nullptr!=it; it = it->next())
		{
			the_presenter.emplace_edge(
				it->get_source(),
				it->get_target(),
				"..."
			);
		}

		the_presenter.draw(path(canonical(argv[0]).parent_path())/"analysis.png");

	}

	return EXIT_SUCCESS;

}