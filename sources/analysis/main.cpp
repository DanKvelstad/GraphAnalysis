#include "support/support.h"
#include <experimental/filesystem>
#include <iostream>
#include <fstream>
#include <string>

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
	std::vector<path> files;
	for(recursive_directory_iterator it(working_folder);
		recursive_directory_iterator()!=it;
		++it
	)
	{
		if (is_regular_file(*it))
		{
			auto extension(it->path().extension().string());
			std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
			if (".c"	== extension ||
				".cc"	== extension ||
				".cpp"	== extension ||
				".c++"	== extension ||
				".cp"	== extension ||
				".cxx"	== extension ||
				".h"	== extension ||
				".hpp"	== extension )
			{
				files.push_back(it->path());
			}
		}
	}
	std::cout << "done" << std::endl;
	auto search_duration(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - search_start));
	std::cout << "Found " << files.size() << " files in " << search_duration.count() << "ms" << std::endl;

	std::cout << "Searching through the files for state machines...";
	auto search_sm_start(std::chrono::steady_clock::now());
	std::vector<std::pair<path, definition>> definitions; 
	for (auto& file : files)
	{

		std::ifstream t(file);
		std::string content;

		t.seekg(0, std::ios::end);
		content.reserve(t.tellg());
		t.seekg(0, std::ios::beg);

		content.assign((std::istreambuf_iterator<char>(t)),
						std::istreambuf_iterator<char>()  );

		for (auto& definition : find_variable_definition(content))
		{
			definitions.emplace_back(file, definition);
		}

	}
	std::cout << "done" << std::endl;
	auto search_sm_duration(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-search_sm_start));
	std::cout << "Found " << definitions.size() << " state machines in " << search_sm_duration.count() << "ms" << std::endl;

	for (auto& result : definitions)
	{

		std::ifstream t(result.first);
		std::string content;

		t.seekg(0, std::ios::end);
		content.reserve(t.tellg());
		t.seekg(0, std::ios::beg);

		content.assign((std::istreambuf_iterator<char>(t)),
						std::istreambuf_iterator<char>());

		std::cout << std::endl << "Statemachine report"			<< std::endl;
		std::cout << "Name   " << result.second.variable	<< std::endl;
		std::cout << "File   " << result.first				<< std::endl;
		std::cout << "States " << result.second.states		<< std::endl;
		for (auto& state : find_enum_class(content, result.second.states))
		{
			std::cout << "       " << state << std::endl;
		}
		std::cout << "Edges  " << result.second.edges << std::endl;
		for (auto& edge : find_enum_class(content, result.second.edges))
		{
			std::cout << "       " << edge << std::endl;
		}

	}

	return EXIT_SUCCESS;

}