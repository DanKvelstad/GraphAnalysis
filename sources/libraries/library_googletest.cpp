#include "msbuild.h"
#include "filesystem.h"
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

void build_vs(const filesystem::path& path_root)
{

	auto path_msvc(path_root / "googletest" / "msvc");

	std::cout << "upgrading the solution... ";
	std::string command_upgrade(find_devenv() + " " + (path_msvc / "gtest.sln").string() + " /upgrade");
	system(command_upgrade.c_str());
	std::cout << "done" << std::endl;

	if(platform::x64 == get_platform())
	{
		msbuild_convert_to_x64(path_msvc);
	}

	msbuild_change_runtime(path_msvc);
	msbuild_remove_project(path_msvc, "gtest_unittest");
	msbuild_remove_project(path_msvc, "gtest_prod_test");
	
	std::string configuration;
	switch(get_configuration())
	{
	case configuration::debug:
		configuration = " /p:Configuration=Debug";
		break;
	case configuration::release:
		configuration = " /p:Configuration=Release";
		break;
	default:
		throw std::exception("unknown configuration");
		break;
	}

	std::string platform;
	switch(get_platform())
	{
	case platform::x86:
		platform = " /p:Platform=x86";
		break;
	case platform::x64:
		platform = " /p:Platform=x64";
		break;
	default:
		throw std::exception("unknown platform");
		break;
	}

	std::string command_build(
		"cd " + (path_root / "googletest" / "msvc").string() + "&&" +
		find_msbuild() + " gtest.sln /t:Build" + configuration+platform
	);

	std::cout << "building with " << configuration << " and " << platform << std::endl;
	system(command_build.c_str());

}

void build_make(const filesystem::path& path_root)
{

}

void move_result(const filesystem::path& path_root)
{

	filesystem::path output(path_root /"googletest"/"msvc"/"gtest");
	filesystem::path gtest;
	filesystem::path gtest_main;

	std::string configuration;
	switch(get_configuration())
	{
	case configuration::debug:
		output /= "Debug";
		filesystem::copy_file(
			output/"gtestd.lib",
			find_output()/"gtestd.lib",
			filesystem::copy_options::overwrite_existing
		);
		filesystem::copy_file(
			output/"gtest_maind.lib",
			find_output()/"gtest_maind.lib",
			filesystem::copy_options::overwrite_existing
		);
		break;
	case configuration::release:
		output /= "Release";
		filesystem::copy_file(
			output / "gtest.lib",
			find_output() / "gtest.lib",
			filesystem::copy_options::overwrite_existing
		);
		filesystem::copy_file(
			output / "gtest_main.lib",
			find_output() / "gtest_main.lib",
			filesystem::copy_options::overwrite_existing
		);
		break;
	default:
		throw std::exception("unknown configuration");
		break;
	}

}

void library_googletest(void)
{

	std::cout	<< "\n+----------------------------------------+\n"
				<< "| Start of Google Test Library ----------|\n"
				<< "+----------------------------------------+" << std::endl;

	bool do_build;

	filesystem::path googletest(find_libraries()/"googletest");
	std::cout << "The path is assumed to be: " << googletest.string() << std::endl;
	
	if(!filesystem::exists(find_output() / "gtestd.lib"))
	{
		std::cout << "The binaries could not be found and therefore needs to be built" << std::endl;
		do_build = true;
	}
	else if(
		git_prepare(
			googletest, 
			"library_googletest.timestamp",				// todo convert into timepoint
			"https://github.com/google/googletest.git"
		)
	)
	{

		auto master_timestamp(git_timestamp(googletest));
		auto output_timestamp(filesystem::last_write_time(find_output() / "gtestd.lib"));
		do_build = master_timestamp > output_timestamp;
		if(do_build)
		{
			std::cout << "The binaries are out-of-date and needs to be rebuilt" << std::endl;
		}
		else
		{
			std::cout << "The binaries are up-to-date and does not need to be rebuilt" << std::endl;
		}

	}
	else
	{
		do_build = false;
	}

	if(do_build)
	{

		switch(get_os())
		{
		case os::windows:
			std::cout << "Rebuilding with msbuild" << std::endl;
			build_vs(googletest);
			break;
		case os::linux:
			std::cout << "Rebuilding with make" << std::endl;
			build_make(googletest);
			break;
		default:
			break;
		}

		move_result(googletest);

	}
	else
	{
		std::cout << "No need to build the library" << std::endl;
	}

	std::cout	<< "+----------------------------------------+\n"
				<< "| End of Google Test Library ------------|\n"
				<< "+----------------------------------------+\n" << std::endl;
	
}