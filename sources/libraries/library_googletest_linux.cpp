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

filesystem::path path_to_gtest_all(void)
{
	return path_to_libraries() / "linux" / "gtest-all.o";
}

filesystem::path path_to_gtest_main(void)
{
	return path_to_libraries() / "linux" / "gtest_main.o";
}

void build(const filesystem::path& path_root)
{

	auto path_make(path_root / "googletest" / "make");

	std::string command_make("cd " + (path_make).string() + "&&make");
	console(command_make.c_str());

	{
		std::error_code error;
		filesystem::rename(path_make/"gtest-all.o", path_to_gtest_all(), error);
		if(error)
		{
			throw std::logic_error(error.message());
		}
	}

	{
		std::error_code error;
		filesystem::rename(path_make/"gtest_main.o", path_to_gtest_main(), error);
		if(error)
		{
			throw std::logic_error(error.message());
		}
	}

}

bool verify_output(void)
{
	return	filesystem::exists(path_to_gtest_all()) &&
			filesystem::exists(path_to_gtest_main());
}