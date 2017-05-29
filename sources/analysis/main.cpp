#include "compiler/compiler.h"
#include "presenter/presenter.h"
#include <experimental/filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <layouter\layouter.h>
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

//		layouter layout;
//		for (auto it(&parsed.get_transitions()); nullptr!=it; it = it->next())
//		{
//			layout.emplace(it->get_source(), it->get_target());
//		}
//
//		std::cout << "Now layouting the statemachine...";
//		auto layouting_start(std::chrono::steady_clock::now());
//		auto the_layout(layout.layout());
//		auto layouting_duration(
//			std::chrono::duration_cast<std::chrono::milliseconds>(
//				std::chrono::steady_clock::now()-layouting_start
//			)
//		);
//		std::cout << "done" << std::endl;
//		std::cout	<< "Layouting finished in "
//					<< layouting_duration.count() << "ms" << std::endl;
//		
//		presenter the_presenter;
//		
//		for (unsigned i(0); i<the_layout.size(); i++)
//		{
//			the_presenter.emplace_state(
//				parsed.get_state(i).get(), 
//				the_layout.at(i).first, 
//				the_layout.at(i).second
//			);
//		}
//
//		for (auto it(&parsed.get_transitions()); nullptr != it; it = it->next())
//		{
//			the_presenter.emplace_edge(
//				it->get_source(),
//				it->get_target(),
//				"..."
//			);
//		}
//
//		the_presenter.draw(path(canonical(argv[0]).parent_path())/"analysis.png");

	}

	return EXIT_SUCCESS;

}