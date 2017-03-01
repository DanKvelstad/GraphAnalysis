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

	std::ofstream ofs(find_intermediate()/"library_googletest.log");

	std::cout << "upgrading the solution... ";
	std::cout.flush();
	std::string command_upgrade(find_devenv() + " " + (path_msvc / "gtest.sln").string() + " /upgrade");
	ofs << command_upgrade << std::endl;
	ofs << console(command_upgrade.c_str());
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
	}

	std::cout << "building solution... ";
	std::cout.flush();
	std::string command_build(
		"cd " + (path_root / "googletest" / "msvc").string() + "&&" +
		find_msbuild() + " gtest.sln /t:Build" + configuration + platform
	);
	ofs << command_build;
	ofs << console(command_build.c_str());
	std::cout << "done" << std::endl;

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
	}

}

void library_googletest(void)
{

	std::cout	<< "\n+----------------------------------------+\n"
				<< "| Start of Google Test Library ----------|\n"
				<< "+----------------------------------------+" << std::endl;

	filesystem::path googletest(find_libraries() / "googletest");

	auto timestamp_git_last_update(get_timestamp(find_intermediate()/"library_googletest.timestamp"));
	auto git_update_delta(std::chrono::system_clock::now()-timestamp_git_last_update);
	std::cout << "git repository was updated ";
	if(git_update_delta < std::chrono::hours(1))
	{
		std::cout	<< std::chrono::duration_cast<std::chrono::minutes>(git_update_delta).count()
					<< " minutes ago" << std::endl;
	}
	else
	{
		std::cout	<< std::chrono::duration_cast<std::chrono::hours>(git_update_delta).count()
					<< " hours ago" << std::endl;
		if(git_update_delta > std::chrono::hours(24))
		{
			git_ensure_up_to_date(
				googletest,
				"https://github.com/google/googletest.git"
			);
		}
	}
	
	bool do_build;

	filesystem::path binary_file;
	switch(get_configuration())
	{
	case configuration::debug:
		binary_file = find_output()/"gtestd.lib";
		break;
	case configuration::release:
		binary_file = find_output()/"gtest.lib";
		break;
	default:
		throw std::exception("unknown configuration");
	}

	if(!filesystem::exists(binary_file))
	{
		std::cout << "The binaries could not be found" << std::endl;
		do_build = true;
	}
	else if(git_get_time_of_latest_commit(googletest)>filesystem::last_write_time(binary_file))
	{
		std::cout << "The binaries are out-of-date" << std::endl;
		do_build = true;
	}
	else
	{
		std::cout << "The binaries are up-to-date" << std::endl;
		do_build = false;
	}

	if(do_build)
	{

		switch(get_os())
		{
		case os::windows:
			build_vs(googletest);
			break;
		case os::linux:
			build_make(googletest);
			break;
		default:
			throw std::exception("unknown os");
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