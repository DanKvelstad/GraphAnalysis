#include "msbuild.h"
#include "files.h"
#include "os.h"
#include "git.h"
#include <experimental/filesystem>
#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include <fstream>
#include <thread>
#include <chrono>
#include "libraries.h"

using namespace std::experimental;

void build(const filesystem::path& path_root);
bool verify_output(void);

void library_googletest(void)
{

	std::cout	<< "--- Google Test Library ----------" << std::endl;

	filesystem::path path_to_workspace(path_to_repositories() / "googletest");

	auto do_build(
		git_update(
			path_to_timestamps()/"googletest.timestamp",
			path_to_workspace,
			"https://github.com/google/googletest.git"
		)
	);

	if(do_build)
	{
		std::cout << "building the binaries due to an updated workspace" << std::endl;
		build(path_to_workspace);
	}
	else if(!verify_output())
	{
		std::cout << "building the binaries because they were not found" << std::endl;
		build(path_to_workspace);
	}
	else
	{
		std::cout << "no need to build the library" << std::endl;
	}

	std::cout << std::endl;
	
}