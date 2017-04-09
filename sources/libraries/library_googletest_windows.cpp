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
	
	auto library = path_to_libraries() / build_directory();
	if(!filesystem::exists(library))
	{
		filesystem::create_directories(library);
	}
	
	return library / "gtest.lib";

}

filesystem::path path_to_gtest_main(void)
{
	
	auto library = path_to_libraries() / build_directory();
	if(!filesystem::exists(library))
	{
		filesystem::create_directories(library);
	}
	
	return library / "gtest_main.lib";

}

void build(const filesystem::path& path_to_root)
{

	std::ofstream ofs(path_to_intermediate()/"googletest.log");
	
	// http://stackoverflow.com/a/9965141
	std::string command_build("\"\"" + path_to_msbuild().string() + "\"");
	
	filesystem::path path_to_solution;
	filesystem::path path_to_built;
	auto devenv(path_to_devenv());
	auto cmake(path_to_cmake());
	if(!devenv.empty())
	{
		std::cout << "upgrading the solution through devenv... ";
		std::cout.flush();
		path_to_solution = path_to_root / "googletest" / "msvc";
		command_build += " \"" + (path_to_solution/"gtest.sln").string() + "\"";
		switch (get_configuration())
		{
		case configuration::release:
			path_to_built = path_to_solution / "gtest" / "release";
			break;
		case configuration::debug:
			path_to_built = path_to_solution / "gtest" / "debug";
			break;
		default:
			throw std::logic_error("unknown configuration");
			break;
		}
		std::string command("\"" + devenv.string() + "\" " + (path_to_solution / "gtest.sln").string() + " /upgrade");
		ofs << console(command) << std::endl << std::endl;
		std::cout << "done" << std::endl;
	}
	else if(!cmake.empty())
	{
		std::cout << "creating a new solution through cmake... ";
		std::cout.flush();
		path_to_solution = path_to_root / "googletest";
		command_build += " \"" + (path_to_solution/"gtest.sln").string() + "\"";
		switch (get_configuration())
		{
		case configuration::release:
			path_to_built = path_to_solution / "Release";
			break;
		case configuration::debug:
			path_to_built = path_to_solution / "Debug";
			break;
		default:
			throw std::logic_error("unknown configuration");
			break;
		}
		std::string command("cd \"" + (path_to_root/"googletest").string() + "\"&&\"" + cmake.string() + "\" .");
		ofs << console(command) << std::endl << std::endl;
		std::cout << "done" << std::endl;
	}
	else
	{
		throw std::logic_error("cannot create a usable solution");
	}
	
	command_build += " /t:Build";

	msbuild_ensure_platform(path_to_solution);
	msbuild_change_runtime(path_to_solution);
	msbuild_remove_project(path_to_solution, "gtest_unittest");
	msbuild_remove_project(path_to_solution, "gtest_prod_test");

	std::cout << "building solution... ";
	std::cout.flush();

	switch(get_configuration())
	{
	case configuration::debug:
		command_build += " /p:Configuration=debug";
		break;
	case configuration::release:
		command_build += " /p:Configuration=release";
		break;
	default:
		throw std::logic_error("unknown configuration");
	}

	switch(get_platform())
	{
	case platform::x86:
		command_build += " /p:Platform=Win32";
		break;
	case platform::x64:
		command_build += " /p:Platform=x64";
		break;
	default:
		throw std::logic_error("unknown platform");
	}

	command_build += "\"";
	ofs << command_build << std::endl;
	ofs << console(command_build) << std::endl;
	std::cout << "done" << std::endl;

	ofs.close();

	std::error_code error;
	filesystem::rename(path_to_built/"gtest.lib",		path_to_gtest_all(),	error);
	filesystem::rename(path_to_built/"gtest_main.lib",	path_to_gtest_main(),	error);
	filesystem::rename(path_to_built/"gtestd.lib",		path_to_gtest_all(),	error);
	filesystem::rename(path_to_built/"gtest_maind.lib",	path_to_gtest_main(),	error);
	
	auto path_to_library_includes(path_to_library_includes() / "gtest");
	if (filesystem::exists(path_to_library_includes))
	{
		filesystem::remove_all(path_to_library_includes);
	}
	filesystem::rename(path_to_root / "googletest" / "include" / "gtest", path_to_library_includes);

}

bool verify_output(void)
{
	return	filesystem::exists(path_to_gtest_all()) &&
			filesystem::exists(path_to_gtest_main());
}